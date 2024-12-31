// Fill out your copyright notice in the Description page of Project Settings.


#include "Catenary/CatenaryWidget.h"

#include "CatenaryBuilder.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetStringLibrary.h"

TSharedRef<SWidget> UCatenaryWidget::RebuildWidget()
{
	SlateCatenary = SNew(SCatenary).Catenary_UObject(this, &ThisClass::GetCatenaryData);
	return SlateCatenary.ToSharedRef();
}

void UCatenaryWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	for (int i = 0; i < Catenaries.Num(); i++)
	{
		TArray<FVector2D> CatenaryPoints;
		FCatenaryBuilder::BuildCatenaryPoints(Catenaries[i], CatenaryPoints);
		Catenaries[i].CatenaryPoints = CatenaryPoints;
	}
}

void UCatenaryWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	SlateCatenary.Reset();
}

void UCatenaryWidget::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	const FProperty* Property = PropertyChangedEvent.Property;
	if (!Property)
	{
		return;
	}
	const FProperty* MemberProperty = PropertyChangedEvent.PropertyChain.GetActiveMemberNode()->GetValue();
	if (!MemberProperty)
	{
		return;
	}
	
	const FString MemberPropertyName = MemberProperty->GetName();
	if (MemberPropertyName == TEXT("Catenaries"))
	{
		const int32 ArrayIndex = PropertyChangedEvent.GetArrayIndex(MemberPropertyName);
		if (Catenaries.IsValidIndex(ArrayIndex))
		{
			Catenaries[ArrayIndex].bIsDirty = true;
		}
		
	}
}

FSlateCatenary UCatenaryWidget::GetCatenaryData() const
{
	return FSlateCatenary(Catenaries, Cast<UWidgetTree>(GetOuter()));
}

void UCatenaryWidget::Tick(float DeltaTime)
{
	if (LastFrameNumberWeTicked == GFrameCounter)
	{
		return;
	}

	for (int i = 0; i < Catenaries.Num(); i++)
	{
		FCatenaryArguments Arg = Catenaries[i];
		FVector2D DesiredP1 = GetDesiredLocation(Arg.P1ConnectionSchema);
		FVector2D DesiredP2 = GetDesiredLocation(Arg.P2ConnectionSchema);
		
		if (Arg.bIsDirty || !DesiredP1.Equals(Arg.P1ConnectionSchema.Point) || !DesiredP2.Equals(Arg.P2ConnectionSchema.Point))
		{
			Catenaries[i].P1ConnectionSchema.Point = DesiredP1;
			Catenaries[i].P2ConnectionSchema.Point = DesiredP2;
			
			TArray<FVector2D> CatenaryPoints;
			FCatenaryBuilder::BuildCatenaryPoints(Catenaries[i], CatenaryPoints);
			Catenaries[i].CatenaryPoints = CatenaryPoints;

			Catenaries[i].bIsDirty = false;
		}
	}

	LastFrameNumberWeTicked = GFrameCounter;
}

FVector2D UCatenaryWidget::GetDesiredLocation(const FCatenaryConnectionSchema& InSchema) const
{
	if (InSchema.PointConnectionRule == ECatenaryConnectionRule::Widget)
	{
		if (UWidgetTree* WidgetTree = Cast<UWidgetTree>(GetOuter()))
		{
			if (UCanvasPanelSlot* CPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(WidgetTree->FindWidget(InSchema.PointWidgetName)))
			{
				return CPS->GetPosition();
			}
		}
	}
	return InSchema.Point;
}

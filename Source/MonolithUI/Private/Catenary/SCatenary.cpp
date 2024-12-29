// Fill out your copyright notice in the Description page of Project Settings.


#include "Catenary/SCatenary.h"

#include "CatenaryBuilder.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Widget.h"

void SCatenary::Construct(const FArguments& InArguments)
{
	Catenary = InArguments._Catenary;

	// UWidgetTree* Test;
	// if (CatenaryRef.GetWidgetTree(Test))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("WidgetTree Found"))
	// }
}

FVector2D SCatenary::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(100.0f, 30.0f);
}

int32 SCatenary::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	const FSlateCatenary& CatenaryRef = Catenary.Get();

	const int PaintLayerId = LayerId + 1;

	for (auto& CatenaryArg : CatenaryRef.Catenaries)
	{
		// Check if a particular Catenary needs to be updated
		
		FVector2D DesiredP1 = GetDesiredLocation(CatenaryArg.P1ConnectionSchema);
		FVector2D DesiredP2 = GetDesiredLocation(CatenaryArg.P2ConnectionSchema);

		// Points are dirty if their current position does not match with their desired location.
		// UE_LOG(LogTemp, Warning, TEXT("DesiredP1: %s | CurrentP1: %s | DesiredP2: %s | CurrentP2: %s"), *DesiredP1.ToString(), *CatenaryArg.P1ConnectionSchema.Point.ToString(), *DesiredP2.ToString(), *CatenaryArg.P2ConnectionSchema.Point.ToString())
		bool bIsDirty = !DesiredP1.Equals(CatenaryArg.P1ConnectionSchema.Point) || !DesiredP2.Equals(CatenaryArg.P2ConnectionSchema.Point);

		if (bIsDirty)
		{
			// If points are dirty, update location.
			CatenaryArg.P1ConnectionSchema.Point = DesiredP1;
			CatenaryArg.P2ConnectionSchema.Point = DesiredP2;
		}
		
		const FSlateCatenaryPaintContext PaintContext(OutDrawElements, AllottedGeometry, PaintLayerId,
		ShouldBeEnabled(bParentEnabled) ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect,
		CatenaryArg.Brush.TintColor.GetColor(InWidgetStyle).ToFColorSRGB());

		PaintCatenary(PaintContext, CatenaryArg, bIsDirty);
	}
	
	return PaintLayerId;
}


void SCatenary::PaintCatenary(const FSlateCatenaryPaintContext& InPaintContext, const FCatenaryArguments& InCatenaryArguments, const bool& bIsDirty) const
{
	FCatenaryBuilder CatenaryBuilder(InCatenaryArguments.Brush.GetImageSize(), InPaintContext);

	// if the catenary is dirty, recalculate the Catenary points
	if (!Catenaries.Contains(InCatenaryArguments) || bIsDirty)
	{
		TArray<FVector2D> CatenaryPoints;
		CatenaryBuilder.BuildCatenaryPoints(InCatenaryArguments, CatenaryPoints);
		Catenaries.Add(InCatenaryArguments, CatenaryPoints);
		CatenaryBuilder.BuildCatenaryGeometry(CatenaryPoints);
	}
	else
	{
		CatenaryBuilder.BuildCatenaryGeometry(Catenaries.FindRef(InCatenaryArguments));
	}

	const FSlateResourceHandle& RenderResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(InCatenaryArguments.Brush);
	FSlateDrawElement::MakeCustomVerts(InPaintContext.OutDrawElements, InPaintContext.LayerId, RenderResourceHandle, CatenaryBuilder.Vertices, CatenaryBuilder.Indices, nullptr, 0, 0, InPaintContext.DrawEffect);
	
}

FVector2D SCatenary::GetDesiredLocation(const FCatenaryConnectionSchema& InSchema) const
{
	if (InSchema.PointConnectionRule == ECatenaryConnectionRule::Widget)
	{
		const FSlateCatenary& CatenaryRef = Catenary.Get();
		UWidgetTree* WidgetTree;
		if (CatenaryRef.GetWidgetTree(WidgetTree))
		{
			if (UCanvasPanelSlot* CPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(WidgetTree->FindWidget(InSchema.PointWidgetName)))
			{
				return CPS->GetPosition();
			}
		}
	}
	return InSchema.Point;
}

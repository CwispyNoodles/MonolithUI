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

	// const FSlateCatenary& CatenaryRef = Catenary.Get();
	// CatenaryArguments_Copy = CatenaryRef.Catenaries;
	// // TODO: Initialize CatenaryPoints
	// for (auto& Arg : CatenaryArguments_Copy)
	// {
	// 	TArray<FVector2D> CatenaryPoints;
	// 	FCatenaryBuilder::BuildCatenaryPoints(Arg, CatenaryPoints);
	//
	// 	CatenaryPointMap.Add(Arg, CatenaryPoints);
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
	const int PaintLayerId = LayerId + 1;

	const FSlateCatenary& CatenaryRef = Catenary.Get();
	
	for (auto& Arg : CatenaryRef.Catenaries)
	{
		const FSlateCatenaryPaintContext PaintContext(OutDrawElements, AllottedGeometry, PaintLayerId,
		ShouldBeEnabled(bParentEnabled) ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect,
		Arg.Brush.TintColor.GetColor(InWidgetStyle).ToFColorSRGB());

		FCatenaryBuilder CatenaryBuilder(Arg.Brush.GetImageSize(), PaintContext);

		CatenaryBuilder.BuildCatenaryGeometry(Arg.CatenaryPoints);

		const FSlateResourceHandle& RenderResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(Arg.Brush);
		FSlateDrawElement::MakeCustomVerts(PaintContext.OutDrawElements, PaintContext.LayerId, RenderResourceHandle, CatenaryBuilder.Vertices, CatenaryBuilder.Indices, nullptr, 0, 0, PaintContext.DrawEffect);
	

		// PaintCatenary(PaintContext, CatenaryArg, bIsDirty);
	}
	
	return PaintLayerId;
}

void SCatenary::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SLeafWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	// const FSlateCatenary& CatenaryRef = Catenary.
	//
	// for (int i = 0; i < CatenaryRef.Catenaries.Num(); i++)
	// {
	// 	CatenaryRef.Catenaries[i].P1ConnectionSchema.Point = FVector2D(0.0f);
	// }

	// for (auto& Arg : CatenaryArguments_Copy)
	// {
	// 	FVector2D DesiredP1 = GetDesiredLocation(Arg.P1ConnectionSchema);
	// 	FVector2D DesiredP2 = GetDesiredLocation(Arg.P2ConnectionSchema);
	//
	// 	// Determine if point is dirty
	// 	if (!DesiredP1.Equals(Arg.P1ConnectionSchema.Point) || !DesiredP2.Equals(Arg.P2ConnectionSchema.Point))
	// 	{
	// 		CatenaryPointMap.Remove(Arg);
	// 		Arg.P1ConnectionSchema.Point = DesiredP1;
	// 		Arg.P2ConnectionSchema.Point = DesiredP2;
	//
	// 		TArray<FVector2D> CatenaryPoints;
	// 		FCatenaryBuilder::BuildCatenaryPoints(Arg, CatenaryPoints);
	//
	// 		CatenaryPointMap.Add(Arg, CatenaryPoints);
	// 	}
	// }
}


void SCatenary::PaintCatenary(const FSlateCatenaryPaintContext& InPaintContext, const FCatenaryArguments& InCatenaryArguments, const bool& bIsDirty) const
{
	// FCatenaryBuilder CatenaryBuilder(InCatenaryArguments.Brush.GetImageSize(), InPaintContext);
	//
	// // if the catenary is dirty, recalculate the Catenary points
	// if (!Catenaries.Contains(InCatenaryArguments) || bIsDirty)
	// {
	// 	TArray<FVector2D> CatenaryPoints;
	// 	CatenaryBuilder.BuildCatenaryPoints(InCatenaryArguments, CatenaryPoints);
	// 	Catenaries.Add(InCatenaryArguments, CatenaryPoints);
	// 	CatenaryBuilder.BuildCatenaryGeometry(CatenaryPoints);
	// }
	// else
	// {
	// 	CatenaryBuilder.BuildCatenaryGeometry(Catenaries.FindRef(InCatenaryArguments));
	// }
	//
	// const FSlateResourceHandle& RenderResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(InCatenaryArguments.Brush);
	// FSlateDrawElement::MakeCustomVerts(InPaintContext.OutDrawElements, InPaintContext.LayerId, RenderResourceHandle, CatenaryBuilder.Vertices, CatenaryBuilder.Indices, nullptr, 0, 0, InPaintContext.DrawEffect);
	//
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

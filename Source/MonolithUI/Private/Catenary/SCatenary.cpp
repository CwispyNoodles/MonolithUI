// Fill out your copyright notice in the Description page of Project Settings.


#include "Catenary/SCatenary.h"

#include "CatenaryBuilder.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"

void SCatenary::Construct(const FArguments& InArguments)
{
	Catenary = InArguments._Catenary;
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

	}
	
	return PaintLayerId;
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

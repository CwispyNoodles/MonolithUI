// Fill out your copyright notice in the Description page of Project Settings.


#include "Catenary/SCatenary.h"

#include "CatenaryBuilder.h"

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
	const FSlateCatenary& CatenaryRef = Catenary.Get();

	const int PaintLayerId = LayerId + 1;

	for (auto& CatenaryArg : CatenaryRef.Catenaries)
	{
		const FSlateCatenaryPaintContext PaintContext(OutDrawElements, AllottedGeometry, PaintLayerId,
		ShouldBeEnabled(bParentEnabled) ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect,
		CatenaryArg.Brush.TintColor.GetColor(InWidgetStyle).ToFColorSRGB());

		PaintCatenary(PaintContext, CatenaryArg);
	}
	
	return PaintLayerId;
}


void SCatenary::PaintCatenary(const FSlateCatenaryPaintContext& InPaintContext, const FCatenaryArguments& InCatenaryArguments) const
{
	FCatenaryBuilder CatenaryBuilder(InCatenaryArguments.Brush.GetImageSize(), InPaintContext);
	
	CatenaryBuilder.BuildCatenaryGeometry(InCatenaryArguments);

	const FSlateResourceHandle& RenderResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(InCatenaryArguments.Brush);
	FSlateDrawElement::MakeCustomVerts(InPaintContext.OutDrawElements, InPaintContext.LayerId, RenderResourceHandle, CatenaryBuilder.Vertices, CatenaryBuilder.Indices, nullptr, 0, 0, InPaintContext.DrawEffect);
	
}

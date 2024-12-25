// Fill out your copyright notice in the Description page of Project Settings.


#include "Catenary/SCatenary.h"

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
	return LayerId;
}

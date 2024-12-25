// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Catenary/SlateCatenary.h"

/**
 * 
 */
class MONOLITHUI_API SCatenary : public SLeafWidget
{
protected:
	TAttribute<FSlateCatenary> Catenary;
public:
	SLATE_BEGIN_ARGS(SCatenary) : _Catenary() {}
	SLATE_ATTRIBUTE(FSlateCatenary, Catenary);
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArguments);
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

};

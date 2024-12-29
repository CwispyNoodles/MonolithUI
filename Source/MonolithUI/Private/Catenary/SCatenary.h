// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateCatenaryPaintContext.h"
#include "Catenary/SlateCatenary.h"

/**
 * 
 */
class MONOLITHUI_API SCatenary : public SLeafWidget
{

public:
	SLATE_BEGIN_ARGS(SCatenary) : _Catenary() {}
	SLATE_ATTRIBUTE(FSlateCatenary, Catenary);
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArguments);
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
	virtual void PaintCatenary(const FSlateCatenaryPaintContext& InPaintContext, const FCatenaryArguments& InCatenaryArguments, const bool& bIsDirty) const;
	FVector2D GetDesiredLocation(const FCatenaryConnectionSchema& InSchema) const;
protected:
	mutable TMap<FCatenaryArguments, TArray<FVector2D>> Catenaries;
	
	TAttribute<FSlateCatenary> Catenary;
};

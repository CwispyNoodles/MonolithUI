// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

struct MONOLITHUI_API FSlateCatenaryPaintContext
{
	FSlateCatenaryPaintContext() = delete;
	FSlateCatenaryPaintContext(FSlateWindowElementList& OutDrawElements, const FGeometry& InGeometry, const int32 InLayerId, const ESlateDrawEffect InDrawEffect, const FColor InTintColor)
		: OutDrawElements(OutDrawElements)
		, AllotedGeometry(InGeometry)
		, DrawEffect(InDrawEffect)
		, TintColor(InTintColor)
		, LayerId(InLayerId)
	{
		PaintGeometry = AllotedGeometry.ToPaintGeometry();
	}

	const FSlateRenderTransform& GetRenderTransform() const
	{
		PaintGeometry.CommitTransformsIfUsingLegacyConstructor();
		return PaintGeometry.GetAccumulatedRenderTransform();
	}

	FSlateWindowElementList& OutDrawElements;
	const FGeometry& AllotedGeometry;
	const ESlateDrawEffect DrawEffect;
	const FColor TintColor;
	FPaintGeometry PaintGeometry;
	int32 LayerId;
};


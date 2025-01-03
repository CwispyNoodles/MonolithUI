// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateCatenaryPaintContext.h"
#include "Catenary/SlateCatenary.h"

struct MONOLITHUI_API FCatenaryBuilder
{
	FCatenaryBuilder(const FVector2D& InSize, const FSlateCatenaryPaintContext& PaintContext);
	
	static void BuildCatenaryPoints(FCatenaryArguments InArgs, TArray<FVector2D>& InCatenaryPoints);

	void BuildCatenaryGeometry(TArray<FVector2D> InPoints);

	TArray<FSlateVertex> Vertices;
	TArray<SlateIndex> Indices;
	float HalfLineThickness;

private:
	// Code for AppendPoint taken from https://github.com/ArmainAP/Unreal-Engine-Widget-Spline-System
	void AppendPoint(FVector2D NewPoint);
	void Finish();

	const FSlateRenderTransform& RenderTransform;
	const FColor SingleColor;

	const FVector2f TextureCoord2;
	const float CoordScaleV;

	FVector2D LastPointAdded[2];
	FVector2D LastNormal;
	int32 NumPointsAdded = 0;
	float CurrentLength = 0.0f;
	float CurrentCoordV = 0.0f;
};

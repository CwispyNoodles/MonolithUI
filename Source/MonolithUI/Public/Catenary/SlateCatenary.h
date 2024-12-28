// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateCatenary.generated.h"


class UWidgetTree;

UENUM(BlueprintType)
enum class ECatenaryConnectionSchema : uint8
{
	Open,
	WidgetAttached
};

USTRUCT(BlueprintType)
struct MONOLITHUI_API FCatenaryConnectionSchema
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Schema Type")
	ECatenaryConnectionSchema PointConnectionSchema = ECatenaryConnectionSchema::Open;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="PointConnectionSchema==ECatenaryConnectionSchema::Open", EditConditionHides))
	FVector2D Point = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="PointConnectionSchema==ECatenaryConnectionSchema::WidgetAttached", EditConditionHides))
	FName PointWidgetName = NAME_None;
};

USTRUCT(BlueprintType)
struct MONOLITHUI_API FCatenaryArguments
{
	GENERATED_BODY()
	
	FCatenaryArguments()
	{
		Brush.SetImageSize(FVector2D::Unit45Deg);
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Point 1 Connection Schema")
	FCatenaryConnectionSchema P1ConnectionSchema;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Point 2 Connection Schema")
	FCatenaryConnectionSchema P2ConnectionSchema;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Length = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Iterations = 6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=1))
	int Segments = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0.01f))
	float IntervalStep = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0.0001f))
	float Precision = 0.0001f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSlateBrush Brush = FSlateBrush();
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct MONOLITHUI_API FSlateCatenary
{
	GENERATED_BODY()

	FSlateCatenary()
	{
	}

	FSlateCatenary(TArray<FCatenaryArguments> InCatenaries, UWidgetTree* InWidgetTree)
		: Catenaries(InCatenaries), WidgetTree(InWidgetTree)
	{}

	bool GetWidgetTree(UWidgetTree*& InWidgetTree) const
	{
		if (WidgetTree)
		{
			InWidgetTree = WidgetTree;
			return true;
		}
		return false;
	}

	UPROPERTY()
	TArray<FCatenaryArguments> Catenaries = TArray<FCatenaryArguments>();

private:
	UPROPERTY()
	UWidgetTree* WidgetTree = nullptr;
};

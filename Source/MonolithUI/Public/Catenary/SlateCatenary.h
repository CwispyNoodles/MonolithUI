// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateCatenary.generated.h"


UENUM(BlueprintType)
enum class ECatenaryConnectionSchema : uint8
{
	Open,
	WidgetAttached
};

USTRUCT(BlueprintType)
struct MONOLITHUI_API FCatenaryArguments
{
	GENERATED_BODY()
	
	FCatenaryArguments()
	{
		Brush.SetImageSize(FVector2D::Unit45Deg);
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="P1 Connection Schema")
	ECatenaryConnectionSchema P1ConnectionSchema = ECatenaryConnectionSchema::Open;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="P1ConnectionSchema==ECatenaryConnectionSchema::Open", EditConditionHides))
	FVector2D P1 = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="P1ConnectionSchema==ECatenaryConnectionSchema::WidgetAttached", EditConditionHides))
	FName P1WidgetName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="P2 Connection Schema")
	ECatenaryConnectionSchema P2ConnectionSchema = ECatenaryConnectionSchema::Open;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="P2ConnectionSchema==ECatenaryConnectionSchema::Open", EditConditionHides))
	FVector2D P2 = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="P2ConnectionSchema==ECatenaryConnectionSchema::WidgetAttached", EditConditionHides))
	FName P2WidgetName = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Length = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Iterations = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=1))
	int Segments = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.01f))
	float IntervalStep = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.0001f))
	float Precision = 0.0001f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush Brush = FSlateBrush();
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct MONOLITHUI_API FSlateCatenary
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCatenaryArguments> Catenaries;
};

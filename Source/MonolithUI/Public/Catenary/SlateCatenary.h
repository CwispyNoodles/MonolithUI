// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateCatenary.generated.h"


class UWidgetTree;

UENUM(BlueprintType)
enum class ECatenaryConnectionRule : uint8
{
	Open,
	Widget,
	// Cursor
};

USTRUCT(BlueprintType)
struct MONOLITHUI_API FCatenaryConnectionSchema
{
	GENERATED_BODY()

	bool operator==(const FCatenaryConnectionSchema& Other) const
	{
		return Point == Other.Point;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Schema Type")
	ECatenaryConnectionRule PointConnectionRule = ECatenaryConnectionRule::Open;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="PointConnectionRule==ECatenaryConnectionRule::Open", EditConditionHides))
	mutable FVector2D Point = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="PointConnectionRule==ECatenaryConnectionRule::Widget", EditConditionHides))
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

	bool operator==(const FCatenaryArguments& Other) const
	{
		return P1ConnectionSchema == Other.P1ConnectionSchema && P2ConnectionSchema == Other.P2ConnectionSchema
		&& Length == Other.Length
		&& Segments == Other.Segments
		&& Brush == Other.Brush;
	}

	// Defines the behavior of Point 1. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Point 1 Connection Schema")
	FCatenaryConnectionSchema P1ConnectionSchema;

	// Defines the behavior of Point 2. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Point 2 Connection Schema")
	FCatenaryConnectionSchema P2ConnectionSchema;

	// The Length of the Catenary.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Length = 500.0f;

	// Number of Segments rendered on the Catenary.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=1))
	int Segments = 10;

	// The equation used for solving for a particular Catenary is 'y = a cosh((x-p)/a)+q'
	// where 'p' and 'q' are x and y offsets and 'a' is the size and scale of the Catenary.
	// 'a' has no closed form solution so we must iteratively solve for it.
	// IntervalStep helps us by first calculating a rough initial estimate for 'a', before we fine tune it
	// on a later step.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0.01f))
	float IntervalStep = 5.0f;

	// The equation used for solving for a particular Catenary is 'y = a cosh((x-p)/a)+q'
	// where 'p' and 'q' are x and y offsets and 'a' is the size and scale of the Catenary.
	// 'a' has no closed form solution so we must iteratively solve for it.
	// When we fine tune a rough estimate of 'a' we iteratively refine our estimate via bisection.
	// Iterations is the maximum number of bisections should we not arrive at our desired Precision for 'a'
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Iterations = 15;

	// The equation used for solving for a particular Catenary is 'y = a cosh((x-p)/a)+q'
	// where 'p' and 'q' are x and y offsets and 'a' is the size and scale of the Catenary.
	// 'a' has no closed form solution so we must iteratively solve for it.
	// Precision is the minimum desired precision we iteratively refine our rough estimate of 'a'.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0.0001f))
	float Precision = 0.0001f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSlateBrush Brush = FSlateBrush();

	UPROPERTY()
	TArray<FVector2D> CatenaryPoints;

	UPROPERTY()
	bool bIsDirty = false;
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

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FCatenaryArguments& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FCatenaryArguments& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FCatenaryArguments));
	return Hash;
}
#endif
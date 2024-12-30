// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateCatenary.h"
#include "Catenary/SCatenary.h"
#include "Components/Widget.h"
#include "CatenaryWidget.generated.h"

/**
 * 
 */
UCLASS()
class MONOLITHUI_API UCatenaryWidget : public UWidget, public FTickableGameObject
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnWidgetRebuilt() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override
	{
		return NSLOCTEXT("Monolith UI", "Monolith UI", "Monolith UI");
	};
#endif

	TSharedPtr<SCatenary> SlateCatenary;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCatenaryArguments> Catenaries;

public:
	FSlateCatenary GetCatenaryData() const;

public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UCatenaryWidget, STATGROUP_Tickables);
	}
	virtual bool IsTickableWhenPaused() const
	{
		return true;
	}
	virtual bool IsTickableInEditor() const
	{
		return true;
	}
	// FTickableGameObject End


private:
	FVector2D GetDesiredLocation(const FCatenaryConnectionSchema& InSchema) const;
	
	// The last frame number we were ticked.
	// We don't want to tick multiple times per frame 
	uint32 LastFrameNumberWeTicked = INDEX_NONE;

};

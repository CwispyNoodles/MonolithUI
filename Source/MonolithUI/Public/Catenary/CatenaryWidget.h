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
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent);
	
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
	// FTickableGameObject
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UCatenaryWidget, STATGROUP_Tickables);
	}
	virtual bool IsTickableWhenPaused() const override
	{
		return true;
	}
	virtual bool IsTickableInEditor() const override
	{
		return true;
	}

#if WITH_EDITOR
	virtual bool IsTickable() const override
	{
		return IsDesignTime() || (IsValid(GetWorld()) && (GetWorld()->WorldType == EWorldType::PIE || GetWorld()->WorldType == EWorldType::GamePreview || GetWorld()->WorldType == EWorldType::Game));
	}
#endif
	// FTickableGameObject End


private:
	/*
	 * TODO: This is currently reliant on attached widgets being children of a Canvas Panel.
	 * Change it so that CatenaryWidget accepts children so that we can extract locational
	 * data directly.
	 **/
	FVector2D GetDesiredLocation(const FCatenaryConnectionSchema& InSchema) const;
	
	// The last frame number we were ticked.
	// We don't want to tick multiple times per frame 
	uint32 LastFrameNumberWeTicked = INDEX_NONE;

};

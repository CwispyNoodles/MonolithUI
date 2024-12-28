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
class MONOLITHUI_API UCatenaryWidget : public UWidget
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
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
};

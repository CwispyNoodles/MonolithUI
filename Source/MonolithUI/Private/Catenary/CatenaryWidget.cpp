// Fill out your copyright notice in the Description page of Project Settings.


#include "Catenary/CatenaryWidget.h"

TSharedRef<SWidget> UCatenaryWidget::RebuildWidget()
{
	SlateCatenary = SNew(SCatenary).Catenary_UObject(this, &ThisClass::GetCatenaryData);
	return SlateCatenary.ToSharedRef();
}

void UCatenaryWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	SlateCatenary.Reset();
}

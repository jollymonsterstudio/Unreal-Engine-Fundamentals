// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameHUD.h"

AInGameHUD::AInGameHUD()
{

}

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HitComboWidgetClass)
	{
		HitComboWidget = CreateWidget<UHitComboWidget>(GetWorld(), HitComboWidgetClass);

		/** Make sure widget was created */
		if (HitComboWidget)
		{
			/** Add it to the viewport */
			HitComboWidget->AddToViewport();
		}
	}
}

void AInGameHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HitComboWidget)
	{

	}
}

void AInGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AInGameHUD::UpdateComboCount(int32 Value)
{
	if (HitComboWidget)
	{
		HitComboWidget->UpdateComboCount(Value);
	}
}

void AInGameHUD::ResetCombo()
{
	if (HitComboWidget)
	{
		HitComboWidget->ResetCombo();
	}
}

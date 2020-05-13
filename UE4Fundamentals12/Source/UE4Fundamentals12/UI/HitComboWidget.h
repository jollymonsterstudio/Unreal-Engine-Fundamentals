// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"

#include "HitComboWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE4FUNDAMENTALS12_API UHitComboWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UHitComboWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TXTCombo;

	void UpdateComboCount(int32 Value);

	void ResetCombo();

	void StoreWidgetAnimations();

	UWidgetAnimation* GetAnimationByName(FName AnimationName) const;

private:
	TMap<FName, UWidgetAnimation*> AnimationsMap;

	UWidgetAnimation* ComboFadeAnimation;
	UWidgetAnimation* ComboShakeAnimation;
};

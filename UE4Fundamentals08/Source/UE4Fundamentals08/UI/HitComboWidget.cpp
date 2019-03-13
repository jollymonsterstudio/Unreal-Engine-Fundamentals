// Fill out your copyright notice in the Description page of Project Settings.

#include "HitComboWidget.h"

#include "Engine.h"

UHitComboWidget::UHitComboWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UHitComboWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StoreWidgetAnimations();

	ComboFadeAnimation = GetAnimationByName(TEXT("ComboFade"));
	ComboShakeAnimation = GetAnimationByName(TEXT("ComboShake"));
}

void UHitComboWidget::UpdateComboCount(int32 Value)
{
	// only update if more than one hit
	if (TXTCombo && Value > 1)
	{
		if (TXTCombo->Visibility == ESlateVisibility::Hidden)
		{
			TXTCombo->SetVisibility(ESlateVisibility::Visible);
		}
		TXTCombo->SetText(FText::FromString((FString::FromInt(Value) + "x Combo")));

		if (ComboShakeAnimation)
		{
			PlayAnimation(ComboShakeAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
		}

		if (ComboFadeAnimation)
		{
			PlayAnimation(ComboFadeAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
		}
	}
}

void UHitComboWidget::ResetCombo()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, __FUNCTION__);

	if (TXTCombo)
	{
		TXTCombo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHitComboWidget::StoreWidgetAnimations()
{
	AnimationsMap.Empty();

	UProperty* Prop = GetClass()->PropertyLink;

	// Run through all properties of this class to find any widget animations
	while (Prop != nullptr)
	{
		// Only interested in object properties
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			// Only want the properties that are widget animations
			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* WidgetAnimation = Cast<UWidgetAnimation>(Obj);

				if (WidgetAnimation && WidgetAnimation->MovieScene)
				{
					FName AnimName = WidgetAnimation->MovieScene->GetFName();
					GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Magenta, AnimName.ToString());
					AnimationsMap.Add(AnimName, WidgetAnimation);
				}
			}
		}

		Prop = Prop->PropertyLinkNext;
	}
}

UWidgetAnimation* UHitComboWidget::GetAnimationByName(FName AnimationName) const
{
	UWidgetAnimation* const* WidgetAnimation = AnimationsMap.Find(AnimationName);
	if (WidgetAnimation)
	{
		return *WidgetAnimation;
	}
	return nullptr;
}
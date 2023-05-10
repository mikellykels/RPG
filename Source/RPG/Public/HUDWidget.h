// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class RPG_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// ** HEALTH AND STAMINA ** //
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;
	
	// ** LEVEL AND XP ** //
	UPROPERTY(meta = (BindWidget))
	UProgressBar* XPBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* XPText;

	// ** EQUIPPED ** //
	UPROPERTY(meta = (BindWidget))
	UImage* EquippedImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EquippedName;
};

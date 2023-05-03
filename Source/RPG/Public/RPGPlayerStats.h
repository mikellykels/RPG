// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundCue.h"
#include "EnemyHealthBarWidget.h"
#include "RPGPlayerStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API URPGPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGPlayerStats();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* LevelUpSound = nullptr;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ** HEALTH ** //
	UPROPERTY(EditAnywhere, Category = "Health")
	float CurrentHealth = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void DecreaseHealth(float Damage);
	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseHealth();
	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseMaxHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealth();
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth();

	// ** STAMINA ** //
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float DepletedStamina = 0.1f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float AddedStamina = 0.2f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float CurrentStamina = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 100.0f;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void DecreaseStamina();
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void IncreaseStamina();
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void IncreaseMaxStamina();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float GetCurrentStamina();
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float GetMaxStamina();

	// ** LEVEL AND XP ** //
	UPROPERTY(EditAnywhere, Category = "Level and XP")
	float AddedXP = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Level and XP")
	int AddedLevel = 1;
	UPROPERTY(EditAnywhere, Category = "Level and XP")
	float CurrentXP = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Level and XP")
	int CurrentLevel = 1;
	UPROPERTY(EditAnywhere, Category = "Level and XP")
	float XP = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Level and XP")
	float MaxXP = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Level and XP")
	int Level = 1;

	UFUNCTION(BlueprintCallable, Category = "Level and XP")
	void IncreaseXP();
	UFUNCTION(BlueprintCallable, Category = "Level and XP")
	void IncreaseLevel();

	UFUNCTION(BlueprintCallable, Category = "Level and XP")
	float GetCurrentXP();
	UFUNCTION(BlueprintCallable, Category = "Level and XP")
	float GetMaxXP();
	UFUNCTION(BlueprintCallable, Category = "Level and XP")
	int GetCurrentLevel();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaDepleted = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaAdded = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float CurrentHealth = 70.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float CurrentStamina = 70.0f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, Category = "XP")
	float XP = 0.0f;

	UPROPERTY(EditAnywhere, Category = "XP")
	float MaxXP = 100.0f;

	UPROPERTY(EditAnywhere, Category = "XP")
	int Level = 1;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float GetCurrentStamina();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float GetMaxStamina();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void DecreaseHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseMaxHealth();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void DecreaseStamina();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void IncreaseStamina();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void IncreaseMaxStamina();
};

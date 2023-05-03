// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyHealthBarWidget.h"
#include "RPGEnemyCharacterBase.generated.h"

class URPGPlayerStats;
class UWidgetComponent;

UCLASS()
class RPG_API ARPGEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGEnemyCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URPGPlayerStats* RPGPlayerStatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component")
	UWidgetComponent* HealthBarComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamage(ARPGEnemyCharacterBase* EnemyCharacterBase);

private:

	FTimerHandle DelayTimer;

	URPGPlayerStats* PlayerStatsCompRef = nullptr;
};

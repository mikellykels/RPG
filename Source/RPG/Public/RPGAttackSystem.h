// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "RPGCharacterBase.h"
#include "RPGAttackSystem.generated.h"

class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API URPGAttackSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGAttackSystem();

	void AxeAttack();
	void AxeAttackCombo();
	void AxeAttackStopCombo();

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* HitReactMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* HitReactAxeMontage = nullptr;

	bool PlayHitReactMontage(bool bIsAxeEquipped);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Attack();

	bool PlayAttackMontage(UAnimMontage* AttackMontage);

	bool bIsAttacking = false;
	bool bCanAttack = true;
	bool bSaveAttack = false;

	int AttackIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Hit")
	bool bEnemyHit = false;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackAMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackBMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackCMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* AxeHitSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* HitSparks;
	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* HitOil;

	void UnbindMontage();

	UFUNCTION()
	void OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<AActor*> ActorsToIgnore;

private:

	FTimerHandle DelayTimer;
	FTimerHandle AxeTraceLoop;

	ARPGCharacterBase* CharacterBase = nullptr;

	void AxeTrace();
};

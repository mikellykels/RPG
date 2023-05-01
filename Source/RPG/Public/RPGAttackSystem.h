// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "RPGCharacterBase.h"
#include "RPGAttackSystem.generated.h"


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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Attack();

	bool PlayAttackMontage(UAnimMontage* AttackMontage);

	bool bIsAttacking = false;
	bool bCanAttack = true;
	bool bSaveAttack = false;

	int AttackIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackAMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackBMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackCMontage = nullptr;

	void UnbindMontage();

	UFUNCTION()
	void OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	FTimerHandle DelayTimer;

	ARPGCharacterBase* CharacterBase;
};

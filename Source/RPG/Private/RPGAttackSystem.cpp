// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAttackSystem.h"
#include "RPGCharacterBase.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
URPGAttackSystem::URPGAttackSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URPGAttackSystem::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterBase = Cast<ARPGCharacterBase>(GetOwner());
}

// Called every frame
void URPGAttackSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URPGAttackSystem::AxeAttack()
{
	bIsAttacking = true;
	bCanAttack = true;
	AxeAttackCombo();
}

void URPGAttackSystem::AxeAttackCombo()
{
	if (bCanAttack)
	{
		switch (AttackIndex)
		{
		case 0:
			GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &URPGAttackSystem::Attack, 0.1f, false);
			break;
		case 1:
			GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &URPGAttackSystem::Attack, 0.1f, false);
			break;
		case 2:
			GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &URPGAttackSystem::Attack, 0.1f, false);
			break;
		default:
			break;
		}
	}
	else
	{
		AxeAttackStopCombo();
	}
}

void URPGAttackSystem::AxeAttackStopCombo()
{
	UnbindMontage();
	bIsAttacking = false;
	bCanAttack = false;
	AttackIndex = 0;
}

void URPGAttackSystem::Attack()
{
	switch (AttackIndex)
	{
	case 0:
		PlayAttackMontage(AttackAMontage);
		break;
	case 1:
		PlayAttackMontage(AttackBMontage);
		break;
	case 2:
		PlayAttackMontage(AttackCMontage);
		break;
	default:
		break;
	}
}

bool URPGAttackSystem::PlayAttackMontage(UAnimMontage* AttackMontage)
{
	const float PlayRate = 1.0f;
	bool bPlayedSuccessfully = CharacterBase->PlayAnimMontage(AttackMontage, PlayRate) > 0.0f;
	if (bPlayedSuccessfully)
	{
		AttackIndex++;
		UAnimInstance* AnimInstance = CharacterBase->GetMesh()->GetAnimInstance();

		if (AttackMontage == AttackAMontage)
		{
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &URPGAttackSystem::OnNotifyBeginRecieved);
		}
		return bPlayedSuccessfully;
	}
	return bPlayedSuccessfully;
}

void URPGAttackSystem::UnbindMontage()
{
	if (UAnimInstance* AnimInstance = CharacterBase->GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &URPGAttackSystem::OnNotifyBeginRecieved);
	}
}

void URPGAttackSystem::OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{;
	if (NotifyName == "ComboNext")
	{
		if (!bSaveAttack)
		{
			AxeAttackStopCombo();
		}
		else
		{
			AxeAttack();
		}
	}
	if (NotifyName == "ComboEnd")
	{
		AxeAttackStopCombo();
	}
}



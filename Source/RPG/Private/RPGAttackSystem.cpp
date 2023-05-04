// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAttackSystem.h"
#include "RPGAxe.h"
#include "RPGCharacterBase.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
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

}

void URPGAttackSystem::AxeTrace()
{
	TArray<AActor*> AttachedActors;
	CharacterBase->GetAttachedActors(AttachedActors);

	if (AttachedActors.Num() > 0)
	{
		AActor* AttachedAxe = AttachedActors[0];
		ARPGAxe* Axe = Cast<ARPGAxe>(AttachedAxe);
		FVector AxeLeftPointLoc = Axe->AxeLeftPoint->GetComponentLocation();
		FVector AxeRightPointLoc = Axe->AxeRightPoint->GetComponentLocation();
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());

		FHitResult Hit;

		bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), AxeLeftPointLoc, AxeRightPointLoc, 15.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Red, FLinearColor::Green, 0.1f);
		if (bHit)
		{
			//DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
			AActor* EnemyHit = Hit.GetActor();

			if (EnemyHit == nullptr)
			{
				return;
			}
			if (EnemyHit->ActorHasTag("Damageable"))
			{
				TSubclassOf <UDamageType> DamageType;
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("HITHIT!"));
				EnemyHit->TakeDamage(25.0f, FDamageEvent(), CharacterBase->GetController(), Axe);
				bEnemyHit = true;
			}
			if (bEnemyHit)
			{
				UGameplayStatics::PlaySoundAtLocation(this, AxeHitSound, AxeLeftPointLoc);
				if (HitSparks)
				{
					float CoefStrength = 5.0f;
					UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(HitSparks, Axe->AxeLeftPoint, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
					NiagaraComp->SetNiagaraVariableFloat(FString("StrengthCoef"), CoefStrength);
				}
				if (HitOil)
				{
					float CoefStrength = 5.0f;
					UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(HitOil, Axe->AxeLeftPoint, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
					NiagaraComp->SetNiagaraVariableFloat(FString("StrengthCoef"), CoefStrength);
				}
			}
		}
		bEnemyHit = false;
	}
}

void URPGAttackSystem::AxeAttack()
{
	bIsAttacking = true;
	bCanAttack = true;

	GetWorld()->GetTimerManager().SetTimer(AxeTraceLoop, this, &URPGAttackSystem::AxeTrace, 0.1f, true);
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
	GetWorld()->GetTimerManager().ClearTimer(AxeTraceLoop);
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



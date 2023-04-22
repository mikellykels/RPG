// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ARPGCharacterBase::ARPGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TraceDistance = 2000;
}

// Called when the game starts or when spawned
void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	MoveCompRef = GetCharacterMovement();
}

void ARPGCharacterBase::InteractPressed()
{
	TraceForward();
	if (FocusedActor)
	{
		IInteractionInterface* Interface = Cast<IInteractionInterface>(FocusedActor);
		Axe = Cast<ARPGAxe>(FocusedActor);
		if (Interface)
		{
			if (Axe)
			{
				CharacterWeaponEquipped = ECharacterWeaponEquipped::GreatAxe;
				Interface->Execute_OnInteract(FocusedActor, this);
				GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel1);
				GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
			}
		}
	}
}

void ARPGCharacterBase::RequestLightAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Light Attack"));
	if (CharacterWeaponEquipped == ECharacterWeaponEquipped::GreatAxe)
	{
		if (PlayAttackMontage())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Played Montage"));
		}
	}
}

void ARPGCharacterBase::TraceStartFocus(AActor* TraceActor)
{
	IInteractionInterface* Interface = Cast<IInteractionInterface>(TraceActor);
	if (Interface)
	{
		Interface->Execute_StartFocus(TraceActor);
	}
}

void ARPGCharacterBase::TraceEndFocus(AActor* TraceActor)
{
	IInteractionInterface* Interface = Cast<IInteractionInterface>(TraceActor);
	if (Interface)
	{
		Interface->Execute_EndFocus(TraceActor);
	}
}

void ARPGCharacterBase::TraceForward_Implementation()
{
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(Loc, Rot);

	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * TraceDistance);

	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f);

	if (bHit)
	{
		//DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);

		AActor* Interactable = Hit.GetActor();

		if (Interactable)
		{
			if (Interactable != FocusedActor)
			{
				if (FocusedActor)
				{
					TraceEndFocus(FocusedActor);
				}
				TraceStartFocus(Interactable);
				FocusedActor = Interactable;
			}
		}
		else
		{
			if (FocusedActor)
			{
				TraceEndFocus(FocusedActor);
			}
			FocusedActor = nullptr;
		}
	}
	else
	{
		TraceEndFocus(FocusedActor);
		FocusedActor = nullptr;
	}
}

bool ARPGCharacterBase::PlayAttackMontage()
{
	const float PlayRate = 1.0f;
	bool bPlayedSuccessfully = PlayAnimMontage(AttackMontage, PlayRate) > 0.0f;
	if (bPlayedSuccessfully)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		DisableWalk();

		if (!BlendingOutDelegate.IsBound())
		{
			BlendingOutDelegate.BindUObject(this, &ARPGCharacterBase::OnMontageBlendingOut);
		}
		AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, AttackMontage);

		if (!MontageEndedDelegate.IsBound())
		{
			MontageEndedDelegate.BindUObject(this, &ARPGCharacterBase::OnMontageEnded);
		}
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);

		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ARPGCharacterBase::OnNotifyBeginRecieved);
		AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &ARPGCharacterBase::OnNotifyEndRecieved);
		return bPlayedSuccessfully;
	}

	return bPlayedSuccessfully;
}

void ARPGCharacterBase::UnbindMontage()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ARPGCharacterBase::OnNotifyBeginRecieved);
		AnimInstance->OnPlayMontageNotifyEnd.RemoveDynamic(this, &ARPGCharacterBase::OnNotifyEndRecieved);
	}
}

void ARPGCharacterBase::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
}

void ARPGCharacterBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	EnableWalk();
	UnbindMontage();
}

void ARPGCharacterBase::OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	// Set up attack hit point
}

void ARPGCharacterBase::OnNotifyEndRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
}

void ARPGCharacterBase::DisableWalk()
{
	if (MoveCompRef)
	{
		MoveCompRef->DisableMovement();
	}
	else
	{
		UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		MoveComp->DisableMovement();
	}
}

void ARPGCharacterBase::EnableWalk()
{
	if (MoveCompRef)
	{
		MoveCompRef->SetMovementMode(MOVE_Walking);
	}
	else
	{
		UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		MoveComp->SetMovementMode(MOVE_Walking);
	}
}

// Called every frame
void ARPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForward();

}

// Called to bind functionality to input
void ARPGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


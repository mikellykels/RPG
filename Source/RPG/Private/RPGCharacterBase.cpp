// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPGAttackSystem.h"
#include "RPGEquipmentSystem.h"
#include "RPGPlayerController.h"
#include "RPGPlayerStats.h"
#include "DrawDebugHelpers.h"

#define LOCTEXT_NAMESPACE "HUD"

// Sets default values
ARPGCharacterBase::ARPGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	RPGPlayerStatsComponent = CreateDefaultSubobject<URPGPlayerStats>(TEXT("Player Stats Comp"));
	RPGAttackSystemComponent = CreateDefaultSubobject<URPGAttackSystem>(TEXT("Attack System Comp"));
	RPGEquipmentSystemComponent = CreateDefaultSubobject<URPGEquipmentSystem>(TEXT("Equipment System Comp"));

	TraceDistance = 2000;
}

// Called when the game starts or when spawned
void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	MoveCompRef = GetCharacterMovement();
	PlayerStatsCompRef = Cast<URPGPlayerStats>(RPGPlayerStatsComponent);

	if (GetController()->GetCharacter())
	{
		DefaultWalkSpeed = GetController()->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed;
	}

	DisplayHUDWidget();
}

float ARPGCharacterBase::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	PlayerStatsCompRef->DecreaseHealth(DamageAmount);

	float Damage = GetOwner()->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UpdateHealthBar();

	return Damage;
}

// Called every frame
void ARPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForward();
	UpdateStamina();

	// TODO: See if can move out of tick
	UpdateXPBar();
	UpdateXPText();
	UpdateLevelText();
}

void ARPGCharacterBase::DisplayHUDWidget()
{
	if (IsValid(HUDWidgetClass))
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		HUDWidget = CreateWidget<UHUDWidget>(PlayerController, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}

void ARPGCharacterBase::UpdateStamina()
{
	if (HUDWidget)
	{
		if (bIsSprinting)
		{
			RPGPlayerStatsComponent->DecreaseStamina();
			float CurrentStamina = RPGPlayerStatsComponent->GetCurrentStamina();
			
			if (CurrentStamina <= 0)
			{
				RequestSprintStop();
			}
			UpdateStaminaBar();
		}
		else
		{
			// TODO: add delay for stamina increasing after stop sprinting
			RPGPlayerStatsComponent->IncreaseStamina();
			UpdateStaminaBar();
		}
	}
}

void ARPGCharacterBase::UpdateXPText()
{
	if (HUDWidget)
	{
		if (PlayerStatsCompRef)
		{
			float CurrentXP = RPGPlayerStatsComponent->GetCurrentXP();
			float MaxXP = RPGPlayerStatsComponent->GetMaxXP();
			FText FormattedText = FText::Format(LOCTEXT("XPText", "{0} / {1}"), CurrentXP, MaxXP);
			HUDWidget->XPText->SetText(FormattedText);
			UpdateXPBar();
		}
	}
}

void ARPGCharacterBase::UpdateLevelText()
{
	if (HUDWidget)
	{
		if (PlayerStatsCompRef)
		{
			int CurrentLevel = RPGPlayerStatsComponent->GetCurrentLevel();
			HUDWidget->LevelText->SetText(FText::AsNumber(CurrentLevel));
		}
	}
}

void ARPGCharacterBase::UpdateHealthBar()
{
	if (HUDWidget)
	{
		float CurrentHealth = RPGPlayerStatsComponent->GetCurrentHealth();
		float MaxHealth = RPGPlayerStatsComponent->GetMaxHealth();
		
		if (CurrentHealth <= 0)
		{
			PlayDeathMontage();
		}

		if (PlayerStatsCompRef)
		{
			FString CurrentHealthStr = FString::SanitizeFloat(CurrentHealth, 0);
			HUDWidget->HealthBar->SetPercent(CurrentHealth / MaxHealth);
			HUDWidget->HealthText->SetText(FText::FromString(CurrentHealthStr));
		}
	}
}

void ARPGCharacterBase::UpdateStaminaBar()
{
	if (HUDWidget)
	{
		float CurrentStamina = RPGPlayerStatsComponent->GetCurrentStamina();
		float MaxStamina = RPGPlayerStatsComponent->GetMaxStamina();
		if (PlayerStatsCompRef && (CurrentStamina < MaxStamina))
		{
			HUDWidget->StaminaBar->SetPercent(CurrentStamina / MaxStamina);
		}
	}
}

void ARPGCharacterBase::UpdateXPBar()
{
	if (HUDWidget)
	{
		if (RPGPlayerStatsComponent)
		{
			float CurrentXP = RPGPlayerStatsComponent->GetCurrentXP();
			float MaxXP = RPGPlayerStatsComponent->GetMaxXP();
			if (PlayerStatsCompRef)
			{
				HUDWidget->XPBar->SetPercent(CurrentXP / MaxXP);
			}
		}
	}
}

void ARPGCharacterBase::RequestSprintStart()
{
	if (GetController()->GetCharacter())
	{
		bIsSprinting = true;
		GetController()->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ARPGCharacterBase::RequestSprintStop()
{
	if (GetController()->GetCharacter())
	{
		bIsSprinting = false;
		GetController()->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	}
}

void ARPGCharacterBase::InteractPressed()
{
	TraceForward();
	if (FocusedActor)
	{
		IInteractionInterface* Interface = Cast<IInteractionInterface>(FocusedActor);
		Axe = Cast<ARPGAxe>(FocusedActor);
		Weapon = Cast<AWeapon>(FocusedActor);
		if (Interface)
		{
			if (Axe)
			{
				if (HUDWidget)
				{
					FString AxeImagePath = FString("/Game/RPG/UI/Icons/SK_Axe_256x256.SK_Axe_256x256");
					UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *AxeImagePath));

					CharacterWeaponEquipped = ECharacterWeaponEquipped::GreatAxe;
					Interface->Execute_OnInteract(FocusedActor, this);
					GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel1);
					GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
					HUDWidget->EquippedImage->SetBrushFromTexture(Texture);
					HUDWidget->EquippedName->SetText(FText::FromString(Axe->WeaponData.Name));
					OnEquip(Axe);
				}
			}
			if (Weapon)
			{
				RPGEquipmentSystemComponent->AddWeaponToInventory(Weapon);
				Weapon->Destroy();
			}
		}
	}
}

void ARPGCharacterBase::RequestAttack()
{
	if (CharacterWeaponEquipped == ECharacterWeaponEquipped::GreatAxe)
	{
		RPGAttackSystemComponent->AxeAttack();
	}
}

void ARPGCharacterBase::RequestEquipmentMenu()
{
	RPGEquipmentSystemComponent->DisplayEquipmentMenu();
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

bool ARPGCharacterBase::PlayDeathMontage()
{
	const float PlayRate = 1.0f;
	bool bPlayedSuccessfully = PlayAnimMontage(DeathMontage, PlayRate) > 0.0f;
	if (bPlayedSuccessfully)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (!MontageEndedDelegate.IsBound())
		{
			MontageEndedDelegate.BindUObject(this, &ARPGCharacterBase::OnMontageEnded);
		}
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, DeathMontage);

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
}

void ARPGCharacterBase::OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	// TODO: Set up attack hit point

	if (NotifyName == FName("AttackAGrunt"))
	{
		if (AttackASound && GetOwner())
		{
			FVector CharacterLocation = GetOwner()->GetActorLocation();
			UGameplayStatics::PlaySoundAtLocation(this, AttackASound, CharacterLocation);
		}
	}

	if (NotifyName == FName("DeathStart"))
	{
		DisableWalk();
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->SetShowMouseCursor(true);
		//SoundCue Triggers
		if (DeathSound && GetOwner())
		{
			FVector CharacterLocation = GetOwner()->GetActorLocation();
			UGameplayStatics::PlaySoundAtLocation(this, DeathSound, CharacterLocation);
		}
		// TODO: Add menu on death
	}
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

#undef LOCTEXT_NAMESPACE


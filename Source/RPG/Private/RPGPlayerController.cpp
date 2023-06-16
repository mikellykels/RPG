// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPGCharacterBase.h"

ARPGPlayerController::ARPGPlayerController()
{
}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// ** Moving ** //
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::RequestMove);

		// ** Looking ** //
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::RequestLook);

		//** Sprinting **//
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::RequestSprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ARPGPlayerController::RequestSprintStop);

		//** Jumping **//
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::RequestJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ARPGPlayerController::RequestJumpStop);

		// ** INTERACT ** //
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::RequestInteract);

		// ** ATTACK ** //
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::RequestAttack);

		// ** INVENTORY MENU ** //
		EnhancedInputComponent->BindAction(InventoryMenuAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::RequestInventoryMenu);
	}
}

void ARPGPlayerController::RequestMove(const FInputActionValue& Value)
{
	ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(GetCharacter());

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (RPGCharacter != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = RPGCharacter->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		RPGCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		RPGCharacter->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARPGPlayerController::RequestLook(const FInputActionValue& Value)
{
	ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(GetCharacter());

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (RPGCharacter != nullptr)
	{
		// add yaw and pitch input to controller
		RPGCharacter->AddControllerYawInput(LookAxisVector.X);
		RPGCharacter->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARPGPlayerController::RequestJumpStart()
{
	if (GetCharacter())
	{
		GetCharacter()->Jump();
	}
}

void ARPGPlayerController::RequestJumpStop()
{
	if (GetCharacter())
	{
		GetCharacter()->StopJumping();
	}
}

void ARPGPlayerController::RequestSprintStart()
{
	if (ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(GetCharacter()))
	{
		RPGCharacter->RequestSprintStart();
	}
}

void ARPGPlayerController::RequestSprintStop()
{
	if (ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(GetCharacter()))
	{
		RPGCharacter->RequestSprintStop();
	}
}

void ARPGPlayerController::RequestInteract()
{
	if (ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(GetCharacter()))
	{
		RPGCharacter->InteractPressed();
	}
}

void ARPGPlayerController::RequestAttack()
{
	if (ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(GetCharacter()))
	{
		RPGCharacter->RequestAttack();
	}
}

void ARPGPlayerController::RequestInventoryMenu()
{
	if (ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(GetCharacter()))
	{
		RPGCharacter->RequestEquipmentMenu();
	}
}




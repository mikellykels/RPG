// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPGCharacterBase.h"

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		// ** MOVEMENT ** //
		InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ARPGPlayerController::RequestJumpStart);
		InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ARPGPlayerController::RequestJumpStop);

		InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ARPGPlayerController::RequestSprintStart);
		InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &ARPGPlayerController::RequestSprintStop);

		InputComponent->BindAxis(TEXT("MoveForward"), this, &ARPGPlayerController::RequestMoveForward);
		InputComponent->BindAxis(TEXT("MoveRight"), this, &ARPGPlayerController::RequestMoveRight);
		InputComponent->BindAxis(TEXT("LookUp"), this, &ARPGPlayerController::RequestLookUp);
		InputComponent->BindAxis(TEXT("LookRight"), this, &ARPGPlayerController::RequestLookRight);

		// ** INTERACT ** //
		InputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &ARPGPlayerController::InteractPressed);

		// ** ATTACKS ** //
		InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ARPGPlayerController::RequestAttack);

		// ** EQUIPMENT MENU ** //
		InputComponent->BindAction(TEXT("EquipmentMenu"), EInputEvent::IE_Pressed, this, &ARPGPlayerController::RequestEquipmentMenu);
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

void ARPGPlayerController::RequestMoveForward(float AxisValue)
{
	if (AxisValue != 0.f)
	{
		FRotator const ControlSpaceRot = GetControlRotation();

		GetPawn()->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), AxisValue);
	}
}

void ARPGPlayerController::RequestMoveRight(float AxisValue)
{
	if (AxisValue != 0.f)
	{
		FRotator const ControlSpaceRot = GetControlRotation();

		GetPawn()->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), AxisValue);
	}
}

void ARPGPlayerController::RequestLookUp(float AxisValue)
{
	AddPitchInput(AxisValue * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPGPlayerController::RequestLookRight(float AxisValue)
{
	AddYawInput(AxisValue * BaseLookRightRate * GetWorld()->GetDeltaSeconds());
}
void ARPGPlayerController::InteractPressed()
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

void ARPGPlayerController::RequestEquipmentMenu()
{
	if (ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(GetCharacter()))
	{
		RPGCharacter->RequestEquipmentMenu();
	}
}




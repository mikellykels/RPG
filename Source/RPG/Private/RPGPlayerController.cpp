// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPGCharacterBase.h"

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetCharacter())
	{
		DefaultWalkSpeed = GetCharacter()->GetCharacterMovement()->MaxWalkSpeed;
	}
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		// Movement
		InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ARPGPlayerController::RequestJumpStart);
		InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ARPGPlayerController::RequestJumpStop);

		InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ARPGPlayerController::RequestSprintStart);
		InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &ARPGPlayerController::RequestSprintStop);

		InputComponent->BindAxis(TEXT("MoveForward"), this, &ARPGPlayerController::RequestMoveForward);
		InputComponent->BindAxis(TEXT("MoveRight"), this, &ARPGPlayerController::RequestMoveRight);
		InputComponent->BindAxis(TEXT("LookUp"), this, &ARPGPlayerController::RequestLookUp);
		InputComponent->BindAxis(TEXT("LookRight"), this, &ARPGPlayerController::RequestLookRight);
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
	if (GetCharacter())
	{
		GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ARPGPlayerController::RequestSprintStop()
{
	if (GetCharacter())
	{
		GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
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



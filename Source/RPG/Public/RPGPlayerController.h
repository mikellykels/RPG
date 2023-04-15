// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

protected:

	void SetupInputComponent() override;

	void RequestJumpStart();
	void RequestJumpStop();

	void RequestSprintStart();
	void RequestSprintStop();

	void RequestMoveForward(float AxisValue);
	void RequestMoveRight(float AxisValue);
	void RequestLookUp(float AxisValue);
	void RequestLookRight(float AxisValue);

	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookUpRate = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookRightRate = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 1200.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DefaultWalkSpeed = 0.0f;
	
};

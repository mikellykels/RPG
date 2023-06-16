// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "RPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryMenuAction;

public:
	
	ARPGPlayerController();
	virtual void BeginPlay() override;

protected:

	void SetupInputComponent() override;

	/** Called for movement input */
	void RequestMove(const FInputActionValue& Value);

	/** Called for looking input */
	void RequestLook(const FInputActionValue& Value);

	/** Called for sprint input */
	void RequestSprintStart();
	void RequestSprintStop();

	/** Called for jump input **/
	void RequestJumpStart();
	void RequestJumpStop();

	/** Called for interact input **/
	void RequestInteract();

	/** Called for attack input **/
	void RequestAttack();

	/** Called for inventory menu input **/
	void RequestInventoryMenu();

	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookUpRate = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookRightRate = 90.0f;
	
};

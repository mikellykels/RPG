// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractionInterface.h"
#include "HUDWidget.h"
#include "RPGAxe.h"
#include "RPGCharacterBase.generated.h"

class URPGPlayerStats;

UENUM(BlueprintType)
enum class ECharacterWeaponEquipped : uint8
{
	None             UMETA(DisplayName = "None"),
	GreatAxe         UMETA(DisplayName = "GreatAxe"),
};

UCLASS()
class RPG_API ARPGCharacterBase : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DisplayHUDWidget();
	void UpdateStamina();
	void UpdateXPText();
	void UpdateLevelText();

	// TODO: Clean this up once implemented in code
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar();

	// TODO: Clean this up once implemented in code
	UFUNCTION(BlueprintCallable)
	void UpdateStaminaBar();

	// TODO: Clean this up once implemented in code
	UFUNCTION(BlueprintCallable)
	void UpdateXPBar();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction");
	float TraceDistance;

	UFUNCTION(BlueprintNativeEvent)
	void TraceForward();
	void TraceForward_Implementation();

	UPROPERTY(VisibleAnywhere, Category = "Weapon Equipped")
	ECharacterWeaponEquipped CharacterWeaponEquipped = ECharacterWeaponEquipped::None;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DeathMontage = nullptr;

	bool PlayAttackMontage();
	bool PlayDeathMontage();

	void UnbindMontage();

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void OnNotifyEndRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;

	UFUNCTION()
	void DisableWalk();

	UFUNCTION()
	void EnableWalk();

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 500.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DefaultWalkSpeed = 0.0f;

	bool bIsSprinting = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void RequestSprintStart();
	void RequestSprintStop();

	void InteractPressed();

	void RequestLightAttack();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URPGPlayerStats* RPGPlayerStatsComponent;

private:

	AActor* FocusedActor;

	ARPGAxe* Axe;

	void TraceStartFocus(AActor* TraceActor);
	void TraceEndFocus(AActor* TraceActor);

	FTimerHandle MovementTimer;

	UCharacterMovementComponent* MoveCompRef = nullptr;

	URPGPlayerStats* PlayerStatsCompRef = nullptr;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass = nullptr;

	UPROPERTY()
	UHUDWidget* HUDWidget;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractionInterface.h"
#include "RPGAxe.h"
#include "RPGCharacterBase.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction");
	float TraceDistance;

	UFUNCTION(BlueprintNativeEvent)
	void TraceForward();
	void TraceForward_Implementation();

	UPROPERTY(VisibleAnywhere, Category = "Weapon Equipped")
	ECharacterWeaponEquipped CharacterWeaponEquipped = ECharacterWeaponEquipped::None;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackMontage = nullptr;

	bool PlayAttackMontage();

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
	void EnableWalk();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InteractPressed();

	void RequestLightAttack();

private:

	AActor* FocusedActor;

	ARPGAxe* Axe;

	void TraceStartFocus(AActor* TraceActor);
	void TraceEndFocus(AActor* TraceActor);

	FTimerHandle MovementTimer;

	UCharacterMovementComponent* MoveCompRef = nullptr;
};

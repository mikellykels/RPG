// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractionInterface.h"
#include "HUDWidget.h"
#include "RPGAxe.h"
#include "Sound/SoundCue.h"
#include "Weapon.h"
#include "RPGCharacterBase.generated.h"

class URPGPlayerStats;
class URPGAttackSystem;
class URPGEquipmentSystem;
class USpringArmComponent;
class UCameraComponent;

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

	//** Camera boom positioning the camera behind the character *//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = true))
	USpringArmComponent* CameraBoom;

	//** Follow camera *//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = true))
	UCameraComponent* FollowCamera;

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

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* DeathSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* AttackASound = nullptr;

	UFUNCTION()
	void DisableWalk();

	UFUNCTION()
	void EnableWalk();

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 500.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DefaultWalkSpeed = 0.0f;

	virtual float TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	bool bIsSprinting = false;

public:	
	virtual void Tick(float DeltaTime) override;

	void RequestSprintStart();
	void RequestSprintStop();

	void InteractPressed();
	void RequestAttack();
	void RequestEquipmentMenu();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URPGPlayerStats* RPGPlayerStatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URPGAttackSystem* RPGAttackSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URPGEquipmentSystem* RPGEquipmentSystemComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEquip(AActor* EquippedWeapon);

private:

	AActor* FocusedActor;

	ARPGAxe* Axe;
	AWeapon* Weapon;

	void TraceStartFocus(AActor* TraceActor);
	void TraceEndFocus(AActor* TraceActor);

	UCharacterMovementComponent* MoveCompRef = nullptr;

	URPGPlayerStats* PlayerStatsCompRef = nullptr;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass = nullptr;

	UPROPERTY()
	UHUDWidget* HUDWidget;
};

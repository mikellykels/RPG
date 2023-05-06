// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "EquipmentWidget.h"
#include "RPGEquipmentSystem.generated.h"

class AWeapon;
class ARPGCharacterBase;

USTRUCT(BlueprintType)
struct FSlots : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle Item;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API URPGEquipmentSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGEquipmentSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	TArray<FSlots> WeaponSlots;
	AWeapon* Weapon;

	bool IsEquipmentMenuOpen = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddWeaponToInventory(AActor* InWeapon);

	void DisplayEquipmentMenu();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEquipmentWidget* EquipmentWidgetRef;

private:

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> EquipmentWidgetClass = nullptr;

	ARPGCharacterBase* CharacterBase;

	UEquipmentWidget* EquipmentWidget = nullptr;
};

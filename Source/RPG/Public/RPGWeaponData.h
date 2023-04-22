// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "RPGWeaponData.generated.h"

UENUM(BlueprintType)
enum class EStance : uint8
{
	Unequip       UMETA(DisplayName = "Unequip"),
	GreatAxe      UMETA(DisplayName = "GreatAxe"),
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString WeaponDescription = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStance Stance;
};

UCLASS()
class RPG_API ARPGWeaponData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGWeaponData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	class UDataTable* WeaponDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FName SelectedWeaponData = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FString Name = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FString WeaponDescription = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	EStance Stance = EStance::Unequip;
};

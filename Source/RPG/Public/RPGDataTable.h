// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "RPGDataTable.generated.h"

UENUM(BlueprintType)
	enum class EEffectType : uint8
{
	None          UMETA(DisplayName = "None"),
	Speed         UMETA(DisplayName = "SpeedBuff"),
	Jump          UMETA(DisplayName = "JumpBuff"),
	Power         UMETA(DisplayName = "PowerBuff"),
	Invinsibility UMETA(DisplayName = "InvBuff"),
};

USTRUCT(BlueprintType)
struct FEffectStats : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EEffectType EffectType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float EffectStrength = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString EffectDescription = "";
};

UCLASS()
class RPG_API ARPGDataTable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARPGDataTable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
		class UDataTable* EffectsTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		EEffectType SelectedEffectBP = EEffectType::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FName SelectedEffect = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		EEffectType EffectType = EEffectType::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		float EffectStrength = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FString Description = "";
};


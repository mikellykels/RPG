// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "RPGWeaponData.h"
#include "RPGAxe.generated.h"

class UStaticMeshComponent;

UCLASS()
class RPG_API ARPGAxe : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGAxe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* InteractionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	UWidgetComponent* InteractionWidget;

	enum class EState
	{
		Idle,
		Equipped,
		Dropped
	};

	UPROPERTY()
	AActor* InteractActor = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor* Caller);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StartFocus();
	virtual void StartFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EndFocus();
	virtual void EndFocus_Implementation();

	UPROPERTY(EditAnywhere)
	USceneComponent* _RootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh");
	UStaticMeshComponent* AxeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Box")
	UArrowComponent* AxeLeftPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Box")
	UArrowComponent* AxeRightPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponData WeaponData;

private:
	EState State = EState::Idle;

	bool bHasBeenPickedUp = false;

};

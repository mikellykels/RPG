// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "RPGEquipmentSystem.h"
#include "Weapon.generated.h"

UCLASS()
class RPG_API AWeapon : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* InteractionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	UWidgetComponent* InteractionWidget;

	UPROPERTY()
	AActor* InteractActor = nullptr;
	
	enum class EState
	{
		Idle,
		Equipped,
		Dropped
	};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlots ItemData;

private:

	UPROPERTY(EditAnywhere)
	USceneComponent* _RootComponent;

	EState State = EState::Idle;

	bool bHasBeenPickedUp = false;
};

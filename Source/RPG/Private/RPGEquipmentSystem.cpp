// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEquipmentSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPGCharacterBase.h"
#include "Weapon.h"

// Sets default values for this component's properties
URPGEquipmentSystem::URPGEquipmentSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URPGEquipmentSystem::BeginPlay()
{
	Super::BeginPlay();

	EquipmentWidgetRef = Cast<UEquipmentWidget>(EquipmentWidget);
	// ...
	
}


// Called every frame
void URPGEquipmentSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URPGEquipmentSystem::AddWeaponToInventory(AActor* InWeapon)
{
	Weapon = Cast<AWeapon>(InWeapon);
	WeaponSlots.Add(Weapon->ItemData);
	FString Name = Weapon->ItemData.Item.RowName.ToString();

	for (FSlots WeaponSlot : WeaponSlots)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Name);
	}
}

void URPGEquipmentSystem::DisplayEquipmentMenu()
{
	if (IsValid(EquipmentWidgetClass))
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!IsEquipmentMenuOpen)
		{
			EquipmentWidgetRef = CreateWidget<UEquipmentWidget>(PlayerController, EquipmentWidgetClass);
			IsEquipmentMenuOpen = true;

			if (EquipmentWidgetRef)
			{
				if (EquipmentWidgetRef->IsInViewport())
				{
					EquipmentWidgetRef->SetVisibility(ESlateVisibility::Visible);
				}
				EquipmentWidgetRef->AddToViewport();
				PlayerController->SetInputMode(FInputModeGameAndUI());
				PlayerController->SetShowMouseCursor(true);

				CharacterBase = Cast<ARPGCharacterBase>(GetOwner());
				UCharacterMovementComponent* CharacterMovement = CharacterBase->GetCharacterMovement();
				CharacterMovement->DisableMovement();
			}
		}
		else
		{
			if (EquipmentWidgetRef)
			{
				IsEquipmentMenuOpen = false;
				EquipmentWidgetRef->SetVisibility(ESlateVisibility::Hidden);
				PlayerController->SetInputMode(FInputModeGameOnly());
				PlayerController->SetShowMouseCursor(false);
				
				CharacterBase = Cast<ARPGCharacterBase>(GetOwner());
				UCharacterMovementComponent* CharacterMovement = CharacterBase->GetCharacterMovement();
				CharacterMovement->SetMovementMode(MOVE_Walking);
			}
		}
	}
}


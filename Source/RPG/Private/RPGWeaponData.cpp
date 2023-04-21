// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGWeaponData.h"

// Sets default values
ARPGWeaponData::ARPGWeaponData()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARPGWeaponData::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponDataTable)
	{
		static const FString ContextString(TEXT("Weapon Context"));
		FWeaponData* WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(SelectedWeaponData), ContextString, true);

		Name = WeaponData->Name;
		WeaponDescription = WeaponData->WeaponDescription;
		Stance = WeaponData->Stance;
	}
}

// Called every frame
void ARPGWeaponData::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


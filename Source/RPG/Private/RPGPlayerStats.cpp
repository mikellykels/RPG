// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerStats.h"

// Sets default values for this component's properties
URPGPlayerStats::URPGPlayerStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URPGPlayerStats::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void URPGPlayerStats::DecreaseHealth()
{
	if (CurrentHealth > 0)
	{
		CurrentHealth = CurrentHealth - Damage;
	}
}

void URPGPlayerStats::IncreaseHealth()
{
}

void URPGPlayerStats::IncreaseMaxHealth()
{
}

float URPGPlayerStats::GetCurrentHealth()
{
	return CurrentHealth;
}

float URPGPlayerStats::GetMaxHealth()
{
	return MaxHealth;
}

float URPGPlayerStats::GetCurrentStamina()
{
	return CurrentStamina;
}

float URPGPlayerStats::GetMaxStamina()
{
	return MaxStamina;
}

void URPGPlayerStats::DecreaseStamina()
{
	if (CurrentStamina > 0)
	{
		CurrentStamina = CurrentStamina - StaminaDepleted;
	}
}

void URPGPlayerStats::IncreaseStamina()
{
	if (CurrentStamina <= MaxStamina)
	{
		CurrentStamina = CurrentStamina + StaminaAdded;
	}
}

void URPGPlayerStats::IncreaseMaxStamina()
{
}

// Called every frame
void URPGPlayerStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


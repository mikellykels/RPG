// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerStats.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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

// Called every frame
void URPGPlayerStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URPGPlayerStats::DecreaseHealth(float Damage)
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

float URPGPlayerStats::GetCurrentXP()
{
	return XP;
}

float URPGPlayerStats::GetMaxXP()
{
	return MaxXP;
}

int URPGPlayerStats::GetCurrentLevel()
{
	return Level;
}

void URPGPlayerStats::DecreaseStamina()
{
	if (CurrentStamina > 0)
	{
		CurrentStamina = CurrentStamina - DepletedStamina;
	}
}

void URPGPlayerStats::IncreaseStamina()
{
	if (CurrentStamina <= MaxStamina)
	{
		CurrentStamina = CurrentStamina + AddedStamina;
	}
}

void URPGPlayerStats::IncreaseMaxStamina()
{
}

void URPGPlayerStats::IncreaseXP()
{
	XP = XP + AddedXP;
	if (XP >= MaxXP)
	{
		IncreaseLevel();
		XP = 0;
	}
}

void URPGPlayerStats::IncreaseLevel()
{
	if (LevelUpSound)
	{
		UGameplayStatics::PlaySound2D(this, LevelUpSound);
	}
	Level = Level + 1;
	MaxXP = MaxXP + 150;
}



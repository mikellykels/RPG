// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGDataTable.h"

// Sets default values
ARPGDataTable::ARPGDataTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARPGDataTable::BeginPlay()
{
	Super::BeginPlay();
	
	if (EffectsTable)
	{
		static const FString ContextString(TEXT("Effect Context"));
		FEffectStats* EffectStats = EffectsTable->FindRow<FEffectStats>(FName(SelectedEffect), ContextString, true);

		EffectType = EffectStats->EffectType;
		EffectStrength = EffectStats->EffectStrength;
		Description = EffectStats->EffectDescription;
	}
}

// Called every frame
void ARPGDataTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


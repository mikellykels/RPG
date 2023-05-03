// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemyCharacterBase.h"
#include "Blueprint/WidgetTree.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPGPlayerStats.h"

// Sets default values
ARPGEnemyCharacterBase::ARPGEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RPGPlayerStatsComponent = CreateDefaultSubobject<URPGPlayerStats>(TEXT("Player Stats Comp"));

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar Comp"));
	HealthBarComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARPGEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerStatsCompRef = Cast<URPGPlayerStats>(RPGPlayerStatsComponent);

}

// Called every frame
void ARPGEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ARPGEnemyCharacterBase::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	RPGPlayerStatsComponent->DecreaseHealth(DamageAmount);

	float Damage = GetOwner()->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	FString DamageAmountStr = FString::SanitizeFloat(DamageAmount, 0);
	OnDamage(this);

	return Damage;
}





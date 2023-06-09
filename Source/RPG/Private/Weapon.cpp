// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "RPGCharacterBase.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = _RootComponent;
	SetRootComponent(RootComponent);

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Box"));
	InteractionBox->SetBoxExtent(FVector(50.0f, 50.0f, 100.0f));
	InteractionBox->SetupAttachment(RootComponent);

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget"));
	InteractionWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	InteractionWidget->SetVisibility(false);
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnInteract_Implementation(AActor* Caller)
{
	State = EState::Equipped;
;
	if (ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(Caller))
	{
		AttachToComponent(RPGCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hand_r_2H_Weapon"));
		SetOwner(RPGCharacter);
		Execute_EndFocus(this);
		InteractionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::StartFocus_Implementation()
{
	if (State != EState::Equipped)
	{
		InteractionWidget->SetVisibility(true);
	}
}

void AWeapon::EndFocus_Implementation()
{
	InteractionWidget->SetVisibility(false);
}


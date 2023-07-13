// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAxe.h"
#include "RPGCharacterBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ARPGAxe::ARPGAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = _RootComponent;

	AxeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Axe Mesh"));
	AxeMesh->SetupAttachment(RootComponent);

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Box"));
	InteractionBox->SetBoxExtent(FVector(50.0f, 50.0f, 100.0f));
	InteractionBox->SetupAttachment(RootComponent);

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget"));
	InteractionWidget->SetupAttachment(RootComponent);

	AxeLeftPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Axe Left Point"));
	AxeLeftPoint->SetupAttachment(AxeMesh);
	AxeRightPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Axe Right Point"));
	AxeRightPoint->SetupAttachment(AxeMesh);
}

// Called when the game starts or when spawned
void ARPGAxe::BeginPlay()
{
	Super::BeginPlay();
	InteractionWidget->SetVisibility(false);
}

// Called every frame
void ARPGAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARPGAxe::OnInteract_Implementation(AActor* Caller)
{
	State = EState::Equipped;
	//Destroy();
	if (ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(Caller))
	{
		AttachToComponent(RPGCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hand_r_2H_Weapon"));
		SetOwner(RPGCharacter);
		Execute_EndFocus(this);
		InteractionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		InteractionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		AxeMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	}
}

void ARPGAxe::StartFocus_Implementation()
{
	if (State != EState::Equipped)
	{
		InteractionWidget->SetVisibility(true);
	}
}

void ARPGAxe::EndFocus_Implementation()
{
	InteractionWidget->SetVisibility(false);
}



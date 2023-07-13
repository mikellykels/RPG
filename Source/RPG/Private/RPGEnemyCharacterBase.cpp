// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemyCharacterBase.h"
#include "RPGAttackSystem.h"
#include "RPGAxe.h"
#include "RPGCharacterBase.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPGPlayerStats.h"

#define BToS(b) b ? L"true" : L"false"

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
	PlayerStatsCompRef->OnDeathDelegate.AddUniqueDynamic(this, &ARPGEnemyCharacterBase::OnDeath);
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

void ARPGEnemyCharacterBase::OnDeath()
{
	if (PlayDeathMontage())
	{
		GetCharacterMovement()->DisableMovement();
		// This will make the character ignore collisions with the enemy
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
		// This will make the enemy ignore collisions with the character
		GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

		if (CharacterBase != nullptr)
		{
			// Retrieve RPGAttackSystem component from CharacterBase
			URPGAttackSystem* AttackSystem = CharacterBase->FindComponentByClass<URPGAttackSystem>();
			if (AttackSystem)
			{
				AttackSystem->ActorsToIgnore.Add(this);
			}
		}
		GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ARPGEnemyCharacterBase::DestroyEnemy, 5.0f, false);
	}
}

void ARPGEnemyCharacterBase::DestroyEnemy()
{
	Destroy();
}

float ARPGEnemyCharacterBase::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// Check if the DamageCauser is an instance of RPGAxe
	if (ARPGAxe* Causer = Cast<ARPGAxe>(DamageCauser))
	{
		// Check if the owner of the RPGAxe is an instance of CharacterBase
		if (ARPGCharacterBase* Character = Cast<ARPGCharacterBase>(Causer->GetOwner()))
		{
			// Store a reference to it
			CharacterBase = Character;
		}
	}

	PlayerStatsCompRef->DecreaseHealth(DamageAmount);

	float Damage = GetOwner()->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	PlayHitReactMontage();
	OnDamage(this);

	return Damage;
}

bool ARPGEnemyCharacterBase::PlayDeathMontage()
{
	const float PlayRate = 1.0f;
	int32 Index = FMath::RandRange(0, DeathMontage.Num() - 1);
	UAnimMontage* TestMontage = DeathMontage[Index];
	bool bPlayedSuccessfully = PlayAnimMontage(TestMontage, PlayRate) > 0.0f;
	if (bPlayedSuccessfully)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (!MontageEndedDelegate.IsBound())
		{
			MontageEndedDelegate.BindUObject(this, &ARPGEnemyCharacterBase::OnMontageEnded);
		}
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, TestMontage);

		AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ARPGEnemyCharacterBase::OnNotifyBeginRecieved);
		return bPlayedSuccessfully;
	}

	return bPlayedSuccessfully;
}

bool ARPGEnemyCharacterBase::PlayHitReactMontage()
{
	const float PlayRate = 1.0f;
	int32 Index = FMath::RandRange(0, HitReactMontage.Num() - 1);
	UAnimMontage* TestMontage = HitReactMontage[Index];
	bool bPlayedSuccessfully = PlayAnimMontage(TestMontage, PlayRate) > 0.0f;
	if (bPlayedSuccessfully)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (!MontageEndedDelegate.IsBound())
		{
			MontageEndedDelegate.BindUObject(this, &ARPGEnemyCharacterBase::OnMontageEnded);
		}
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, TestMontage);

		AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ARPGEnemyCharacterBase::OnNotifyBeginRecieved);
		return bPlayedSuccessfully;
	}

	return bPlayedSuccessfully;
}

void ARPGEnemyCharacterBase::UnbindMontage()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ARPGEnemyCharacterBase::OnNotifyBeginRecieved);
	}
}

void ARPGEnemyCharacterBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}

void ARPGEnemyCharacterBase::OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName("DeathStart"))
	{
		//SoundCue Triggers
		if (DeathSound && GetOwner())
		{
			FVector CharacterLocation = GetOwner()->GetActorLocation();
			UGameplayStatics::PlaySoundAtLocation(this, DeathSound, CharacterLocation);
		}
	}
}

void ARPGEnemyCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	PlayerStatsCompRef->OnDeathDelegate.RemoveDynamic(this, &ARPGEnemyCharacterBase::OnDeath);

	Super::EndPlay(EndPlayReason);
}





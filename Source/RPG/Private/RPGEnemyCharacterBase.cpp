// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemyCharacterBase.h"
#include "Blueprint/WidgetTree.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
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

	PlayerStatsCompRef->OnDeathDelegate.AddDynamic(this, &ARPGEnemyCharacterBase::OnDeath);
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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("played montage!"));
		PlayerStatsCompRef->OnDeathDelegate.RemoveDynamic(this, &ARPGEnemyCharacterBase::OnDeath);
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

	RPGPlayerStatsComponent->DecreaseHealth(DamageAmount);

	float Damage = GetOwner()->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamage(this);

	return Damage;
}

bool ARPGEnemyCharacterBase::PlayDeathMontage()
{
	const float PlayRate = 1.0f;
	int32 Index = FMath::RandRange(0, DeathMontage.Num() - 1);
	bool bPlayedSuccessfully = PlayAnimMontage(DeathMontage[Index], PlayRate) > 0.0f;
	if (bPlayedSuccessfully)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (!MontageEndedDelegate.IsBound())
		{
			MontageEndedDelegate.BindUObject(this, &ARPGEnemyCharacterBase::OnMontageEnded);
		}
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, DeathMontage[Index]);

		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ARPGEnemyCharacterBase::OnNotifyBeginRecieved);
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





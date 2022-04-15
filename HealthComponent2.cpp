// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent2.h"
#include "Kismet/GameplayStatics.h"
#include "Cow3rdPersonGameMode.h" 

// Sets default values for this component's properties
UHealthComponent2::UHealthComponent2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent2::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	//Call the multicast delegate function
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent2::DamageTaken);

	GameMode = Cast<ACow3rdPersonGameMode>(UGameplayStatics::GetGameMode(this));
}

// Called every frame
void UHealthComponent2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent2::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("damaged actor %s, instigator %s, actor causing damage %s"),
	//*DamagedActor->GetName(), *Instigator->GetName(), *DamageCauser->GetName());

	if (Damage <= 0.f) return;

	Health -= Damage;

	UE_LOG(LogTemp, Warning, TEXT("Enemy Hit!!!!! Remaining Life = %f"), Health);

	if (Health <= 0.f && GameMode)
	{
		GameMode->ActorDied(DamagedActor);
	}
}

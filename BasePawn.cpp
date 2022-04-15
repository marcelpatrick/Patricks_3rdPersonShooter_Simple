// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "Projectile.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Construct Components
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));

	//Attach Components
	BaseMesh->SetupAttachment(CapsuleComp);
	ProjectileSpawnPoint->SetupAttachment(BaseMesh);
}

void ABasePawn::HandleDestruction()
{
	UE_LOG(LogTemp, Warning, TEXT("Handle destruction called, Actor Died!!!!!!!!!! "));

	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("houve um problema com as DeathParticles"));
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySound2D(this, DeathSound);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("problema com DeathSound"));
	}
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePawn::Fire()
{		
	// Spawn projectile from the projectile spawn point
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	
	//UE_LOG(LogTemp, Warning, TEXT("cliquei em Fire"));
	//UE_LOG(LogTemp, Warning, TEXT("location: %s, rotation: %s"), *Location.ToString(), *Rotation.ToString());

	//Set the owner of this projectile to whomever is firing now so that we know who hit and who got hit
	Projectile->SetOwner(this);
}

void ABasePawn::RotateActor(FVector LookAtTarget)
{
	//This vector will be the distance between the target and the base mesh of the actor who is aimig at it
	FVector ToTarget = LookAtTarget - BaseMesh->GetComponentLocation();

	//Adjust the players rotation to look in the direction (yaw) that our vector is pointing to
	FRotator RotateToTarget = FRotator(0.f, ToTarget.Rotation().Yaw -90.f, 0.f);

	//Set actor rotation passing our rotator variable as a parameter
	BaseMesh->SetWorldRotation(RotateToTarget);
}



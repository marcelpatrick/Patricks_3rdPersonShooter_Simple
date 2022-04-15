// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h" 



// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->InitialSpeed = 3000.f;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail")); 
	TrailParticles->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (LaunchSound)
	{
		UGameplayStatics::PlaySound2D(this, LaunchSound);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Problema com LaunchSound"));
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuse, const FHitResult& Hit)
{

	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MyOwner deu um nullptr"));
		Destroy();
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();

	UClass* DamageTypeClass = UDamageType::StaticClass();

	
	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{

		//UE_LOG(LogTemp, Warning, TEXT("meu Projectile Hit alguma coisa"));

		UGameplayStatics::ApplyDamage(
			OtherActor, /*damaged actor*/
			Damage, /*damage amount*/
			MyOwnerInstigator, /*the controller of the actor who is causing the damage*/ 
			this, /*actor causing damage*/
			DamageTypeClass /*class of the type of damage this actor is causing*/
			);

		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problema com as HitParticles!!!!!!"));
		}

		if (HitSound)
		{
			UGameplayStatics::PlaySound2D(this, HitSound);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problema com HitSound"));
		}
	}

	Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

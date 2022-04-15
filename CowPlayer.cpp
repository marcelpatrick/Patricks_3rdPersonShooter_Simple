// Fill out your copyright notice in the Description page of Project Settings.


#include "CowPlayer.h"
#include "Components/InputComponent.h"

ACowPlayer::ACowPlayer()
{
    //Construct components
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));

    //Attach components
    SpringArm->SetupAttachment(RootComponent);
    Camera->SetupAttachment(SpringArm);
}

void ACowPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACowPlayer::HandleDestruction()
{
    Super::HandleDestruction();

    SetActorHiddenInGame(true);

    SetActorTickEnabled(false);

    bAlive = false;
}

void ACowPlayer::BeginPlay()
{
    Super::BeginPlay();

    CowPlayerController = Cast<APlayerController>(GetController());
}

// Called to bind functionality to input
void ACowPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("Move"), this, &ACowPlayer::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACowPlayer::Turn);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ACowPlayer::Fire);
}

void ACowPlayer::Move(float Value)
{
    FVector DeltaVector = FVector::ZeroVector;

    DeltaVector.X = Value * Speed;

    AddActorLocalOffset(DeltaVector, true);
}

void ACowPlayer::Turn(float Value)
{
    FRotator DeltaRotator = FRotator::ZeroRotator;

    DeltaRotator.Yaw = Value * TurnRate;

    AddActorLocalRotation(DeltaRotator, true);
}

void ACowPlayer::Fire()
{
    Super::Fire();
}
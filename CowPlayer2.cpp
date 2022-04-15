// Fill out your copyright notice in the Description page of Project Settings.


#include "CowPlayer2.h"
#include "Components/InputComponent.h"


ACowPlayer2::ACowPlayer2()
{
    //Construct components
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));

    //Attach components
    SpringArm->SetupAttachment(RootComponent);
    Camera->SetupAttachment(SpringArm);
}

// Called to bind functionality to input
void ACowPlayer2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("Move"), this, &ACowPlayer2::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACowPlayer2::Turn);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ACowPlayer2::Fire);
}

void ACowPlayer2::Move(float Value)
{
    FVector DeltaVector = FVector::ZeroVector;

    DeltaVector.X = Value * Speed;

    AddActorLocalOffset(DeltaVector, true);
}

void ACowPlayer2::Turn(float Value)
{
    FRotator DeltaRotator = FRotator::ZeroRotator;

    DeltaRotator.Yaw = Value * TurnRate;

    AddActorLocalRotation(DeltaRotator, true);
}

void ACowPlayer2::Fire()
{
    Super::Fire();
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "CowEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"


void ACowEnemy::BeginPlay()
{

    Super::BeginPlay();

    //Define the player pointer by Accessing the player through its pawn and store this result inside our player pointer
    CowPlayerPointer = Cast<ACowPlayer>(UGameplayStatics::GetPlayerPawn(
        this, /*world context*/ 
        0 /*player index, which player we are talking about*/ 
        ));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ACowEnemy::CheckFireCondition, FireRate, true);

}

void ACowEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (InFireRange())
    {
        RotateActor(CowPlayerPointer->GetActorLocation());
    }
}

void ACowEnemy::HandleDestruction()
{
    Super::HandleDestruction();

    Destroy();
}

bool ACowEnemy::InFireRange()
{
    if (CowPlayerPointer)
    {
        float Distance = FVector::Dist(GetActorLocation(), CowPlayerPointer->GetActorLocation());
        if (Distance <= FireRange)
        {
            return true;   
        }
    }
    return false;
}

void ACowEnemy::CheckFireCondition()
{
    if (CowPlayerPointer == nullptr)
    {
        return;
    }

    if (InFireRange() && CowPlayerPointer->bAlive)
    {
        Fire();
    }
}


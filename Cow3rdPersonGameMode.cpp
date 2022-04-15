// Fill out your copyright notice in the Description page of Project Settings.

#include "Cow3rdPersonGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "CowPlayer.h"
#include "CowEnemy.h"


void ACow3rdPersonGameMode::ActorDied(AActor* DeadActor)
{
    if (DeadActor == CowPlayer)
    {
        UE_LOG(LogTemp, Warning, TEXT("my player was killed"));

        CowPlayer->HandleDestruction();

        if (CowPlayer->GetCowPlayerController())
        {
            CowPlayer->DisableInput(CowPlayer->GetCowPlayerController());
        }
        
        GameOver(false);
    }
    else if (ACowEnemy* DeadEnemy = Cast<ACowEnemy>(DeadActor))
    {
        DeadEnemy->HandleDestruction();

        TargetCows = GetTargetCowsCount();

        UE_LOG(LogTemp, Warning, TEXT("numero de enemies = %i"), TargetCows);

        if (TargetCows == 0)
        {
            GameOver(true);
        }
    }
}

void ACow3rdPersonGameMode::BeginPlay()
{
    Super::BeginPlay();

    CowPlayer = Cast<ACowPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));

    //Cow3rdPersonPlayerController = Cast<ACow3rdPersonPlayerController>(UGameplayStatics::GetPlayerController(this, 0)); 

    GetTargetCowsCount();
}


int32 ACow3rdPersonGameMode::GetTargetCowsCount()
{
    TArray<AActor*> Enemies;

    UGameplayStatics::GetAllActorsOfClass(this, ACowEnemy::StaticClass(), Enemies);

    return Enemies.Num();
}
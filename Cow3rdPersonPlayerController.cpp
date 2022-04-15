// Fill out your copyright notice in the Description page of Project Settings.


#include "Cow3rdPersonPlayerController.h"
#include "GameFramework/Pawn.h"


void ACow3rdPersonPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    if (bPlayerEnabled)
    {
        GetPawn()->EnableInput(this);   
    }
    else
    {
        GetPawn()->DisableInput(this);
    }   
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Cow3rdPersonPlayerController.h"
#include "CowPlayer.h"
#include "Cow3rdPersonGameMode.generated.h"


UCLASS()
class COW_3RDPERSON_API ACow3rdPersonGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	public:

		void ActorDied(AActor* DeadActor);

	protected:

		virtual void BeginPlay() override;

		UFUNCTION(BlueprintImplementableEvent)
		void GameOver(bool bWonGame);

	private:

		int32 TargetCows = 0;

		int32 GetTargetCowsCount();

		ACowPlayer* CowPlayer;

		bool bWonGame;

		//ACow3rdPersonPlayerController* Cow3rdPersonPlayerController;
};

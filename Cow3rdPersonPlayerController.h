// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Cow3rdPersonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COW_3RDPERSON_API ACow3rdPersonPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	public:

		void SetPlayerEnabledState(bool bPlayerEnabled);

	private:

};

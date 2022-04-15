// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "CowPlayer.h"
#include "CowEnemy.generated.h"

/**
 * 
 */
UCLASS()
class COW_3RDPERSON_API ACowEnemy : public ABasePawn
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

protected:

	virtual void BeginPlay() override;

private:

	ACowPlayer* CowPlayerPointer;

	FTimerHandle FireRateTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Components")
	float FireRate = 2.f;

	UPROPERTY(EditAnywhere, Category = "Components")
	float FireRange = 1000.f;

	void CheckFireCondition();

	bool InFireRange();
	
};

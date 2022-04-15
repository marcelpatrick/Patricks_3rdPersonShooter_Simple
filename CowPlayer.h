// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CowPlayer.generated.h"

/**
 * 
 */
UCLASS()
class COW_3RDPERSON_API ACowPlayer : public ABasePawn
{
	GENERATED_BODY()

	public:

		ACowPlayer();

		virtual void Tick(float DeltaTime) override;

		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		bool bAlive = true;

		void HandleDestruction();

		//Makea a custom public getter for us to get the private variable CowPlayerController from inside other classes
		APlayerController* GetCowPlayerController() const
		{
			return CowPlayerController;
		}

	protected:

		virtual void BeginPlay() override;

	private:

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
		USpringArmComponent* SpringArm;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
		UCameraComponent* Camera;

		void Move(float Value);
		void Turn(float Value);
		void Fire();

		UPROPERTY(EditAnywhere, Category = "Components")
		float Speed = 100.f;
		
		UPROPERTY(EditAnywhere, Category = "Components")
		float TurnRate = 3.f;

		APlayerController* CowPlayerController; 
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Controller.h"
#include "Cow3rdPersonGameMode.h"
#include "HealthComponent2.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COW_3RDPERSON_API UHealthComponent2 : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent2();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;
	float Health = 0.f;
	
	//Callback damage function
	UFUNCTION()
	void DamageTaken(
		AActor* DamagedActor, 
		float Damage, 
		const UDamageType* DamageType, 
		AController* Instigator, 
		AActor* DamageCauser);

	class ACow3rdPersonGameMode* GameMode;
};

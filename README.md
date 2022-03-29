# Patricks_3rdPersonShooter_Simple

# Project Steps:
1. Create Components (Parent Classes: BasePawn)
2. Create Sub-Components (Child Classes: Tank, Tower, Projectile)
3. User Input (Moving the Actor, Firing)
4. Actions and Events (Hit Events, Health Component, Apply Damage)
5. Game Rules, Game Mode and Game Controller (Game Cycle: Start > Death > Winning, Loosing > End)
6. Special Effects (Sounds, Particles)


-> #include are not described here and must be added to each respective component when needed. Refer to the code documents.

# 1: Create Components

## 1.1: BasePawn C++ class
Create a new C++ class "BasePawn" inheriting from the Pawn class that will be the base class for both our player and our enemies.
On File click on Refresh Virtual Studio Code project.

## 1.2: Declare Component Variables:
In its header file initialize and expose the variables that will correspond to each of the components that we will use in the world.

```cpp
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USceneComponent* ProjectileSpawnPoint;
```

## 1.3: Construct Component Objects:
In the cpp file, Construct these components so that they are able to ve visible in our world

```cpp
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Construct Components
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));

	//Attach Components
	BaseMesh->SetupAttachment(CapsuleComp);
	ProjectileSpawnPoint->SetupAttachment(BaseMesh);
}
```

# 2: Create Sub-Components

Create the other components that will inherit from BasePawn parent component.

## 2.1: Player Class:

On Unreal Engine, right click on BasePawn c++ and select "create c++ class derived from this class", name it CowPlayer.
  
### 2.1.1: Declare Component Variables:

In CowPlayer.h, Declare a ACowPlayer() constructor function, the Camera and Sprint arm components that will be following our player around for our 3rs person point of view.

```cpp
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


public:

    //Constructor function
		ACowPlayer();

private:

	UPROPERTY(VisibleAnywhere, Category="Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UCameraComponent* Camera; 
```

### 2.1.2: Construct Component Objects:

In CowPlayer.cpp, Construct the Camera and Sprint arm components.
Attach the spring arm to the root component and the camera to the spring arm.

```cpp
ACowPlayer::ACowPlayer()
{
    //Construct components
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));

    //Attach components
    SpringArm->SetupAttachment(RootComponent);
    Camera->SetupAttachment(SpringArm);
}
```

### 2.1.3: Blueprints

On Unreal, select CowPlayer c++ and click on Create a new blueprint based on CowPlayer, name it "BP_PawnCowPlayer".
In BP_PawnCowPlayer, View Port > Details > Static Mesh: select which static mesh you are going to use to physically represent your player in the world.
Adjust the camera and the spring arm, camera and projectile spawn point position.
In BP_PawnCowPlayer, View Port > Details > change Auto Possess Player to Player 0.
In BP_PawnCowPlayer, View Port > Details > Rendering > disable player hidden in game from both BP_PawnCowPlayer(self) and BaseMesh.
Place the blueprint in the world and delete the default player start.

## 2.2: Enemy Class:

Create a new C++ class inheriting from BasePawn.cpp and call it CowEnemy.

Create a new BP derived from CowEnemy.cpp to be our enemy. Call it BP_PawnCowEnemy. Place it in the world

## 2.3: Projectile Class:

Create a projectile c++ class with Actor as parent.

### 2.3.1: Declare Component Variables.

In Projectile.h
```cpp
private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMeshComponent* ProjectileMesh;
```

### 2.3.2: Construct Component Objects.
```cpp
AProjectile::AProjectile()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;
}
```

### 2.3.3: Blueprint
Create a new BluePrint based on this projectile c++ class. Open it and select the projectile static mesh for this BP.


# 3: User Input


## 3.1: Create an axis mapping for movement and an action mapping for firing  

- Unreal > Edit > Project Settings > Input > Bindings > Axis Mapping / Action Mapping
- Click add and create one axis mapping function Move to move forwards and backwards: assign one key for forwards with value +1 and one key for backwards with value -1.
- Also create one function Turn for turning left and right and assign its respective keys with respective values
- Click add and create one action mapping function Fire for fire and assign a key to it.

## 3.2: Bind the axis / action mapping to our action callback functions

- Exclude the SetupPlayerInputComponent() function from BasePawn.h and BasePawn.cpp and include it both in CowPlayer.h and CowPlayer.cpp.
- In CowPlayer.h, Declare the Move() and Turn() and Fire() funtions.
- Include Speed and Turn Rate variables to fine tune the player's movements.

```cpp
private:
		void Move(float Value);
		void Turn(float Value);
		void Fire();

		UPROPERTY(EditAnywhere, Category = "Components")
		float Speed = 600.f;
		
		UPROPERTY(EditAnywhere, Category = "Components")
		float TurnRate = 100.f;
```

- Define our SetupPlayerInputComponent() in CowPlayer.cpp
- Inside SetupPlayerInputComponent() bind each user input axis or action mapping to its correspondent action callback functions
- Define the action callback functions Move() and Turn() in CowPlayer.cpp

```cpp
#include "Components/InputComponent.h"

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
```

- In unreal > select BP_PawnPlayer > Collision > CapsuleComp > collision presets > set to physicsActor 
- In unreal > select BP_PawnPlayer > physics > set simulate physics off for both the CapsuleComponent and the BaseMesh
- Make sure you move the capsule and the base mesh a little above the ground so that they don't get stuck in the terrain

## 3.3: Fire Function:

- For the player to fire a projectile, we need it to spawn a projectile object from the projectile spawn point. 
- To do this, the Fire() function needs to call a projectile C++ class but this C++ class needs to connect to a blueprint with a mesh so that it can spawn a real object in the world. 
- To make this connection (Reflection) between a C++ class on code and an Unreal Blueprint we need to use a UClass() - Unreal Class - from the type of the object we want to spawn.
- This UClass() will be inside the player C++ code, and will connect to the Projectile blueprint when gets called by the Fire() function.
- To Declare a UClass() we need to use a TSubclassOf<> template function.
- Had we not used UClass() it would only spawn an object based on a raw c++ class which could not contain a static mesh. 

Mouse click > Binding > Fire() > projectile UClass > BP_Projectile > spawn projectile mesh in the world

- In BasePawn.h, Declare the action callback function Fire(). 
- Then use TSubclassOf<> template function to declare a UClass() of type AProjectile.

```cpp
protected:
	void Fire();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;
```

- In the BP_PawnCowPlayer blueprint > Details > Combat > Projectile Class set BP_Projectile as the ProjectileClass to be spawned by the Player.
- Do the same thing in BP_PawnCowEnemy blueprint.

Define the action callback function Fire() in BasePawn.cpp - because this one will be inherited by both the Player and the Enemy actors.
```cpp
#include "Projectile.h"

void ABasePawn::Fire()
{
	// Spawn projectile from the projectile spawn point
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);

	//Set the owner of this projectile to whomever is firing now so that we know who hit and who got hit
	Projectile->SetOwner(this);
}
``` 

### 3.3.1: Set the Projectile movement: 

In Projectile.h, Declare the movement component variable
```cpp
#include "GameFramework/ProjectileMovementComponent.h"

private:
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UProjectileMovementComponent* ProjectileMovement;
```

In Projectile.cpp, construct the projectile movement component 
```cpp
AProjectile::AProjectile()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->InitialSpeed = 1300.f;
}
```

In CowPlayer.cpp, call the fire function of the the parent class
```cpp
void ACowPlayer::Fire()
{
    Super::Fire();
}
```

## 3.5: Implement the Fire function for the Enemies (including a timer)

Create a C++ class inheriting from base pawn to be the enemies class, CowEnemy.cpp

- In CowEnemy.h, Create a Tick function that takes in a float DeltaTime parameter and overrides the tick function that got inherited from base pawn. (before compiling, define Tick function on CowEnemy.cpp
- Declare a begin play function and also override the inherited begin play function. (before compiling, define BeginPlay function in CowEnemy.cpp)
- Create a ACowPlayer* pointer to store the Player's location in order for the enemies to find it and follow it. 
- Create a variable type FTimerHandle to store info about the world time and pass this as parameter to set our timer with a delay for the fire rate. 
- Declare a fire rate and fire range variables to adjust these parameters for the enemies fire conditions
- Declare a CheckFireCondition() function to check if it is the right moment to fire and a InFireRange() function to trigger if Player is within fire range: 
```cpp
#include "CowPlayer.h"


public:

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:

	ACowPlayer* CowPlayerPointer;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 2.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 100.f;

	void CheckFireCondition();

	bool InFireRange();
```

In CowPlayer.h, Declare a bool to tell if the tank is still alive, if it is not, stop firing at it
```cpp
public:
	bool bAlive = true;
```


- in BasePawn.h , Declare a RotateActor() function to rotate the enemy towards the player that take a FVector.
```cpp
protected:
	void RotateActor(FVector LookAtTarget);
```

- In BasePawn.cpp, Define RotateActor() function
- (I added -90.f degrees to the yaw in the RotateToTarget Rotator because the static mesh was always turning sideways instead of facing the player. so these -90 degrees act as an adjustment to that. I also had to turn the ProjectileSpawnPoint + 90.f in respect to the world).
```cpp
void ABasePawn::RotateActor(FVector LookAtTarget)
{
	//This vector will be the distance between the target and the base mesh of the actor who is aimig at it
	FVector ToTarget = LookAtTarget - BaseMesh->GetComponentLocation();

	//Adjust the players rotation to look in the direction (yaw) that our vector is pointing to
	FRotator RotateToTarget = FRotator(0.f, ToTarget.Rotation().Yaw -90.f, 0.f);

	//Set actor rotation passing our rotator variable as a parameter
	BaseMesh->SetWorldRotation(RotateToTarget);
}
```
- In CowEnemy.cpp, Define our custom BeginPlay function to get the Player location in order for the enemy to follow it.
- Define our custom tick function to find the player location and rotate the enemy towards the player if it is in range. 
- Define CheckFireCondition() and InFireRange() functions.
```cpp
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ACowEnemy::BeginPlay()
{

    Super::BeginPlay();

    //Access the player through its pawn and store this result inside our player pointer
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
```


# 4. Actions and Events


## 4.1: Hit event

- **HIT**: Projectile Component hits an Actor > it triggers a Hit Event > the Multicast Delegate function **OnComponentHit**, in the Projectile class, listens to this event and broadcasts **FHitResult** to the Callback Function **OnHit()**, also in the Projectile class, bound to it by **AddDynamic** 

### 4.1.1: Multicast Delegate:

Broadcast the hit event and call the callback function OnHit(). Use a Multicast Delegate OnComponentHit() - already present in the Unreal Engine code.

A Multicast Delegate Works like a listener that calls other functions when an event occurs. It will listen to the hit event generated by OnComponentHit() and broadcast the event data in FHitResult - the Call - to all Callback functions - OnHit() - that are bound to it in its invocation list.

In Projectile.cpp, in BeginPlay(): 
```cpp

```

Make sure that in BP_Projectile blueprint, collision presets are set as "Collision Enabled: Query and Physics"

### 4.1.2: Callback Function

Declare the Callback Functions and Define which actions it will perform 

In Projectile.h: Declare the OnHit() Callback function to be broadcasted when the OnComponentHit function is called.
```cpp


```

In Projectile.cpp: Define the OnHit Callback function to be called by OnComponentHit and specify what actions it will perform.
```cpp


```

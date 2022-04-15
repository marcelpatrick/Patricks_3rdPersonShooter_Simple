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

### 4.1.1: Callback Function

Declare the Callback Functions and Define which actions it will perform 

In Projectile.h: Declare the OnHit() Callback function to be broadcasted when the OnComponentHit function is called and the damage amount
```cpp
private:
	UPROPERTY(EditAnywhere)
	float Damage = 50.f;
	
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp, //Component that hit something
		AActor* OtherActor, //Actor that got hit
		UPrimitiveComponent* OtherComp, //Component associated to the actor that got hit
		FVector NormalImpulse, //Impact generated by the hit
		const FHitResult& Hit //Variable that will store info about the hit results
	);
```

In Projectile.cpp: Define the OnHit Callback function to be called by OnComponentHit and specify what actions it will perform.
- Inside it call Destroy() for the projectile to be destroyed as soon as it hits something.
```cpp
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuse, const FHitResult& Hit)
{
	Destroy();
}
```

### 4.1.2: Multicast Delegate:

Broadcast the hit event and call the callback function OnHit(). Use a Multicast Delegate OnComponentHit() - already present in the Unreal Engine code.

A Multicast Delegate Works like a listener that calls other functions when an event occurs. It will listen to the hit event generated by OnComponentHit() and broadcast the event data in FHitResult - the Call - to all Callback functions - OnHit() - that are bound to it in its invocation list.

In Projectile.cpp, in BeginPlay(): 
```cpp
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}
```

Make sure that in BP_Projectile blueprint > Event Graph > ProjectileMesh > Details > collision > collision presets are set as "Custom" > "Collision Enabled: Query and Physics"


## 4.2: Damage Event: 

- **HIT**: Projectile Component hits an Actor > it triggers a Hit Event > the Multicast Delegate function **OnComponentHit**, in the Projectile class, listens to this event and broadcasts **FHitResult** to the Callback Function **OnHit()**, also in the Projectile class, bound to it by **AddDynamic** 
   - **DAMAGE**: > the **OnHit()** Callback function will apply the damage using **UGamePlaystatics::ApplyDamage()** function inside it > **UGameplayStatics::ApplyDamage()** triggers a Damage Event > the Multicast Delegate function **OnTakeAnyDamage**, in HealthComponent class, listens to this event and broadcasts the damage parameters to the Callback function **DamageTaken()**, also in the HealthComponent class, bound to it by **AddDynamic** > **DamageTaken()** Callback function updates the health variables declared in HealthComponent.h, decreasing the health of the damaged actors

### 4.2.1: Apply Damage OnHit and generate the Damage Event:

In Projectile.cpp define the damage event to be generated by the OnHit() Callback Function when it gets called by the Multicast Delegate OnComponentHit()
```cpp
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"


void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuse, const FHitResult& Hit)
{

	AActor* MyOwner = GetOwner();
	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	UGameplayStatics::ApplyDamage(
		OtherActor, /*damaged actor*/
		Damage, /*damage amount*/
		MyOwnerInstigator, /*the controller of the actor who is causing the damage*/ 
		this, /*actor causing damage*/
		DamageTypeClass /*class of the type of damage this actor is causing*/
		);

	Destroy();
}
```

## 4.2.2: HealthComponent class

Create a new HealthComponent c++ class in Unreal derived from Actor Component. 

In HealhComponent.h create health variables 
```cpp
private:

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;

	float Health = 0.f;
```

In HealthComponent.cpp Define and ininitialize the health variables
```cpp
void UHealthComponent2::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

```

Include our custom component in our Blueprint. In BP_PawnTank, event graph, HealthComponent and do the same for BP_PawnTurret.

## 4.2.3: Callback Function:

Use UGameplayStatics::ApplyDamage to trigger the damage event > Use the OnTakeAnyDamage() Delegate Function to Broadcast the damage event > and call the DamageTaken() Callback Function to > do what the damage will cause to the actors' health.

In HealthComponent.h Declare the Callback Function DamageTaken()
```cpp
private:
	//Callback damage function
	UFUNCTION()
	void DamageTaken(
		AActor* DamagedActor, 
		float Damage, 
		const UDamageType* DamageType, 
		AController* MyOwnerInstigator, 
		AActor* DamageCauser);

```

In HealthComponent.cpp Define the Callback Function DamageTaken() that is bound to OnTakeAnyDamage() Multicast Delegate Function that owns this HealthComponent. Inside it specify what kind of damage it will cause to the Health variables. 
```cpp
void UHealthComponent2::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* MyOwnerInstigator, AActor* DamageCauser)
{

	if (Damage <= 0.f) return;

	Health -= Damage;

	UE_LOG(LogTemp, Warning, TEXT("Voce foi atingido!!!, seu Health atual é de: %f"), Health);
}
```

## 4.2.4: Multicast Delegate:

In HealthComponent.cpp, Define the Multicast Delegate Function OnTakeAnyDamage and bind it to its Callback Function DamageTaken.
```cpp
void UHealthComponent2::BeginPlay()
{
	//Call the multicast delegate function
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent2::DamageTaken);	
}
 
```


# 5: Game Rules and Game Mode.

The Game Mode will be responsible for defining winning, loosing and death conditions, starting and ending the game and defining who the player0 is (default pawn).


## 5.1: Player Controller

Implement our custom Player Controller class. This class will be used to be called from the GameMode when the actor dies to disable input.

### 5.1.1: Create a C++ class.
Create a c++ class derived from Player Controller: Cow3rdPersonPlayerController. 

#### 5.1.1.1: Declare:
In Cow3rdPersonPlayerController.h, Declare a function to switch whether inputs will be enabled or disabled.
```cpp


```

#### 5.1.1.2: Define:
In Cow3rdPersonPlayerController.cpp, Define this function.
```cpp


```

### 5.1.2: Create a blueprint based on the C++ class
Create a BP_Cow3rdPersonPlayerController blueprint based on this Cow3rdPersonPlayerController class.


## 5.2: GameMode: START / STOP

Define when the game starts and stops

### 5.2.1: Create a C++ class:
Create a new c++ class with Game Mode Base as parent class: Cow3rdPersonGameMode. 

### 5.2.2: Create a blueprint based on this C++ class:
Create a new blueprint BP_Cow3rdPersonGameMode based on this new class.

Open BP_Cow3rdPersonGameMode > Details > in Classes > PlayerController class: change the player controller to our custom BP_Cow3rdPersonPlayerController.

In BP_Cow3rdPersonGameMode > Details > Classes > Default Pawn Class, change it to BP_PawnCowPlayer so that this will define the default pawn to be possessed when we start the game as being the tank - we will play as this actor

In Edit > Project settings > Maps and Modes > Default Modes > Default Game Mode, change the default game mode to BP_Cow3rdPersonGameMode.

In the world > Select the BP_PawnCowPlayer actor, in Details > Pawn > Auto Possess player > selct Player0.

### 5.2.3: Declare the variables and functions

In Cow3rdPersonGameMode.h, override BeginPlay() so that we can define out own actions when the game begins.
- Declare a BeginPlay() function and override the custom BeginPlay() function.
```cpp
#include "Cow3rdPersonPlayerController.h"

	protected:
		virtual void BeginPlay() override;
```

### 5.2.4: Define the variables and functions

In Cow3rdPersonGameMode.cpp 
- Define the value of the player controller pointer.
- Define a function to count all alive enemies in the world, call in on game start and pass the current result to a variable.
```cpp
void ACow3rdPersonGameMode::BeginPlay()
{
    Super::BeginPlay();
}
```


## 5.3: GameMode: DEATH

- **HIT**: Projectile Component hits an Actor > it triggers a Hit Event > the Multicast Delegate function **OnComponentHit**, in the Projectile class, listens to this event and broadcasts **FHitResult** to the Callback Function **OnHit()**, also in the Projectile class, bound to it by **AddDynamic** 
   - **DAMAGE**: > the **OnHit()** Callback function will apply the damage using **UGamePlaystatics::ApplyDamage()** function inside it > **UGameplayStatics::ApplyDamage()** triggers a Damage Event > the Multicast Delegate function **OnTakeAnyDamage**, in HealthComponent class, listens to this event and broadcasts the damage parameters to the Callback function **DamageTaken()**, also in the HealthComponent class, bound to it by **AddDynamic** > **DamageTaken()** Callback function updates the health variables declared in HealthComponent.h, decreasing the health of the damaged actors
       - **DEATH**: > (If Health <= 0) > From inside **DamageTaken()** callback function, call the **ActorDied()** function in the ToonTanksGameMode class > From inside **ActorDied()**, call the **HandleDestruction()** function in the BasePawn class that defines what happens when the actor gets destroyed - special effects, particles, sound - and hides the actor from the game so that it is no longer visible.

### 5.3.1: HandleDestruction()

- Create a function to destroy the player that got killed. implement it on basepawn and inside each player class

In BasePawn.h declare HandleDestruction(), BeginPlay and Tick
```cpp
public:
	void HandleDestruction();
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
```

In BasePawn.cpp define HandleDestruction, BeginPlay and Tick
```cpp
void ABasePawn::HandleDestruction()
{
	UE_LOG(LogTemp, Warning, TEXT("Handle destruction called, Actor Died!!!!!!!!!! "));
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
```

In CowPlayer.h define HandleDestruction
```cpp
public:
	void HandleDestruction();
```

In CowPlayer.cpp 
- declare HandleDestruction()
- In BeginPlay, initialize player controller
```cpp
void ACowPlayer::HandleDestruction()
{
    Super::HandleDestruction();

    SetActorHiddenInGame(true);

    SetActorTickEnabled(false);
}

void ACowPlayer::BeginPlay()
{
    Super::BeginPlay();

    CowPlayerController = Cast<APlayerController>(GetController());
}
```

In CowEnemy.h declare HandleDestruction
```cpp
public:
	void HandleDestruction();
```
In CowEnemy.cpp 
- Declare HandleDestruction()
```cpp
void ACowEnemy::HandleDestruction()
{
    Super::HandleDestruction();

    Destroy();
}
```

### 5.3.2: ActorDied() function

- In Cow3rdPersonGameMode.h Declare the ActorDied() function. 
- Add a Player variable to check if the dead actor was the Player or the Enemy. 
- Override BeginPlay().
```cpp
#include "GameFramework/GameModeBase.h"
#include "Cow3rdPersonPlayerController.h"
#include "CowPlayer.h"

public:
	void ActorDied(AActor* DeadActor);
	
protected:
	virtual void BeginPlay() override;

private:
	ACowPlayer* CowPlayer;
```

In Cow3rdPersonGameMode.cpp 
- Define ActorDied() function and from inside that call the HandleDestruction() function.
- Also get our player pawn to disable it in case our player died
```cpp
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
    }
    else if (ACowEnemy* DeadEnemy = Cast<ACowEnemy>(DeadActor))
    {
        DeadEnemy->HandleDestruction();
    }
}

void ACow3rdPersonGameMode::BeginPlay()
{
    Super::BeginPlay();

    CowPlayer = Cast<ACowPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
}
```

### 5.3.3: Call the ActorDied() function from the HealthComponent class to perform the death actions when Health reaches 0.

In HealthComponent.cpp, 
- In BeginPlay
	- Use the multicast delegate function to call damage taken
	- initialize the gamemode variable
- call ActorDied() from the DamageTaken() callback function to perform death when damage makes the Health variables reach 0.
```cpp
void UHealthComponent2::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	//Call the multicast delegate function
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent2::DamageTaken);

	GameMode = Cast<ACow3rdPersonGameMode>(UGameplayStatics::GetGameMode(this));
}

void UHealthComponent2::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;

	Health -= Damage;

	UE_LOG(LogTemp, Warning, TEXT("Enemy Hit!!!!! Remaining Life = %f"), Health);

	if (Health <= 0.f && GameMode)
	{
		GameMode->ActorDied(DamagedActor);
	}
}
```


## 5.4: Define what actions happen when player gets destroyed.

In BasePawn.h, declare the HandleDestruction() function:
```cpp
public:
	void HandleDestruction();
```
In BasePawn.cpp Define HandleDestruction() funtion:
```cpp
void ABasePawn::HandleDestruction()
{
	UE_LOG(LogTemp, Warning, TEXT("Handle destruction called, Actor Died!!!!!!!!!! "));
}
```
Implement HandleDestruction() in the Enemy class.

In Enemy.h, declare the HandleDestruction() function:
```cpp
public:
	void HandleDestruction();
```
In Tower.cpp 
- Define HandleDestruction() funtion and in it call Super::HandleDestruction() to inherit the implementations of this function from the BasePawn.
- call Destroy() to destroy the mesh
```cpp
void ACowEnemy::HandleDestruction()
{
    Super::HandleDestruction();

    Destroy();
}
```

Implement HandleDestruction() in the Player class.

In CowPlayer.h, 
- Declare the HandleDestruction() function 
- Declare a private variable to store the player controller.
- Declare a getter function to access the player controller private variable by other classes
```cpp
public:
	void HandleDestruction();
	
	//Makea a custom public getter for us to get the private variable CowPlayerController from inside other classes
	APlayerController* GetCowPlayerController() const
	{
		return CowPlayerController;
	}

private:
	APlayerController* CowPlayerController; 
```
In Player.cpp 
- Define HandleDestruction() funtion and in it call Super::HandleDestruction() to inherit the implementations of this function from the BasePawn.
- Then hide actor so that it disappears 
- Disable all actions on tick for this actor
- And define its bAlive variable to false so that we know it is dead
```cpp
void ACowPlayer::HandleDestruction()
{
    Super::HandleDestruction();

    SetActorHiddenInGame(true);

    SetActorTickEnabled(false);

    bAlive = false;
}
```



## 5.5: GameMode: GAME OVER / WIN - LOOSE

Define what happens when GameOver: either we destroy all enemies (Win Condition > GameOver = true) or we get destroyed (Loose Condition > GameOver = false)

### 5.5.1: Declare variables:

In Cow3rdPersonGameMode.h
Create a blueprint implementable event so that we can communicate our c++ code with our blueprints and show a Game Over widget on the screen. 
```cpp
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);
```

### 5.5.2: Call functions

In Cow3rdPersonGameMode.cpp 
- Call the GameOver function when our actor dies or when we have destroyed all enemies
- *** since GameOver() is a BlueprintImplementableEvent, we don't need to define this function in Cow3rdPersonGameMode.cpp because it will already be implemented in the blueprint.
```cpp
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
```

### 5.5.3: Create the GameOver blueprint implementable event and show the widget

Create a new widget blueprint: in the unreal engine > create a new folter > right click in it > user interface > widget blueprint. call it WBP_EngGameWidget.
Include a display text component and set it to "is variable".

![image](https://user-images.githubusercontent.com/12215115/158021451-b0af04f8-3ca0-4c7b-9fa5-58317dade81a.png)

in BP_Cow3rdPersonGameMode, Event Graph, 

- Create an event game over, link it to Create Widget, select class WBP_EngGameWidget and output add to viewport linking the return value from the previous function into target
- Pull another string from Create Widget return value and select Get [widget text variable name] text. Output a set Text function an link the output of Add to Viewport as its input.
- Use select to show "You Won" or "You Lost" depending upon the boolean variable value that is coming out of event gameover and link it to SetText.

![image](https://user-images.githubusercontent.com/12215115/158021553-acd3cce7-54cb-422a-be3e-dc0857058fba.png)



# 6: Special Effects


## 6.1: Particle Systems

### 6.1.1: Declare variables

In Projectile.h, Declare pointer variables of the type particle system for HitParticles and TrailParticles and make them editable anywhere.
```cpp
private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* HitParticles; 

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystemComponent* TrailParticles;
```

In BasePawn.h, declare pointer variables of the type particle system for death particles
```cpp
private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* DeathParticles; 
```

### 6.1.2: Spawn Particles

In Projectile.cpp, Spawn the particle when we hit something, inside the apply damage if statement created previously
```cpp
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuse, const FHitResult& Hit)
{
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Problema com as HitParticles!!!!!!"));
	}
}
```

In Projectile.cpp, in the constructor function, construct the smoke trail particle variable and attach it to the root component so that it follows the projectile around.
```cpp
AProjectile::AProjectile()
{
	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail")); 
	TrailParticles->SetupAttachment(RootComponent);
}
```

In BasePawn.cpp, spawn the death particles emitter when the actor gets destroyed
```cpp
void ABasePawn::HandleDestruction()
{
	UE_LOG(LogTemp, Warning, TEXT("Handle destruction called, Actor Died!!!!!!!!!! "));

	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("houve um problema com as DeathParticles"));
	}
	
}
```

### 6.1.3: Set particle systems in the Blueprints

In BP_Projectile > select BP_Projectile > in our HitParticles variable field > select the particles we are going to use from the drop down.

In BP_Projectile > Event Grapgh > select trail particles > details > Particles > Template variable > select projectile trail

In BP_PawnTank > select BP_PawnTank(self) > in Details > Combat > Death Particles > select the particles mesh we are going to use

In BP_PawnTurret > select BP_PawnTurret(self) > in Details > Combat > Death Particles > select the particles mesh we are going to use


## 6.2: Sounds

### 6.2.1: Declare Variables

In Projectile.h, Declare the sound pointer variables of type USoundBase*
```cpp
private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound; 

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* LaunchSound; 
```

In BasePawn.h, declare a sound variable for when our any of the actors die
```cpp
private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* DeathSound; 
```

### 6.2.2: Play Sounds when actors get hit or die

In Projectile.cpp, play launch sound when the projectile is spawned and the hit sound when it hits something
```cpp
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (LaunchSound)
	{
		UGameplayStatics::PlaySound2D(this, LaunchSound);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Problema com LaunchSound"));
	}
}
```

In BasePawn.cpp, play sound when any actor dies
```cpp
void ABasePawn::HandleDestruction()
{
	if (DeathSound)
	{
		UGameplayStatics::PlaySound2D(this, DeathSound);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("problema com DeathSound"));
	}
}
```

### 6.2.3: Set the sound components in the Blueprints

In BP_Projectile > Event Grapgh > select BP_Projectile(self) > Details > Combat > HitSound, LaunchSound > select the sound component from the dropdown menu

In BP_PawnTank > Event Grapgh > select BP_PawnTank(self) > Details > Combat > DeathSound > select the sound component from the dropdown menu

In BP_PawnTurret > Event Grapgh > select BP_PawnTurret(self) > Details > Combat > DeathSound > select the sound component from the dropdown menu

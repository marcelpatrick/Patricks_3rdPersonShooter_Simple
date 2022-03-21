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








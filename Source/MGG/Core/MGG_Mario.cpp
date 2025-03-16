#include "MGG_Mario.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "MGG/GravityFields/BaseGravityFieldComponent.h"

/**
 * @brief Constructor for the player character.
 *
 * @details Initializes the Mario character with default components and settings:
 * 1. Creates a static mesh component for visual representation
 * 2. Sets up a spring arm and camera for third-person view
 * 3. Configures camera settings for smooth following and rotation
 * 4. Sets initial physics values and movement properties
 */
AMGG_Mario::AMGG_Mario()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(SphereMeshAsset.Object);
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = false;     // Allows rotation via controls
	CameraBoom->bInheritPitch = false;               // Pitch for up/down viewing
	CameraBoom->bInheritYaw = false;                 // Allows horizontal rotation
	CameraBoom->bInheritRoll = false;               // Deactivates the roll to avoid strange rotations
	CameraBoom->bDoCollisionTest = true;            // Activate collision to prevent camera from passing through walls
	CameraBoom->bEnableCameraLag = true;            // Adds a slight delay for better fluidity
	CameraBoom->CameraLagSpeed = 10.0f;             // Catch-up camera speed
	CameraBoom->CameraRotationLagSpeed = 10.0f;


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;
}

/**
 * @brief Called when the game starts or when the actor is spawned.
 *
 * @details Initializes the player's starting state:
 * 1. Sets default gravity vector (typically downward)
 * 2. Searches the level for gravity field components
 * 3. Checks if the player starts within any gravity fields
 * 4. Sets initial gravity vector based on starting position
 */
void AMGG_Mario::BeginPlay()
{
	Super::BeginPlay();
	GravityVector = FVector(0, 0, -980.0f);  // Default gravity
	
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

	for (const AActor* Actor : AllActors)
	{
		if (UBaseGravityFieldComponent* GravityField = Actor->FindComponentByClass<UBaseGravityFieldComponent>())
		{
			if (GravityField->IsActorInGravityField(this))
			{
				GravityFields.Add(GravityField);
			}
		}
	}
	
	if (GravityFields.Num() > 0)
	{
		bIsInGravityField = true;

		if (UBaseGravityFieldComponent* ActiveField = GetActiveGravityField())
		{
			GravityVector = ActiveField->CalculateGravityVector(GetActorLocation());
		}
	}
}

/**
 * @brief Handles player movement input.
 *
 * @details Translates 2D input values from the controller into 3D movement considering gravity orientation:
 * 1. Gets movement input vector from the controller
 * 2. Obtains the camera rotation for movement direction reference
 * 3. Calculates the up vector as opposite to the current gravity direction
 * 4. Projects forward and right vectors onto the plane perpendicular to gravity
 * 5. Constructs a movement direction vector based on input and projected directions
 * 6. Adds this vector to the character's velocity
 *
 * This gravity-relative movement system is key to Super Mario Galaxy-style gameplay,
 * allowing the player to move naturally on any surface regardless of its orientation.
 *
 * @param Value The input value from the movement controls.
 */
void AMGG_Mario::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator CameraRotation = CameraBoom->GetComponentRotation();
		
		FVector Forward = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
		FVector Right = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
		FVector Up = -GravityVector.GetSafeNormal();
		
		Forward = FVector::VectorPlaneProject(Forward, Up).GetSafeNormal();
		Right = FVector::VectorPlaneProject(Right, Up).GetSafeNormal();
		
		// Y positive = forward, Y negative = reverse
		// X positive = right, X negative = left
		FVector DesiredMovement = Forward * MovementVector.Y + Right * MovementVector.X;
		DesiredMovement.Normalize();
		Velocity += DesiredMovement;
	}
}

/**
 * @brief Handles player camera control input.
 *
 * @details Processes input values for camera rotation:
 * 1. Updates the camera yaw (horizontal rotation) based on X input
 * 2. Updates the camera pitch (vertical rotation) based on Y input
 * 3. Clamps the pitch value to prevent over-rotation
 *
 * @param Value The input value from the look controls.
 */
void AMGG_Mario::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	CameraYaw += LookAxisVector.X;
	CameraPitch = FMath::Clamp(CameraPitch + LookAxisVector.Y, -80.0f, 80.0f);
}

void AMGG_Mario::Jump()
{
	
}

void AMGG_Mario::StopJumping()
{
	
}

/**
 * @brief Rotates the character to align with the current gravity direction.
 *
 * @details Calculates and applies the rotation needed to make the character stand
 * perpendicular to the current gravity direction:
 * 1. Normalizes the current gravity vector
 * 2. Establishes reference vectors to calculate orientation
 * 3. Uses cross products to find perpendicular vectors defining the character's plane
 * 4. Calculates rotation to align the character's up vector opposite to gravity
 * 5. Sets the character's rotation to this new orientation
 * 6. Applies camera rotation relative to the new character orientation
 *
 * This method is essential for creating the illusion that the character is properly
 * standing on surfaces of any orientation, which is a signature feature of
 * Super Mario Galaxy's gameplay.
 */
void AMGG_Mario::RotatingMario()
{
	FVector NG = GravityVector;
	NG.Normalize();

	//Check the reference of the Vector
	FVector temp = FVector(1, 0, 0);
	if (FVector::DotProduct(NG, temp) > 0.99) { temp = FVector(0, 1, 0); }

	FVector VR = FVector::CrossProduct(NG, temp);
	VR.Normalize();
	VR = FVector::CrossProduct(NG, VR);
	VR.Normalize();

	FVector PointDepart = GetActorLocation();
	FVector PlaneNormal = -NG.GetSafeNormal();

	//Debug Forward Vector in Yellow
	{
		DrawDebugLine(
			GetWorld(),
			PointDepart,
			PointDepart + GetActorForwardVector() * 100.f,
			FColor::Yellow,
			false,
			.1f,
			0,
			1.0f
		);
	}
	//Debug Up Vector in Black
	{
		DrawDebugLine(
			GetWorld(),
			PointDepart,
			PointDepart + GetActorUpVector() * 100.f,
			FColor::Black,
			false,
			.1f,
			0,
			1.0f
		);
	}
	
	FVector DefaultUp = FVector(0, 0, 1);
	FQuat AlignementRotation = FQuat::FindBetweenVectors(DefaultUp, PlaneNormal);

	FRotator NewRotation = AlignementRotation.Rotator();
	SetActorRotation(NewRotation);
	FQuat RCamera = FQuat::MakeFromRotator(FRotator(CameraPitch, CameraYaw, 0));
	CameraBoom->SetRelativeRotation(RCamera);
}

/**
 * @brief Called every frame to update the character.
 *
 * @details Calls PhysicProcess to handle physics simulation and then performs
 * standard actor tick operations.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void AMGG_Mario::Tick(float DeltaTime)
{
	PhysicProcess(DeltaTime);
	Super::Tick(DeltaTime);
}

/**
 * @brief Processes physics simulation for the character on each frame.
 *
 * @details Handles all physics-related updates for the player character:
 * 1. Updates the current gravity field affecting the player
 * 2. Performs ground detection using a raycast in the gravity direction
 * 3. Applies gravity only when the character is not grounded
 * 4. Updates the character's position based on velocity and gravity
 * 5. Resets velocity after movement is applied
 * 6. Calls RotatingMario() to align the character with the current gravity
 *
 * This method is crucial for the Mario Galaxy-style gameplay as it handles
 * how the character interacts with gravity fields and surfaces.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void AMGG_Mario::PhysicProcess(float DeltaTime)
{
	UpdateCurrentGravityField();
	
	float UsingGravity = 1;

	FVector PointDepart = GetActorLocation();

	// End point (forward direction * distance)
	FVector Direction = GravityVector;
	Direction.Normalize();
	FVector PointArrivee = PointDepart + (Direction * 120.0f);

	// Raycast configuration
	FHitResult ResultatHit;
	FCollisionQueryParams ParamsCollision;
	ParamsCollision.AddIgnoredActor(this); // Ignore self
	TEnumAsByte<ECollisionChannel> CanalCollision = ECC_Visibility;
	
	// Perform raycast
	bool aHit = GetWorld()->LineTraceSingleByChannel(
		ResultatHit,
		PointDepart,
		PointArrivee,
		CanalCollision,
		ParamsCollision
	);

	// Draw a debug line to visualize the raycast
	DrawDebugLine(
		GetWorld(),
		PointDepart,
		PointArrivee,
		FColor::Red,
		false,
		2.0f,
		0,
		1.0f
	);

	// If the raycast has touched anything
	if (aHit)
	{
		UsingGravity = 0;
	}
	else
	{
		UsingGravity = 1;
	}

	FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime * Speed) + (GravityVector * DeltaTime * UsingGravity);
	Velocity = FVector(0);
	SetActorLocation(NewLocation, true);
	RotatingMario();
}

/**
 * @brief Sets up input bindings for the player character.
 *
 * @details Configures the input mapping context and binds input actions to character functions:
 * 1. Adds the default mapping context to the player controller
 * 2. Binds the Jump action to Jump and StopJumping methods
 * 3. Binds the Move action to the Move method
 * 4. Binds the Look action to the Look method
 *
 * @param PlayerInputComponent The input component to bind to.
 */
void AMGG_Mario::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMGG_Mario::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMGG_Mario::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMGG_Mario::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMGG_Mario::Look);
	}
}

/**
 * @brief Implementation of the IGravityAffected interface method for entering gravity fields.
 *
 * @details Called when the character enters a gravity field:
 * 1. Checks if the character was previously in a gravity field
 * 2. Updates the flag indicating gravity field presence
 * 3. Logs the new gravity vector for debugging
 * 4. Updates the character's internal gravity vector
 *
 * This method is critical for the character to respond to different gravity fields
 * as they move through the game world.
 *
 * @param NewGravityVector The gravity vector of the entered field.
 */
void AMGG_Mario::OnEnterGravityField_Implementation(const FVector& NewGravityVector)
{
	if (!bIsInGravityField)
	{
		bIsInGravityField = true;
		UE_LOG(LogTemp, Warning, TEXT("Player: new gravity vector received : %s"), *NewGravityVector.ToString());
		GravityVector = NewGravityVector;
	}
}

/**
 * @brief Implementation of the IGravityAffected interface method for exiting gravity fields.
 *
 * @details Called when the character exits all gravity fields:
 * 1. Checks if the character was previously in a gravity field
 * 2. Updates the flag indicating gravity field presence
 * 3. Resets the gravity vector to the default downward direction
 * 4. Logs the gravity field exit for debugging
 *
 * This method ensures the character returns to normal gravity when not
 * under the influence of any special gravity fields.
 */
void AMGG_Mario::OnExitGravityField_Implementation()
{
	if (bIsInGravityField)
	{
		bIsInGravityField = false;
		GravityVector = FVector(0, 0, -980.0f);
		UE_LOG(LogTemp, Warning, TEXT("Player: leaving the gravity field"));
	}
}

/**
 * @brief Updates the character's current gravity based on active gravity fields.
 *
 * @details Implements the IGravityAffected interface method:
 * 1. Gets the highest priority active gravity field
 * 2. If a field is found, updates the gravity vector based on the character's position
 * 3. If no field is found, could optionally reset to default gravity
 *
 * This method is typically called each frame to ensure the character always
 * experiences the correct gravitational influence as they move through the world.
 */
void AMGG_Mario::UpdateCurrentGravityField()
{
	UBaseGravityFieldComponent* ActiveField = GetActiveGravityField();
	
	if (ActiveField)
	{
		GravityVector = ActiveField->CalculateGravityVector(GetActorLocation());
	}
	else
	{
		// No gravity field found, reset gravity to default
	}
}

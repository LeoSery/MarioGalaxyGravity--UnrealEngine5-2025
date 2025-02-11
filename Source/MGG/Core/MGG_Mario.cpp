#include "MGG_Mario.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

AMGG_Mario::AMGG_Mario()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(SphereMeshAsset.Object);
		MeshComponent->SetWorldScale3D(FVector(0.5f));
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true;

}

void AMGG_Mario::BeginPlay()
{
	Super::BeginPlay();
	GravityVector = FVector(0, 0, -980.0f);  // default gravity
}

void AMGG_Mario::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		Velocity += (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);
	}
}

void AMGG_Mario::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMGG_Mario::Jump()
{
	
}

void AMGG_Mario::StopJumping()
{
}

void AMGG_Mario::Tick(float DeltaTime)
{
	PhysicProcess(DeltaTime);
	Super::Tick(DeltaTime);
}

void AMGG_Mario::PhysicProcess(float DeltaTime)
{
	float UsingGravity = 1;

	FVector PointDepart = GetActorLocation();

	// Point d'arrivée (direction vers l'avant * distance)
	FVector Direction = GetActorUpVector();
	FVector PointArrivee = PointDepart + (-1* Direction * 31.0f);

	// Configuration du raycast
	FHitResult ResultatHit;
	FCollisionQueryParams ParamsCollision;
	ParamsCollision.AddIgnoredActor(this); // Ignore l'acteur qui lance le raycast
	TEnumAsByte<ECollisionChannel> CanalCollision = ECC_Visibility;
	// Effectuer le raycast
	bool aHit = GetWorld()->LineTraceSingleByChannel(
		ResultatHit,
		PointDepart,
		PointArrivee,
		CanalCollision,
		ParamsCollision
	);

	// Dessiner une ligne de debug pour visualiser le raycast
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

	// Si le raycast a touché quelque chose
	if (aHit)
		UsingGravity = 0;
	else
		UsingGravity = 1;

	FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime * Speed) + (GravityVector * DeltaTime * UsingGravity);
	Velocity = FVector(0);
	SetActorLocation(NewLocation, true);
}

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

void AMGG_Mario::OnEnterGravityField_Implementation(const FVector& NewGravityVector)
{
	if (!bIsInGravityField)
	{
		bIsInGravityField = true;
		UE_LOG(LogTemp, Warning, TEXT("Player: new gravity vector received : %s"), *NewGravityVector.ToString());
		GravityVector = NewGravityVector;
	}
}

void AMGG_Mario::OnExitGravityField_Implementation()
{
	if (bIsInGravityField)
	{
		bIsInGravityField = false;
		GravityVector = FVector(0, 0, -980.0f);
		UE_LOG(LogTemp, Warning, TEXT("Player: leaving the gravity field"));
	}
}

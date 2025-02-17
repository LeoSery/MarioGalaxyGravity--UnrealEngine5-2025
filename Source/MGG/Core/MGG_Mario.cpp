#include "MGG_Mario.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MGG/GravityFields/BaseGravityFieldComponent.h"

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
	CameraBoom->bUsePawnControlRotation = false;     // Permet la rotation via les contrôles
	CameraBoom->bInheritPitch = false;               // Permet le pitch pour regarder haut/bas
	CameraBoom->bInheritYaw = false;                 // Permet la rotation horizontale
	CameraBoom->bInheritRoll = false;               // Désactive le roll pour éviter les rotations étranges
	CameraBoom->bDoCollisionTest = true;            // Active la collision pour éviter que la caméra traverse les murs

	// Ajoutons ces paramètres pour un comportement plus fluide
	CameraBoom->bEnableCameraLag = true;            // Ajoute un léger délai pour plus de fluidité
	CameraBoom->CameraLagSpeed = 10.0f;             // Vitesse de rattrapage de la caméra
	CameraBoom->CameraRotationLagSpeed = 10.0f;


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;

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
		const FRotator CameraRotation = CameraBoom->GetComponentRotation();
		
		FVector Forward = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
		FVector Right = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
		FVector Up = -GravityVector.GetSafeNormal();
		
		Forward = FVector::VectorPlaneProject(Forward, Up).GetSafeNormal();
		Right = FVector::VectorPlaneProject(Right, Up).GetSafeNormal();
		
		// Y positif = avant, Y négatif = arrière
		// X positif = droite, X négatif = gauche
		FVector DesiredMovement = Forward * MovementVector.Y + Right * MovementVector.X;
		DesiredMovement.Normalize();
		Velocity += DesiredMovement;
	}
}

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

void AMGG_Mario::Tick(float DeltaTime)
{
	PhysicProcess(DeltaTime);
	Super::Tick(DeltaTime);
}

void AMGG_Mario::PhysicProcess(float DeltaTime)
{
	UpdateCurrentGravityField();
	
	float UsingGravity = 1;

	FVector PointDepart = GetActorLocation();

	// Point d'arriv�e (direction vers l'avant * distance)
	FVector Direction = GravityVector;
	Direction.Normalize();
	FVector PointArrivee = PointDepart + (Direction * 31.0f);

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

	// Si le raycast a touch� quelque chose
	if (aHit)
		UsingGravity = 0;
	else
		UsingGravity = 1;

	FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime * Speed) + (GravityVector * DeltaTime * UsingGravity);
	Velocity = FVector(0);
	SetActorLocation(NewLocation, true);
	RotatingMario();
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

void AMGG_Mario::UpdateCurrentGravityField()
{
	if (CurrentGravityField)
	{
		GravityVector = CurrentGravityField->CalculateGravityVector(GetActorLocation());
	}
}

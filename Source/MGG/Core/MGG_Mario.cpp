#include "MGG_Mario.h"
#include "InputAction.h"

AMGG_Mario::AMGG_Mario()
{
	PrimaryActorTick.bCanEverTick = true;

	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(SphereMeshAsset.Object);
		MeshComponent->SetWorldScale3D(FVector(0.5f));
	}
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
		const FVector ForwardDirection = FRotator(0, GetWorld()->GetFirstPlayerController()->PlayerCameraManager.Get()->GetCameraRotation().Yaw, 0).Vector();
		const FVector RightDirection = ForwardDirection.RotateAngleAxis(90, { 0,0,1 });
		Velocity += ForwardDirection + RightDirection;
	}
}

void AMGG_Mario::Look(const FInputActionValue& Value)
{
	
}

void AMGG_Mario::Jump()
{
	
}

void AMGG_Mario::Tick(float DeltaTime)
{
	PhysicProcess(DeltaTime);
	Super::Tick(DeltaTime);
}

void AMGG_Mario::PhysicProcess(float DeltaTime)
{
	Velocity += GravityVector * DeltaTime;
	
	FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime);
	SetActorLocation(NewLocation, true);
}

void AMGG_Mario::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMGG_Mario::OnEnterGravityField(const FVector& NewGravityVector)
{
	if (!bIsInGravityField)
	{
		bIsInGravityField = true;
		UE_LOG(LogTemp, Warning, TEXT("Player: new gravity vector received : %s"), *NewGravityVector.ToString());
		GravityVector = NewGravityVector;
	}
}

void AMGG_Mario::OnExitGravityField()
{
	if (bIsInGravityField)
	{
		bIsInGravityField = false;
		GravityVector = FVector(0, 0, -980.0f);
		UE_LOG(LogTemp, Warning, TEXT("Player: leaving the gravity field"));
	}
}

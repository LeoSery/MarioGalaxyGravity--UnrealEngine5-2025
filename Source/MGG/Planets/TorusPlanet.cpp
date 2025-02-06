#include "TorusPlanet.h"

ATorusPlanet::ATorusPlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	PlanetRadius = 1000.0f;
	PlanetGravity = 9.81f;

	TorusGravityField = CreateDefaultSubobject<UTorusGravityFieldComponent>(TEXT("TorusGravityField"));
	TorusGravityField->SetupAttachment(RootComponent);
}

void ATorusPlanet::BeginPlay()
{
	Super::BeginPlay();
}

void ATorusPlanet::InitializeGravityField()
{
	if (TorusGravityField)
	{
		TorusGravityField->SetGravityInfluenceRange(PlanetRadius);
		TorusGravityField->SetGravityStrength(PlanetGravity);
	}
}

void ATorusPlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATorusPlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	InitializeGravityField();
}
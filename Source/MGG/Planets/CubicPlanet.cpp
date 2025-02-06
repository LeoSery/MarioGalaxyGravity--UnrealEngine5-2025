#include "CubicPlanet.h"

ACubicPlanet::ACubicPlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	PlanetRadius = 1000.0f;
	PlanetGravity = 9.81f;

	CubeGravityField = CreateDefaultSubobject<UCubeGravityFieldComponent>(TEXT("CubeGravityField"));
	CubeGravityField->SetupAttachment(RootComponent);
}

void ACubicPlanet::BeginPlay()
{
	Super::BeginPlay();
}

void ACubicPlanet::InitializeGravityField()
{
	if (CubeGravityField)
	{
		CubeGravityField->SetGravityInfluenceRange(PlanetRadius);
		CubeGravityField->SetGravityStrength(PlanetGravity);
	}
}

void ACubicPlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACubicPlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	InitializeGravityField();
}

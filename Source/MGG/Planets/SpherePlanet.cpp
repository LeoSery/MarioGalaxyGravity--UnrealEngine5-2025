#include "SpherePlanet.h"

ASpherePlanet::ASpherePlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	PlanetRadius = 1000.0f;
	PlanetGravity = 9.81f;

	SphereGravityField = CreateDefaultSubobject<USphereGravityFieldComponent>(TEXT("SphereGravityField"));
	SphereGravityField->SetupAttachment(RootComponent);
}

void ASpherePlanet::BeginPlay()
{
	Super::BeginPlay();
}

void ASpherePlanet::InitializeGravityField()
{
	if (SphereGravityField)
	{
		SphereGravityField->SetGravityInfluenceRange(PlanetRadius);
		SphereGravityField->SetGravityStrength(PlanetGravity);
	}
}

void ASpherePlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpherePlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	InitializeGravityField();
}

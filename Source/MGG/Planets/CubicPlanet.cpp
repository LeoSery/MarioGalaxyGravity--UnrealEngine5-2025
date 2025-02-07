#include "CubicPlanet.h"

ACubicPlanet::ACubicPlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	CubeGravityField = CreateDefaultSubobject<UCubeGravityFieldComponent>(TEXT("CubeGravityField"));
	CubeGravityField->SetupAttachment(RootComponent);
}

void ACubicPlanet::BeginPlay()
{
	Super::BeginPlay();
	SyncGravityFieldSettings();
}

void ACubicPlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACubicPlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SyncGravityFieldSettings();
}

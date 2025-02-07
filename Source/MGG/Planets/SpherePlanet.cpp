#include "SpherePlanet.h"

ASpherePlanet::ASpherePlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereGravityField = CreateDefaultSubobject<USphereGravityFieldComponent>(TEXT("SphereGravityField"));
	SphereGravityField->SetupAttachment(RootComponent);
}

void ASpherePlanet::BeginPlay()
{
	Super::BeginPlay();
	SyncGravityFieldSettings();
}

void ASpherePlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpherePlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SyncGravityFieldSettings();
}

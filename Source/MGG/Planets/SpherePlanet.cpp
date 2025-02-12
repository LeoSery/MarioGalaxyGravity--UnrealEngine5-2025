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

	if (SphereGravityField)
	{
		SphereGravityField->UpdateFieldDimensions();
	}
}

void ASpherePlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SyncGravityFieldSettings();

	if (SphereGravityField)
	{
		SphereGravityField->UpdateFieldDimensions();
		SphereGravityField->RedrawDebugField();
	}
}

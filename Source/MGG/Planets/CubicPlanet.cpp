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

	if (CubeGravityField)
	{
		CubeGravityField->UpdateFieldDimensions();
		CubeGravityField->RedrawDebugField();
	}
}

void ACubicPlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SyncGravityFieldSettings();

	if (CubeGravityField)
	{
		CubeGravityField->UpdateFieldDimensions();
		CubeGravityField->RedrawDebugField();
	}
}

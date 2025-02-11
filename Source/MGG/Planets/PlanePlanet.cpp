#include "PlanePlanet.h"

APlanePlanet::APlanePlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	PlaneGravityField = CreateDefaultSubobject<UPlaneGravityFieldComponent>(TEXT("PlaneGravityField"));
	PlaneGravityField->SetupAttachment(RootComponent);
}

void APlanePlanet::BeginPlay()
{
	Super::BeginPlay();
	SyncGravityFieldSettings();

	if (PlaneGravityField)
	{
		PlaneGravityField->UpdateFieldDimensions();
	}
}

void APlanePlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	static bool bFirstConstruction = true;
	if (bFirstConstruction && PlanetMesh && PlanetMesh->GetStaticMesh())
	{
		FBoxSphereBounds bounds = PlanetMesh->GetStaticMesh()->GetBounds();
		float meshHeight = bounds.BoxExtent.Z * 2.0f;
		
		FVector meshOffset = FVector(0.0f, 0.0f, -meshHeight * 0.5f);
		FVector totalOffset = InitialOffset + meshOffset;
	}
	
	SyncGravityFieldSettings();

	if (PlaneGravityField)
	{
		PlaneGravityField->UpdateFieldDimensions();
		PlaneGravityField->RedrawDebugField();
	}
}
#include "CylinderPlanet.h"

ACylinderPlanet::ACylinderPlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("/Engine/BasicShapes/Cylinder"));
	if (CylinderMeshAsset.Succeeded() && PlanetMesh)
	{
		PlanetMesh->SetStaticMesh(CylinderMeshAsset.Object);
		DefaultMesh = CylinderMeshAsset.Object;
	}

	CylinderGravityField = CreateDefaultSubobject<UCylinderGravityFieldComponent>(TEXT("CylinderGravityField"));
	CylinderGravityField->SetupAttachment(RootComponent);
}

void ACylinderPlanet::BeginPlay()
{
	Super::BeginPlay();
	SyncGravityFieldSettings();

	if (CylinderGravityField)
	{
		float cylinderHeight = PlanetRadius * CylinderHeightRatio;
		CylinderGravityField->CylinderHeight = cylinderHeight;
        
		CylinderGravityField->UpdateFieldDimensions();
		CylinderGravityField->RedrawDebugField();
	}
}

void ACylinderPlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SyncGravityFieldSettings();

	if (CylinderGravityField)
	{
		float cylinderHeight = PlanetRadius * CylinderHeightRatio;
		CylinderGravityField->CylinderHeight = cylinderHeight;
        
		CylinderGravityField->UpdateFieldDimensions();
		CylinderGravityField->RedrawDebugField();
	}
}
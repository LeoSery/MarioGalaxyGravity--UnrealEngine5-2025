#include "CylinderPlanet.h"

/**
 * @brief Constructor for the cylinder planet class.
 *
 * @details Initializes the planet with a cylinder gravity field component.
 * Sets up the tick functionality and attaches the gravity field to the root component.
 * Also attempts to load and assign a default cylinder mesh if available.
 */
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

/**
 * @brief Called when the game starts or when the actor is spawned.
 *
 * @details Calls the parent BeginPlay method, synchronizes gravity field settings,
 * and configures the cylinder gravity field using the planet's radius and height ratio.
 * The CylinderHeight is calculated from the planet radius multiplied by the height ratio.
 */
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

/**
 * @brief Called when the actor is placed or moved in the editor.
 *
 * @details Updates the planet's transform and synchronizes gravity field settings.
 * Also updates the cylinder height based on the current planet radius and height ratio,
 * ensuring consistent proportions when the planet is scaled.
 *
 * @param Transform The new transform of the actor.
 */
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

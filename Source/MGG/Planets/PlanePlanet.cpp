#include "PlanePlanet.h"

/**
 * @brief Constructor for the plane planet class.
 *
 * @details Initializes the planet with a plane gravity field component.
 * Sets up the tick functionality and attaches the gravity field to the root component.
 */
APlanePlanet::APlanePlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	PlaneGravityField = CreateDefaultSubobject<UPlaneGravityFieldComponent>(TEXT("PlaneGravityField"));
	PlaneGravityField->SetupAttachment(RootComponent);
}

/**
 * @brief Called when the game starts or when the actor is spawned.
 *
 * @details Calls the parent BeginPlay method, synchronizes gravity field settings,
 * and ensures the plane gravity field is properly initialized with updated dimensions
 * and debug visualization.
 */
void APlanePlanet::BeginPlay()
{
	Super::BeginPlay();
	SyncGravityFieldSettings();

	if (PlaneGravityField)
	{
		PlaneGravityField->UpdateFieldDimensions();
		PlaneGravityField->RedrawDebugField();
	}
}

/**
 * @brief Called when the actor is placed or moved in the editor.
 *
 * @details Updates the planet's transform and synchronizes gravity field settings.
 * On first construction, applies the InitialOffset adjusted for mesh height to properly
 * position the plane. This handles special positioning requirements for planar gravity.
 *
 * @param Transform The new transform of the actor.
 */
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
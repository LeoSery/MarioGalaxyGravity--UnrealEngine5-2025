#include "CubicPlanet.h"

/**
 * @brief Constructor for the cubic planet class.
 *
 * @details Initializes the planet with a cube gravity field component.
 * Sets up the tick functionality and attaches the gravity field to the root component.
 */
ACubicPlanet::ACubicPlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	CubeGravityField = CreateDefaultSubobject<UCubeGravityFieldComponent>(TEXT("CubeGravityField"));
	CubeGravityField->SetupAttachment(RootComponent);
}

/**
 * @brief Called when the game starts or when the actor is spawned.
 *
 * @details Calls the parent BeginPlay method, synchronizes gravity field settings,
 * and ensures the cube gravity field is properly initialized with updated dimensions
 * and debug visualization.
 */
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

/**
 * @brief Called when the actor is placed or moved in the editor.
 *
 * @details Updates the planet's transform and synchronizes gravity field settings.
 * This ensures that the cube gravity field properly reflects the planet's position,
 * orientation, and properties in the editor.
 *
 * @param Transform The new transform of the actor.
 */
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

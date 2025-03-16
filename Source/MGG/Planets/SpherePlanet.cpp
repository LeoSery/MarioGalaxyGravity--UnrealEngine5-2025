#include "SpherePlanet.h"

/**
 * @brief Constructor for the sphere planet class.
 *
 * @details Initializes the planet with a sphere gravity field component.
 * Sets up the tick functionality and attaches the gravity field to the root component.
 */
ASpherePlanet::ASpherePlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereGravityField = CreateDefaultSubobject<USphereGravityFieldComponent>(TEXT("SphereGravityField"));
	SphereGravityField->SetupAttachment(RootComponent);
}

/**
 * @brief Called when the game starts or when the actor is spawned.
 *
 * @details Calls the parent BeginPlay method, synchronizes gravity field settings,
 * and ensures the sphere gravity field is properly initialized with updated dimensions
 * and debug visualization.
 */
void ASpherePlanet::BeginPlay()
{
	Super::BeginPlay();
	SyncGravityFieldSettings();

	if (SphereGravityField)
	{
		SphereGravityField->UpdateFieldDimensions();
		SphereGravityField->RedrawDebugField();
	}
}

/**
 * @brief Called when the actor is placed or moved in the editor.
 *
 * @details Updates the planet's transform and synchronizes gravity field settings.
 * This ensures that the sphere gravity field properly reflects the planet's position,
 * orientation, and properties in the editor.
 *
 * @param Transform The new transform of the actor.
 */
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

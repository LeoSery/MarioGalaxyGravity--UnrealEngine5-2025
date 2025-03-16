#include "BasePlanet.h"

#include "MGG/GravityFields/BaseGravityFieldComponent.h"

/**
 * @brief Constructor for the base planet class.
 *
 * @details Initializes the planet with default mesh and gravity settings.
 * Sets up the static mesh component as the root component.
 */
ABasePlanet::ABasePlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	PlanetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlanetMesh"));
	PlanetMesh->SetGenerateOverlapEvents(false);
	RootComponent = PlanetMesh;
	
	PlanetRadius = 1000.0f;
	GravityStrength = 981.0f;
	GravityFieldPriority = 0;
	GravityInfluenceRange = 1000.0f;

	if (PlanetMesh && DefaultMesh)
	{
		PlanetMesh->SetStaticMesh(DefaultMesh);
	}
}

/**
 * @brief Called when the game starts or when the actor is spawned.
 *
 * @details Caches the gravity field component and initializes its dimensions and debug visualization.
 */
void ABasePlanet::BeginPlay()
{
	CachedGravityField = GetComponentByClass<UBaseGravityFieldComponent>();
	
	if (CachedGravityField)
	{
		CachedGravityField->UpdateGravityVolume();
		CachedGravityField->RedrawDebugField();
	}
	
	Super::BeginPlay();
}

/**
 * @brief Updates the scale of the planet based on its radius.
 *
 * @details Calculates a scale factor based on the planet radius and applies it to the mesh.
 * This ensures all planets have a consistent size relationship regardless of their mesh.
 */
void ABasePlanet::UpdatePlanetScale()
{
	if (PlanetMesh)
	{
		float const ScaleFactor = PlanetRadius / 100.0f;
		PlanetMesh->SetWorldScale3D(FVector(ScaleFactor));
	}
}

/**
 * @brief Updates the planet's mesh to match the configured default mesh.
 *
 * @details Checks if the current mesh matches the default mesh and updates it if necessary,
 * then calls UpdatePlanetScale to ensure proper sizing.
 */
void ABasePlanet::UpdatePlanetMesh()
{
	if (PlanetMesh && DefaultMesh && PlanetMesh->GetStaticMesh() != DefaultMesh)
	{
		PlanetMesh->SetStaticMesh(DefaultMesh);
		UpdatePlanetScale();
	}
}

/**
 * @brief Called every frame to update the planet.
 *
 * @details Handles any per-frame updates required by the planet.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ABasePlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * @brief Called when the actor is placed or moved in the editor.
 *
 * @details Updates the planet's mesh and scale to ensure proper visualization in the editor.
 *
 * @param Transform The new transform of the actor.
 */
void ABasePlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdatePlanetMesh();
	UpdatePlanetScale();
}

/**
 * @brief Called when a property of the planet is changed in the editor.
 *
 * @details Handles updates to the planet's properties, such as radius or gravity settings,
 * ensuring that dependent components are updated accordingly.
 *
 * @param PropertyChangedEvent Information about the property that was changed.
 */
void ABasePlanet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ABasePlanet, PlanetRadius))
	{
		UpdatePlanetScale();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(ABasePlanet, DefaultMesh))
	{
		UpdatePlanetMesh();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(ABasePlanet, GravityStrength) ||
			 PropertyName == GET_MEMBER_NAME_CHECKED(ABasePlanet, GravityFieldPriority) ||
			 PropertyName == GET_MEMBER_NAME_CHECKED(ABasePlanet, GravityInfluenceRange))
	{
		SyncGravityFieldSettings();
	}
	
	if (UBaseGravityFieldComponent* GravityField = GetComponentByClass<UBaseGravityFieldComponent>()) //work but not optimized -> disable it after level design
	{
		GravityField->RedrawDebugField();
	}
}

/**
 * @brief Called when the planet is moved in the editor.
 *
 * @details Updates the debug visualization of the gravity field when the planet is moved.
 *
 * @param bFinished Whether the move operation has completed.
 */
void ABasePlanet::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (UBaseGravityFieldComponent* GravityField = GetComponentByClass<UBaseGravityFieldComponent>())
	{
		GravityField->RedrawDebugField();
	}
}

/**
 * @brief Synchronizes the gravity field settings with the planet's configuration.
 *
 * @details Transfers the planet's gravity settings (strength, priority, influence range)
 * to its associated gravity field component. This ensures that changes made to the planet's
 * properties in the editor are reflected in the actual gravity behavior at runtime.
 * 
 * The method:
 * 1. Caches the gravity field component if not already cached
 * 2. Compares current settings with the component's settings
 * 3. Updates only the values that have changed
 * 
 * This is essential for maintaining consistency between the planet's visual representation
 * and its gravitational effects on gameplay.
 */
void ABasePlanet::SyncGravityFieldSettings()
{
	if (!CachedGravityField)
	{
		CachedGravityField = GetComponentByClass<UBaseGravityFieldComponent>();
	}
	
	if (CachedGravityField)
	{
		if (CachedGravityField->GetGravityStrength() != GravityStrength)
			CachedGravityField->SetGravityStrength(GravityStrength);
            
		if (CachedGravityField->GetGravityFieldPriority() != GravityFieldPriority)
			CachedGravityField->SetGravityFieldPriority(GravityFieldPriority);
            
		if (CachedGravityField->GetGravityInfluenceRange() != GravityInfluenceRange)
			CachedGravityField->SetGravityInfluenceRange(GravityInfluenceRange);
	}
}
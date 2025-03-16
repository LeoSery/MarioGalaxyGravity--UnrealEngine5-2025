#include "TorusPlanet.h"

/**
 * @brief Constructor for the torus planet class.
 *
 * @details Initializes the planet with both a torus gravity field component and a
 * procedural torus mesh component. Sets up the tick functionality, attaches the 
 * components to the root, and configures collision settings for the torus mesh.
 */
ATorusPlanet::ATorusPlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	TorusMesh = CreateDefaultSubobject<UTorusMeshComponent>(TEXT("TorusMesh"));
	TorusMesh->SetupAttachment(RootComponent);
	
	TorusGravityField = CreateDefaultSubobject<UTorusGravityFieldComponent>(TEXT("TorusGravityField"));
	TorusGravityField->SetupAttachment(RootComponent);

	TorusMesh->SetCollisionProfileName(TEXT("BlockAll"));
	TorusMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

/**
 * @brief Called when the game starts or when the actor is spawned.
 *
 * @details Calls the parent BeginPlay method, synchronizes the torus mesh settings
 * and gravity field settings. Ensures both the procedural mesh and gravity field
 * are properly initialized with updated dimensions and debug visualization.
 */
void ATorusPlanet::BeginPlay()
{
	Super::BeginPlay();
	SyncTorusMeshSettings();
	SyncGravityFieldSettings();

	if (TorusMesh)
	{
		TorusMesh->GenerateTorusMesh();
	}
    
	if (TorusGravityField)
	{
		TorusGravityField->UpdateFieldDimensions();
		TorusGravityField->RedrawDebugField();
	}
}

/**
 * @brief Synchronizes the torus mesh settings with the planet configuration.
 *
 * @details Transfers the planet's torus parameters (TorusRadius, TubeRadius, segments)
 * to the procedural mesh component and regenerates the mesh. This ensures that changes
 * made to the planet's properties in the editor are reflected in the visual representation.
 * 
 * Unlike other planet types that use standard Unreal primitives, the torus requires
 * this additional synchronization step to maintain its procedurally generated geometry.
 */
void ATorusPlanet::SyncTorusMeshSettings()
{
	if (TorusMesh)
	{
		TorusMesh->TorusRadius = TorusRadius;
		TorusMesh->TubeRadius = TubeRadius;
		TorusMesh->TorusSegments = TorusSegments;
		TorusMesh->TubeSegments = TubeSegments;
		TorusMesh->GenerateTorusMesh();
	}
}

/**
 * @brief Called when the actor is placed or moved in the editor.
 *
 * @details Updates the planet's transform, synchronizes both the torus mesh settings
 * and gravity field settings. Ensures that the procedural mesh is regenerated with
 * current parameters and the gravity field dimensions are updated accordingly.
 *
 * @param Transform The new transform of the actor.
 */
void ATorusPlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SyncTorusMeshSettings();
	SyncGravityFieldSettings();

	if (TorusMesh)
	{
		TorusMesh->GenerateTorusMesh();
	}
    
	if (TorusGravityField)
	{
		TorusGravityField->UpdateFieldDimensions();
		TorusGravityField->RedrawDebugField();
	}
}

/**
 * @brief Called when a property of the torus planet is changed in the editor.
 *
 * @details Handles updates to the torus planet's properties, particularly focusing
 * on torus-specific parameters like radius and segment counts. When these properties
 * change, the procedural mesh is regenerated and the gravity field is updated.
 *
 * @param PropertyChangedEvent Information about the property that was changed.
 */
void ATorusPlanet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ATorusPlanet, TorusRadius) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(ATorusPlanet, TubeRadius) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(ATorusPlanet, TorusSegments) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(ATorusPlanet, TubeSegments))
	{
		SyncTorusMeshSettings();
		
		if (TorusGravityField)
		{
			TorusGravityField->UpdateFieldDimensions();
			TorusGravityField->RedrawDebugField();
		}
	}
}

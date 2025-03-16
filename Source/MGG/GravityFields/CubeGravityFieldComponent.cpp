#include "CubeGravityFieldComponent.h"
#include "Components/BoxComponent.h"

/**
 * @brief Constructor for the cube gravity field component.
 *
 * @details Initializes the component with a box-shaped collision volume and
 * sets up the necessary collision response settings.
 */
UCubeGravityFieldComponent::UCubeGravityFieldComponent()
{
	UBoxComponent* CubeVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("GravityVolume"));
	GravityVolume = CubeVolume;
	GravityVolume->SetupAttachment(this);

	GravityVolume->SetCollisionProfileName(TEXT("OverlapAll"));
	GravityVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GravityVolume->SetGenerateOverlapEvents(true);
	
	if (CubeVolume)
	{
		CubeVolume->SetHiddenInGame(false);
		CubeVolume->SetVisibility(true);
	}
	
	CubeVolume->SetBoxExtent(FVector(GetTotalGravityRadius()));

	GravityVolume->OnComponentBeginOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeBeginOverlap);
	GravityVolume->OnComponentEndOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeEndOverlap);
}

/**
 * @brief Draws a debug representation of the cube gravity field.
 *
 * @details Uses the debug drawer to visualize the gravity field as a cube
 * with the specified dimensions and orientation.
 */
void UCubeGravityFieldComponent::DrawDebugGravityField()
{
	UE_LOG(LogTemp, Warning, TEXT("Drawing debug cube gravity field"));
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawCube(
			CurrentDimensions.Center, 
			CurrentDimensions.Size, 
			GetComponentRotation(), 
			FColor::Red
		);
	}
}

/**
 * @brief Calculates the gravity vector for a given target location.
 *
 * @details This method implements the cube-specific gravity logic through these steps:
 * 1. Calculates the target's position relative to the cube's center
 * 2. Determines if the target is outside the cube along each axis (X, Y, Z)
 * 3. Counts the number of axes where the target is outside:
 *    - If 1 axis: target is on a face -> gravity perpendicular to that face
 *    - If 2 axes: target is on an edge -> combined gravity from two adjacent faces
 *    - If 3 axes: target is on a corner -> combined gravity from three adjacent faces
 * 4. Calculates blend factors to ensure smooth transitions between zones
 *
 * @param TargetLocation The location of the target for which to calculate gravity
 * @return The normalized gravity vector multiplied by the gravity strength
 */
FVector UCubeGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
	FVector RelativePosition = TargetLocation - CurrentDimensions.Center;

	FVector MeshSize;
	if (AActor* Owner = GetOwner())
	{
		if (UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>())
		{
			MeshSize = MeshComp->Bounds.BoxExtent;
		}
	}
	
	FCubePositionFlags Flags = CalculatePositionFlags(RelativePosition, MeshSize);
	
	int32 OutsideAxesCount = (Flags.X != FCubePositionFlags::Inside ? 1 : 0) + (Flags.Y != FCubePositionFlags::Inside ? 1 : 0) + (Flags.Z != FCubePositionFlags::Inside ? 1 : 0);
    
	FVector GravityVector;
	
	if (OutsideAxesCount == 1)
	{
		GravityVector = ConstructGravityComponentVector(Flags, FVector(1.0f));
	}
	else if (OutsideAxesCount > 1)
	{
		FVector BlendFactors = CalculateBlendFactors(RelativePosition, MeshSize, Flags);
		GravityVector = ConstructGravityComponentVector(Flags, BlendFactors);
	}
	
	return GravityVector.GetSafeNormal() * GravityStrength;
}

/**
 * @brief Calculates the dimensions of the cube gravity field.
 *
 * @details Determines the appropriate size of the gravity field based on the
 * owner's mesh and the configured influence range.
 *
 * @return A structure containing the size and center of the gravity field.
 */
UBaseGravityFieldComponent::FGravityFieldDimensions UCubeGravityFieldComponent::CalculateFieldDimensions() const
{
	FGravityFieldDimensions Dimensions;
	float Radius = 0.0f;

	if (AActor* Owner = GetOwner())
	{
		if (UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>())
		{
			FVector Extent = MeshComp->GetStaticMesh()->GetBoundingBox().GetExtent();
			FVector ActorScale = Owner->GetActorScale3D();
			Extent = Extent * ActorScale;
			Radius = FMath::Max3(Extent.X, Extent.Y, Extent.Z) + GravityInfluenceRange;
		}
	}
	
	Dimensions.Size = FVector(Radius);
	Dimensions.Center = GetComponentLocation();
    
	return Dimensions;
}

/**
 * @brief Updates the collision volume of the cube gravity field.
 *
 * @details Adjusts the box-shaped collision volume to match the current dimensions
 * and orientation of the cube gravity field.
 */
void UCubeGravityFieldComponent::UpdateGravityVolume()
{
	if (UBoxComponent* CubeVolume = Cast<UBoxComponent>(GravityVolume))
	{
		CubeVolume->SetBoxExtent(CurrentDimensions.Size);
		CubeVolume->SetWorldLocation(CurrentDimensions.Center);
		CubeVolume->SetWorldRotation(GetComponentRotation());
	}
}

/**
 * @brief Determines the position flags for a point relative to the cube.
 *
 * @details Analyzes a point's position relative to the cube along each axis (X, Y, Z)
 * and generates position flags. For each axis, the point can be:
 * - Inside: Point is between -Extent and +Extent on this axis
 * - Forward: Point is beyond +Extent on this axis
 * - Behind: Point is below -Extent on this axis
 *
 * These flags are crucial for determining which face, edge, or corner the point is closest to,
 * which directly affects the gravity direction calculation.
 *
 * @param RelativePosition The position relative to the cube's center
 * @param Extent The half-dimensions of the cube
 * @return FCubePositionFlags structure containing flags for each axis
 */
UCubeGravityFieldComponent::FCubePositionFlags UCubeGravityFieldComponent::CalculatePositionFlags(const FVector& RelativePosition, const FVector& Extent) const
{
	FCubePositionFlags Flags = {FCubePositionFlags::Inside, FCubePositionFlags::Inside, FCubePositionFlags::Inside};
    
	if (RelativePosition.X <= -Extent.X)
	{
		Flags.X = FCubePositionFlags::Behind;
	}
	else if (RelativePosition.X >= Extent.X)
	{
		Flags.X = FCubePositionFlags::Forward;
	}
    
	if (RelativePosition.Y <= -Extent.Y)
	{
		Flags.Y = FCubePositionFlags::Behind;
	}
	else if (RelativePosition.Y >= Extent.Y)
	{
		Flags.Y = FCubePositionFlags::Forward;
	}
    
	if (RelativePosition.Z <= -Extent.Z)
	{
		Flags.Z = FCubePositionFlags::Behind;
	}
	else if (RelativePosition.Z >= Extent.Z)
	{
		Flags.Z = FCubePositionFlags::Forward;
	}
    
	return Flags;
}

/**
 * @brief Calculates blend factors for smooth gravity transitions.
 *
 * @details When an object moves between different zones of the gravity field (e.g.,
 * from a face to an edge), this method calculates blend factors for each axis
 * to ensure a smooth gravity transition.
 *
 * The algorithm uses an "EdgeBlendDistance" concept that defines a transition zone
 * near the edges. The closer a point is to the center of a face, the lower the
 * factor for that axis. Conversely, the closer it is to an edge or corner,
 * the higher the factors for the corresponding axes.
 *
 * @param RelativePosition The position relative to the cube's center
 * @param Extent The half-dimensions of the cube
 * @param Flags The position flags indicating which sides of the cube the point is on
 * @return A vector containing blend factors for each axis (between 0.0 and 1.0)
 */
FVector UCubeGravityFieldComponent::CalculateBlendFactors(const FVector& RelativePosition, const FVector& Extent, const FCubePositionFlags& Flags) const
{
	const float EdgeBlendDistance = Extent.X * 0.0001f;
    
	FVector BlendFactors;
	
	if (Flags.X != FCubePositionFlags::Inside)
	{
		float Distance = FMath::Abs(RelativePosition.X) - (Extent.X - EdgeBlendDistance);
		BlendFactors.X = FMath::Clamp(Distance / EdgeBlendDistance, 0.0f, 1.0f);
	}
    
	if (Flags.Y != FCubePositionFlags::Inside)
	{
		float Distance = FMath::Abs(RelativePosition.Y) - (Extent.Y - EdgeBlendDistance);
		BlendFactors.Y = FMath::Clamp(Distance / EdgeBlendDistance, 0.0f, 1.0f);
	}
    
	if (Flags.Z != FCubePositionFlags::Inside)
	{
		float Distance = FMath::Abs(RelativePosition.Z) - (Extent.Z - EdgeBlendDistance);
		BlendFactors.Z = FMath::Clamp(Distance / EdgeBlendDistance, 0.0f, 1.0f);
	}
    
	return BlendFactors;
}

/**
 * @brief Constructs the final gravity vector based on position flags and blend factors.
 *
 * @details Creates a gravity vector pointing inward from the appropriate cube faces:
 * - For X+ face (Forward), the vector has a negative X component
 * - For X- face (Behind), the vector has a positive X component
 * - Similarly for Y and Z faces
 *
 * The blend factors are applied to each component to weight their influence
 * on the final vector. For example, near an edge, both adjacent faces contribute
 * significantly to the gravity direction.
 *
 * This approach replicates Super Mario Galaxy's behavior where gravity is always
 * perpendicular to the surface but transitions smoothly between surfaces.
 *
 * @param Flags The position flags indicating which sides of the cube the point is on
 * @param Factors The blend factors for each axis (defaults to 1.0 for all axes)
 * @return The constructed gravity vector
 */
FVector UCubeGravityFieldComponent::ConstructGravityComponentVector(const FCubePositionFlags& Flags, const FVector& Factors) const
{
	return FVector(
		Flags.X == FCubePositionFlags::Forward ? -Factors.X : Flags.X == FCubePositionFlags::Behind ? Factors.X : 0.0f,
		Flags.Y == FCubePositionFlags::Forward ? -Factors.Y : Flags.Y == FCubePositionFlags::Behind ? Factors.Y : 0.0f,
		Flags.Z == FCubePositionFlags::Forward ? -Factors.Z : Flags.Z == FCubePositionFlags::Behind ? Factors.Z : 0.0f
	);
}

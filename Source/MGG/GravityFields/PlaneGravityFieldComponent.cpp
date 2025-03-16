#include "PlaneGravityFieldComponent.h"
#include "Components/BoxComponent.h"

/**
 * @brief Constructor for the plane gravity field component.
 *
 * @details Initializes the component with a box-shaped collision volume and
 * sets up the necessary collision response settings.
 */
UPlaneGravityFieldComponent::UPlaneGravityFieldComponent()
{
	UBoxComponent* BoxVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("GravityVolume"));
	GravityVolume = BoxVolume;
	GravityVolume->SetupAttachment(this);
	
	GravityVolume->SetCollisionProfileName(TEXT("OverlapAll"));
	GravityVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GravityVolume->SetGenerateOverlapEvents(true);

	if (BoxVolume)
	{
		BoxVolume->SetHiddenInGame(false);
		BoxVolume->SetVisibility(true);
	}
	
	BoxVolume->SetBoxExtent(FVector(1000.0f, 1000.0f, 500.0f));

	GravityVolume->OnComponentBeginOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeBeginOverlap);
	GravityVolume->OnComponentEndOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeEndOverlap);
}

/**
 * @brief Draws a debug representation of the plane gravity field.
 *
 * @details Uses the debug drawer to visualize the plane with its dimensions and orientation.
 */
void UPlaneGravityFieldComponent::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawPlane(
			CurrentDimensions.Center,
			GetUpVector(),
			GetComponentRotation(),
			CurrentDimensions.Size.X,
			CurrentDimensions.Size.Z,
			FColor::Orange
		);
	}
}

/**
 * @brief Calculates the gravity vector for a given target location.
 *
 * @details For a plane gravity field, this method implements a simple but essential gravity logic:
 * - Returns a constant gravity vector in the negative direction of the plane's up vector
 * - The force is uniform throughout the entire field, regardless of target position
 * - This creates a "directional gravity" effect similar to Earth's gravity but in any orientation
 *
 * @param TargetLocation The location of the target (not used in calculation as gravity is uniform)
 * @return The gravity vector pointing in the negative up direction of the plane
 */
FVector UPlaneGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
	return -GetUpVector() * GravityStrength;
}

/**
 * @brief Calculates the dimensions of the plane gravity field.
 *
 * @details Determines the appropriate size of the gravity field based on the
 * owner's mesh and the configured influence range.
 *
 * @return A structure containing the size and center of the gravity field.
 */
UBaseGravityFieldComponent::FGravityFieldDimensions UPlaneGravityFieldComponent::CalculateFieldDimensions() const
{
	FGravityFieldDimensions Dimensions;
	
	float baseSize = 0.0f;
	if (AActor* Owner = GetOwner())
	{
		if (UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>())
		{
			FVector Extent = MeshComp->GetStaticMesh()->GetBoundingBox().GetExtent();
			FVector ActorScale = GetOwner()->GetActorScale3D();
			Extent = Extent * ActorScale;
			
			baseSize = FMath::Max(Extent.X, Extent.Y) * 2.0f;
		}
	}
    
	Dimensions.Size = FVector(baseSize, baseSize, GravityInfluenceRange);
	Dimensions.Center = GetComponentLocation();
    
	return Dimensions;
}

/**
 * @brief Updates the collision volume of the plane gravity field.
 *
 * @details Adjusts the box-shaped collision volume to match the dimensions and position of the plane:
 * 1. Calculates mesh offset to align with the owner's static mesh bounds
 * 2. Sets the box extent to half the current dimensions (required by UE box components)
 * 3. Applies rotation from the component's transform
 * 4. Calculates the final position with several offsets:
 *    - Adds a mesh-based vertical offset to position above/below the mesh appropriately
 *    - Applies a rotated offset to maintain correct orientation in all rotations
 *    - Adds an "up" vector offset to extend the gravity field in the direction of influence
 * 
 * These offsets are crucial for ensuring the gravity field extends correctly from the
 * physical surface of the plane into the space where objects will interact with it.
 */
void UPlaneGravityFieldComponent::UpdateGravityVolume()
{
	if (UBoxComponent* BoxVolume = Cast<UBoxComponent>(GravityVolume))
	{
		FVector MeshOffset = FVector::ZeroVector;
		if (AActor* Owner = GetOwner())
		{
			if (UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>())
			{
				FBoxSphereBounds bounds = MeshComp->GetStaticMesh()->GetBounds();
				MeshOffset = FVector(0.0f, 0.0f, bounds.BoxExtent.Z);
			}
		}

		FVector BoxExtent = CurrentDimensions.Size * 0.5f;
		BoxVolume->SetBoxExtent(BoxExtent);
		
		FRotator Rotation = GetComponentRotation();
		FVector RotatedOffset = Rotation.RotateVector(MeshOffset);

		FVector UpOffset = Rotation.RotateVector(FVector(0.0f, 0.0f, BoxExtent.Z));
		FVector AdjustedCenter = CurrentDimensions.Center + RotatedOffset + UpOffset;
        
		BoxVolume->SetWorldLocation(AdjustedCenter);
		BoxVolume->SetWorldRotation(Rotation);
	}
}

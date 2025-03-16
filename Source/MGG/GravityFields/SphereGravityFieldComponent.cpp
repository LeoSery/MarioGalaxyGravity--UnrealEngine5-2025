#include "SphereGravityFieldComponent.h"
#include "Components/SphereComponent.h"

/**
 * @brief Constructor for the sphere gravity field component.
 *
 * @details Initializes the component with a sphere-shaped collision volume and
 * sets up the necessary collision response settings.
 */
USphereGravityFieldComponent::USphereGravityFieldComponent()
{
	USphereComponent* SphereVolume = CreateDefaultSubobject<USphereComponent>(TEXT("GravityVolume"));
	GravityVolume = SphereVolume;
	GravityVolume->SetupAttachment(this);
    
	GravityVolume->SetCollisionProfileName(TEXT("OverlapAll"));
	GravityVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GravityVolume->SetGenerateOverlapEvents(true);

	if (SphereVolume)
	{
		SphereVolume->SetHiddenInGame(false);
		SphereVolume->SetVisibility(true);
	}

	GravityVolume->OnComponentBeginOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeBeginOverlap);
	GravityVolume->OnComponentEndOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeEndOverlap);
}

/**
 * @brief Draws a debug representation of the sphere gravity field.
 *
 * @details Uses the debug drawer to visualize the sphere with its radius and center position.
 */
void USphereGravityFieldComponent::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawSphere(CurrentDimensions.Center, CurrentDimensions.Size.X, 32, FColor::Red);
	}
}

/**
 * @brief Calculates the gravity vector for a given target location in a spherical gravity field.
 *
 * @details This method implements the most intuitive form of planetary gravity:
 * 1. Calculates the direction vector from the target to the sphere's center
 * 2. Normalizes this vector to get a unit direction
 * 3. Multiplies by the gravity strength to get the final force vector
 * 
 * This creates the classic "pull toward center" gravity effect where:
 * - Objects on the surface experience gravity perpendicular to the surface
 * - The gravity direction changes smoothly as objects move around the sphere
 * - All points at the same distance from center experience the same gravity strength
 *
 * @param TargetLocation The location of the target for which to calculate gravity
 * @return The gravity vector pointing toward the sphere's center
 */
FVector USphereGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
	FVector DirectionToCenter = GetComponentLocation() - TargetLocation;
	return DirectionToCenter.GetSafeNormal() * GravityStrength;
}

/**
 * @brief Calculates the dimensions of the sphere gravity field.
 *
 * @details Determines the appropriate radius of the gravity field based on the
 * owner's mesh and the configured influence range.
 *
 * @return A structure containing the size and center of the gravity field.
 */
UBaseGravityFieldComponent::FGravityFieldDimensions USphereGravityFieldComponent::CalculateFieldDimensions() const
{
	FGravityFieldDimensions Dimensions;
	float radius = 0.0f;
    
	if (AActor* Owner = GetOwner())
	{
		if (UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>())
		{
			FVector Extent = MeshComp->GetStaticMesh()->GetBoundingBox().GetExtent();
			FVector ActorScale = Owner->GetActorScale3D();
			Extent = Extent * ActorScale;
			radius = Extent.GetMax() + GravityInfluenceRange;
		}
	}
    
	Dimensions.Size = FVector(radius);
	Dimensions.Center = GetComponentLocation();
    
	return Dimensions;
}

/**
 * @brief Updates the collision volume of the sphere gravity field.
 *
 * @details Adjusts the sphere-shaped collision volume to match the current radius
 * and position of the gravity field.
 */
void USphereGravityFieldComponent::UpdateGravityVolume()
{
	if (USphereComponent* SphereVolume = Cast<USphereComponent>(GravityVolume))
	{
		SphereVolume->SetSphereRadius(CurrentDimensions.Size.X);
		SphereVolume->SetWorldLocation(CurrentDimensions.Center);
		SphereVolume->SetWorldRotation(GetComponentRotation());
	}
}

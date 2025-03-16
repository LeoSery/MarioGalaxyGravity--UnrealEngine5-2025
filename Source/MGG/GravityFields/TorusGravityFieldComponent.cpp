#include "TorusGravityFieldComponent.h"

#include "Components/SphereComponent.h"
#include "MGG/Planets/BasePlanet.h"
#include "MGG/Utils/MeshGenerator/TorusMeshComponent.h"

/**
 * @brief Constructor for the torus gravity field component.
 *
 * @details Initializes the component with a sphere-shaped collision volume that
 * encompasses the entire torus, and sets up the necessary collision response settings.
 */
UTorusGravityFieldComponent::UTorusGravityFieldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
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
 * @brief Draws a debug representation of the torus gravity field.
 *
 * @details Uses the debug drawer to visualize the torus with its main radius, 
 * tube radius, and orientation.
 *
 * --- Disabled for this component ---
 */
void UTorusGravityFieldComponent::DrawDebugGravityField()
{
	// if (bShowDebugField && currentDrawer && GetOwner())
	// {
	// 	UTorusMeshComponent* TorusMesh = GetOwner()->FindComponentByClass<UTorusMeshComponent>();
	// 	if (TorusMesh)
	// 	{
	// 		float TorusRadius = TorusMesh->TorusRadius;
	// 		float TubeRadius = TorusMesh->TubeRadius;
	// 		
	// 		float ScaleFactor = GetTorusScaleFactor();
	// 		
	// 		TorusRadius *= ScaleFactor;
	// 		TubeRadius *= ScaleFactor;
 //            
	// 		currentDrawer->DrawTorus(GetComponentLocation(), TorusRadius, TubeRadius, 32, FColor::Red);
	// 	}
	// }
}

/**
 * @brief Calculates the gravity vector for a given target location in a torus gravity field.
 *
 * @details This method implements the torus-specific gravity logic:
 * 1. Gets the torus parameters (center, main radius, tube radius) from the owner's mesh
 * 2. Establishes reference vectors for orientation:
 *    - 'gu' as the up vector (typically Z-axis)
 *    - 'gr' as the right vector (typically X-axis)
 * 3. Calculates the normalized direction vector from torus center to target
 * 4. Finds the point on the torus's ring that is closest to the target by:
 *    - Computing the angle between the direction vector and reference vector
 *    - Using this angle to rotate around the torus's ring
 *    - Scaling by the torus's main radius to get the exact point on the ring
 * 5. Creates a gravity vector pointing from the target toward this closest point
 * 
 * This implementation creates the distinctive "donut" gravity of a torus where:
 * - Objects are pulled toward the circular ring at the center of the torus
 * - The gravity direction continuously changes as objects move around the torus
 * - Objects can orbit inside or outside the torus following the ring's curvature
 *
 * @param TargetLocation The location of the target for which to calculate gravity
 * @return The gravity vector pointing toward the closest point on the torus's ring
 */
FVector UTorusGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
    if (AActor* Owner = GetOwner())
    {
	    if (UTorusMeshComponent* TorusMesh = Owner->FindComponentByClass<UTorusMeshComponent>())
        {
            FVector TorusCenter = GetComponentLocation();
	    	
            FVector gu = FVector(0, 0, 1);
            FVector gr = FVector(1, 0, 0);
	    	
            float BaseRadius = TorusMesh->TorusRadius;
            float ScaledRadius = BaseRadius * GetTorusScaleFactor();
	    	
            FVector V = (TargetLocation - TorusCenter).GetSafeNormal();
	    	
            float dotProduct = FVector::DotProduct(V, gr);
            dotProduct = FMath::Clamp(dotProduct, -1.0f, 1.0f);
            float angle = FMath::Acos(dotProduct);
            float sign = FMath::Sign(FVector::DotProduct(FVector::CrossProduct(V, gr), gu));
	    	
            FVector rotatedGr = gr.RotateAngleAxis(angle * sign * 180.0f / PI, gu);
            FVector P = TorusCenter + rotatedGr * ScaledRadius;
            FVector GravityDirection = (P - TargetLocation).GetSafeNormal();
            
            return GravityDirection * GravityStrength;
        }
    }
	
    return FVector(0, 0, -1) * GravityStrength;
}

/**
 * @brief Calculates the dimensions of the torus gravity field.
 *
 * @details Determines the appropriate size of the gravity field based on the
 * owner's torus mesh properties and the configured influence range.
 *
 * @return A structure containing the size and center of the gravity field.
 */
UBaseGravityFieldComponent::FGravityFieldDimensions UTorusGravityFieldComponent::CalculateFieldDimensions() const
{
	FGravityFieldDimensions Dimensions;
	
	float meshRadiusBase = 0.0f;
	float scaleFactor = GetTorusScaleFactor();
    
	if (AActor* Owner = GetOwner())
	{
		if (UTorusMeshComponent* TorusMesh = Owner->FindComponentByClass<UTorusMeshComponent>())
		{
			meshRadiusBase = TorusMesh->TorusRadius + TorusMesh->TubeRadius;
		}
	}
	
	float actualMeshRadius = meshRadiusBase * scaleFactor;
	float totalRadius = actualMeshRadius + GravityInfluenceRange;
    
	Dimensions.Size = FVector(totalRadius);
	Dimensions.Center = GetComponentLocation();
    
	return Dimensions;
}

/**
 * @brief Updates the collision volume of the torus gravity field.
 *
 * @details Adjusts the sphere-shaped collision volume to encompass the entire torus
 * plus the gravity influence range.
 */
void UTorusGravityFieldComponent::UpdateGravityVolume()
{
	if (USphereComponent* SphereVolume = Cast<USphereComponent>(GravityVolume))
	{
		if (AActor* Owner = GetOwner())
		{
			if (UTorusMeshComponent* TorusMesh = Owner->FindComponentByClass<UTorusMeshComponent>())
			{
				float MainRadius = TorusMesh->TorusRadius;
				float TubeRadius = TorusMesh->TubeRadius;
				float TotalRadius = (MainRadius + TubeRadius) * GetTorusScaleFactor() + GravityInfluenceRange;
                
				SphereVolume->SetSphereRadius(TotalRadius);
				SphereVolume->SetWorldLocation(GetComponentLocation());
			}
			else
			{
				SphereVolume->SetSphereRadius(CurrentDimensions.Size.X);
				SphereVolume->SetWorldLocation(CurrentDimensions.Center);
			}
		}
	}
}

/**
 * @brief Gets the scale factor for the torus based on the planet's radius.
 *
 * @details Unlike other planet shapes that use Unreal's built-in primitives (cube, sphere),
 * the torus uses a procedurally generated mesh created at runtime. This requires special 
 * handling for scaling:
 * 
 * 1. The procedural torus mesh is initially created with standard dimensions (TorusRadius, TubeRadius)
 * 2. These values need to be scaled to match the actual desired planet size
 * 3. This method retrieves the scale factor from the owner planet (PlanetRadius/100.0f)
 * 4. This factor is then applied to all torus calculations including:
 *    - Mesh generation dimensions
 *    - Gravity field calculations
 *    - Collision volume sizing
 * 
 * Without this scale factor, the procedural torus would not maintain consistent
 * proportions with other planet types in the game, and gravity calculations would
 * not align properly with the visual representation.
 *
 * @return The scale factor to apply to the torus dimensions based on the planet radius.
 */
float UTorusGravityFieldComponent::GetTorusScaleFactor() const
{
	float ScaleFactor = 1.0f;
	if (AActor* Owner = GetOwner())
	{
		if (ABasePlanet* Planet = Cast<ABasePlanet>(Owner))
		{
			ScaleFactor = Planet->PlanetRadius / 100.0f;
		}
	}
	return ScaleFactor;
}

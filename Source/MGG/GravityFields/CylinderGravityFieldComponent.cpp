#include "CylinderGravityFieldComponent.h"
#include "Components/CapsuleComponent.h"

/**
 * @brief Constructor for the cylinder gravity field component.
 *
 * @details Initializes the component with a capsule-shaped collision volume and
 * sets up the necessary collision response settings.
 */
UCylinderGravityFieldComponent::UCylinderGravityFieldComponent()
{
    UCapsuleComponent* CapsuleVolume = CreateDefaultSubobject<UCapsuleComponent>(TEXT("GravityVolume"));
    GravityVolume = CapsuleVolume;
    GravityVolume->SetupAttachment(this);
    
    GravityVolume->SetCollisionProfileName(TEXT("OverlapAll"));
    GravityVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GravityVolume->SetGenerateOverlapEvents(true);

    if (CapsuleVolume)
    {
        CapsuleVolume->SetHiddenInGame(false);
        CapsuleVolume->SetVisibility(true);
    }

    GravityVolume->OnComponentBeginOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeBeginOverlap);
    GravityVolume->OnComponentEndOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeEndOverlap);
}

/**
 * @brief Draws a debug representation of the cylinder gravity field.
 *
 * @details Renders a visual representation of the cylinder gravity field using debug lines,
 * displaying the extent and orientation of the field in the game world.
 *
 *  --- Currently disabled for this component ---
 */
void UCylinderGravityFieldComponent::DrawDebugGravityField()
{
    // if (bShowDebugField && currentDrawer)
    // {
    //     currentDrawer->DrawCylinder(
    //         CurrentDimensions.Center,
    //         GetUpVector(),
    //         CurrentDimensions.Size.X,
    //         CylinderHeight,
    //         16,
    //         FColor::Magenta
    //     );
    // }
}

/**
 * @brief Calculates the gravity vector for a given target location in a cylindrical gravity field.
 *
 * @details This method implements the cylinder-specific gravity logic based on the target's position:
 * 1. Calculates the vector from the cylinder's center to the target
 * 2. Projects this vector onto the cylinder's up vector to determine the target's height relative to the center
 * 3. Checks if the target is beyond the cylinder's half-height:
 *    - If above the top: gravity points downward (toward the top face)
 *    - If below the bottom: gravity points upward (toward the bottom face)
 *    - If within the cylinder's height: calculates the nearest point on the central axis, then
 *      creates a radial gravity vector pointing from the target to this point
 *
 * This implementation creates a cylindrical planet where objects stick to the sides with gravity
 * always pointing toward the central axis, while objects on the top or bottom experience
 * planar gravity perpendicular to the flat faces.
 *
 * @param TargetLocation The location of the target for which to calculate gravity
 * @return The gravity vector calculated based on the target's position relative to the cylinder
 */
FVector UCylinderGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
    FVector CylinderCenter = GetComponentLocation();
    FVector UpVector = GetUpVector();
    FVector CenterToTarget = TargetLocation - CylinderCenter;
    
    float ProjectionLength = FVector::DotProduct(CenterToTarget, UpVector);
    float HalfHeight = CylinderHeight * 0.5f;

    // Plane gravity ( top and bottom )
    if (FMath::Abs(ProjectionLength) > HalfHeight)
    {
        if (ProjectionLength > 0)
        {
            // Bottom
            return -UpVector * GravityStrength;
        }
        else
        {
            // Top
            return UpVector * GravityStrength;
        }
    }
    else
    {
        // Radial gravity ( side )
        FVector PointOnAxis = CylinderCenter + UpVector * ProjectionLength;
        FVector RadialVector = TargetLocation - PointOnAxis;
        
        if (RadialVector.IsNearlyZero())
        {
            FVector ArbitraryDir = FMath::Abs(UpVector.Z) < 0.9f ? FVector(0, 0, 1) : FVector(1, 0, 0);
            RadialVector = FVector::CrossProduct(UpVector, ArbitraryDir).GetSafeNormal();
        }
        
        return -RadialVector.GetSafeNormal() * GravityStrength;
    }
}

/**
 * @brief Calculates the dimensions of the cylinder gravity field.
 *
 * @details Determines the appropriate size of the gravity field based on the
 * owner's mesh and the configured influence range.
 *
 * @return A structure containing the size and center of the gravity field.
 */
UBaseGravityFieldComponent::FGravityFieldDimensions UCylinderGravityFieldComponent::CalculateFieldDimensions() const
{
    FGravityFieldDimensions Dimensions;
    float radius = 100.0f;;
    
    if (AActor* Owner = GetOwner())
    {
        if (UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>())
        {
            if (MeshComp->GetStaticMesh())
            {
                FVector Extent = MeshComp->GetStaticMesh()->GetBoundingBox().GetExtent();
                FVector ActorScale = Owner->GetActorScale3D();
                Extent = Extent * ActorScale;
                radius = FMath::Max(Extent.X, Extent.Y) + GravityInfluenceRange;
            }
            else
            {
                radius = 100.0f + GravityInfluenceRange;
            }
        }
    }
    
    Dimensions.Size = FVector(radius);
    Dimensions.Center = GetComponentLocation();
    
    return Dimensions;
}

/**
 * @brief Updates the collision volume of the cylinder gravity field.
 *
 * @details Adjusts the capsule-shaped collision volume to match the current dimensions
 * of the cylinder gravity field.
 */
void UCylinderGravityFieldComponent::UpdateGravityVolume()
{
    if (UCapsuleComponent* CapsuleVolume = Cast<UCapsuleComponent>(GravityVolume))
    {
        CapsuleVolume->SetCapsuleRadius(CurrentDimensions.Size.X);
        CapsuleVolume->SetCapsuleHalfHeight(CylinderHeight * 0.5f + CurrentDimensions.Size.X);
        CapsuleVolume->SetWorldLocation(CurrentDimensions.Center);
        CapsuleVolume->SetWorldRotation(GetComponentRotation());
    }
}
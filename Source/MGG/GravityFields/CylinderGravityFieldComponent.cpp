#include "CylinderGravityFieldComponent.h"
#include "Components/CapsuleComponent.h"

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
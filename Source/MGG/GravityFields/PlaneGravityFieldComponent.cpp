#include "PlaneGravityFieldComponent.h"

UPlaneGravityFieldComponent::UPlaneGravityFieldComponent()
{
	
}

void UPlaneGravityFieldComponent::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		float baseSize = 0.0f;
		if (AActor* Owner = GetOwner())
		{
			if (UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>())
			{
				FVector Extent = MeshComp->Bounds.BoxExtent;
				baseSize = FMath::Max(Extent.X, Extent.Y) * 2.0f;
			}
		}
		
		float height = GravityInfluenceRange;
		
		currentDrawer->DrawPlane(GetComponentLocation(), GetUpVector(), baseSize, height, FColor::Red);
	}
}

void UPlaneGravityFieldComponent::ApplyGravity()
{
	// TODO: Implement custom gravity field
}
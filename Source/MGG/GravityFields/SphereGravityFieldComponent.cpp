#include "SphereGravityFieldComponent.h"

USphereGravityFieldComponent::USphereGravityFieldComponent()
{
	
}

void USphereGravityFieldComponent::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawSphere(GetComponentLocation(), GetTotalGravityRadius(), 32, FColor::Red);
	}
}

// Sphere gravity field is not implemented yet
FVector USphereGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
	return FVector::ZeroVector;
}

UBaseGravityFieldComponent::FGravityFieldDimensions USphereGravityFieldComponent::CalculateFieldDimensions() const
{
	return FGravityFieldDimensions();
}

void USphereGravityFieldComponent::UpdateGravityVolume()
{
	
}

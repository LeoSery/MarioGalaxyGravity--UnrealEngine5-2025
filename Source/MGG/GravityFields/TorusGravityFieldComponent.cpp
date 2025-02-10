#include "TorusGravityFieldComponent.h"

UTorusGravityFieldComponent::UTorusGravityFieldComponent()
{
	
}

void UTorusGravityFieldComponent::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawTorus(GetComponentLocation(), GetTotalGravityRadius(), GetTotalGravityRadius() * 0.2f, 32, FColor::Red);
	}
}

// Torus gravity field is not implemented yet
FVector UTorusGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
	return FVector::ZeroVector;
}

UBaseGravityFieldComponent::FGravityFieldDimensions UTorusGravityFieldComponent::CalculateFieldDimensions() const
{
	return FGravityFieldDimensions();
}

void UTorusGravityFieldComponent::UpdateGravityVolume()
{
	
}

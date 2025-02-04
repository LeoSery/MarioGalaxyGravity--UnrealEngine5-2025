#include "PlaneGravityFieldComponent.h"

UPlaneGravityFieldComponent::UPlaneGravityFieldComponent()
{
	GravityType = PLANE;
}

void UPlaneGravityFieldComponent::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawPlane(GetComponentLocation(), GetUpVector(), GravityRadius * 2, FColor::Red);
	}
}

void UPlaneGravityFieldComponent::ApplyGravity()
{
	// TODO: Implement custom gravity field
}
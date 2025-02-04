#include "SphereGravityFieldComponent.h"

USphereGravityFieldComponent::USphereGravityFieldComponent()
{
	GravityType = SPHERE;
}

void USphereGravityFieldComponent::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawSphere(GetComponentLocation(), GravityRadius, 32, FColor::Red);
	}
}

void USphereGravityFieldComponent::ApplyGravity()
{
	// TODO: Implement custom gravity field
}
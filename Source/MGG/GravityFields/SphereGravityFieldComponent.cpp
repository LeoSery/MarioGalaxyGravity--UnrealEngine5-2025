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

void USphereGravityFieldComponent::ApplyGravity()
{
	// TODO: Implement custom gravity field
}
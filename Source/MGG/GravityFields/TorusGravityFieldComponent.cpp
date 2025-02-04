#include "TorusGravityFieldComponent.h"

UTorusGravityFieldComponent::UTorusGravityFieldComponent()
{
	GravityType = TORUS;
}

void UTorusGravityFieldComponent::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawTorus(GetComponentLocation(), GravityRadius, GravityRadius * 0.2f, 32, FColor::Red);
	}
}

void UTorusGravityFieldComponent::ApplyGravity()
{
	// TODO: Implement custom gravity field
}

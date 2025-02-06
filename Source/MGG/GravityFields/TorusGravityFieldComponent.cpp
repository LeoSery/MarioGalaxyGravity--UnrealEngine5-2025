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

void UTorusGravityFieldComponent::ApplyGravity()
{
	// TODO: Implement custom gravity field
}

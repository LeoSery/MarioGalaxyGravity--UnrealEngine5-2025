#include "CubeGravityFieldComponent.h"

UCubeGravityFieldComponent::UCubeGravityFieldComponent()
{
	GravityType = CUBE;
}

void UCubeGravityFieldComponent::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawCube(GetComponentLocation(), FVector(GravityRadius), GetComponentRotation(), FColor::Red );
	}
}

void UCubeGravityFieldComponent::ApplyGravity()
{
	// TODO: Implement custom gravity field
}
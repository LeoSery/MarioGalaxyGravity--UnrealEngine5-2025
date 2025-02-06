#include "CubeGravityFieldComponent.h"

UCubeGravityFieldComponent::UCubeGravityFieldComponent()
{
	
}

void UCubeGravityFieldComponent::DrawDebugGravityField()
{
	UE_LOG(LogTemp, Warning, TEXT("Drawing debug cube gravity field"));
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawCube(GetComponentLocation(), FVector( GetTotalGravityRadius()), GetComponentRotation(), FColor::Red );
	}
}

void UCubeGravityFieldComponent::ApplyGravity()
{
	// TODO: Implement custom gravity field
}
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

// Cube gravity field is not implemented yet 
FVector UCubeGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
	return FVector::ZeroVector;
}

UBaseGravityFieldComponent::FGravityFieldDimensions UCubeGravityFieldComponent::CalculateFieldDimensions() const
{
	return FGravityFieldDimensions();
}

void UCubeGravityFieldComponent::UpdateGravityVolume()
{

}

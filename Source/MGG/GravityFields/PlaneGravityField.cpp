#include "PlaneGravityField.h"
#include "DrawDebugHelpers.h"

APlaneGravityField::APlaneGravityField()
{
	GravityType = PLANE;
}

void APlaneGravityField::BeginPlay()
{
	Super::BeginPlay();
}

void APlaneGravityField::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawPlane(GetActorLocation(), GetActorUpVector(), GravityRadius * 2, FColor::Red);
	}
}

void APlaneGravityField::ApplyGravity()
{
	// TODO: Implement custom gravity field
}

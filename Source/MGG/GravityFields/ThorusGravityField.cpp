#include "ThorusGravityField.h"
#include "DrawDebugHelpers.h"

AThorusGravityField::AThorusGravityField()
{
	GravityType = TORUS;
}

void AThorusGravityField::BeginPlay()
{
	Super::BeginPlay();
}

void AThorusGravityField::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawTorus(GetActorLocation(), GravityRadius, GravityRadius * 0.2f, 32, FColor::Red);
	}
}

void AThorusGravityField::ApplyGravity()
{
	// TODO: Implement custom gravity field
}
#include "SphereGravityField.h"
#include "DrawDebugHelpers.h"

ASphereGravityField::ASphereGravityField()
{
	GravityType = SPHERE;
}

void ASphereGravityField::BeginPlay()
{
	Super::BeginPlay();
}


void ASphereGravityField::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawSphere(GetActorLocation(), GravityRadius, 32, FColor::Red);
	}
}

void ASphereGravityField::ApplyGravity()
{
	// TODO: Implement custom gravity field
}

#include "CubeGravityField.h"
#include "DrawDebugHelpers.h"

ACubeGravityField::ACubeGravityField()
{
	GravityType = CUBE;
}

void ACubeGravityField::BeginPlay()
{
	Super::BeginPlay();
}

void ACubeGravityField::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawCube(GetActorLocation(), FVector(GravityRadius), GetActorRotation(), FColor::Red);
	}
}

void ACubeGravityField::ApplyGravity()
{
	// TODO: Implement custom gravity field
}
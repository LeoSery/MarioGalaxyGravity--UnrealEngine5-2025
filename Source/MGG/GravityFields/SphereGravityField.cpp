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

void ASphereGravityField::ApplyGravity()
{
	// TODO: Implémenter la gravité sphérique
}

void ASphereGravityField::DrawDebugGravityField()
{
	if (bShowDebugField)
	{
		DrawDebugSphere(
			GetWorld(),
			GetActorLocation(),
			GravityRadius,
			32,
			FColor::Blue,
			true,
			-1.0f,
			0,
			2.0f
		);
	}
}
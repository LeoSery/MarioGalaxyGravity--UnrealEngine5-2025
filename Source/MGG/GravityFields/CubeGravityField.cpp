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
	if (bShowDebugField)
	{
		FVector Extent(GravityRadius);
		DrawDebugBox(
			GetWorld(),
			GetActorLocation(),
			Extent,
			GetActorRotation().Quaternion(),
			FColor::Blue,
			true,
			-1.0f,
			0,
			2.0f
		);
	}
}

void ACubeGravityField::ApplyGravity()
{
	// TODO: Implémenter la gravité cubique
}
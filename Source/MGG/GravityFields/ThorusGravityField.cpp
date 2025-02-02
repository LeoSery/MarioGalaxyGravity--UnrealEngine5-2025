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
	if (bShowDebugField)
	{
		FVector Center = GetActorLocation();
		float TorusRadius = GravityRadius;
		float TubeRadius = GravityRadius * 0.2f;
		int32 Segments = 32;
		int32 TubeSegments = 16;

		for(int32 i = 0; i < Segments; i++)
		{
			float Angle = (2.0f * PI * i) / Segments;
			float NextAngle = (2.0f * PI * (i + 1)) / Segments;
            
			FVector Start = Center + FVector(
				TorusRadius * FMath::Cos(Angle),
				TorusRadius * FMath::Sin(Angle),
				0.0f
			);
            
			FVector End = Center + FVector(
				TorusRadius * FMath::Cos(NextAngle),
				TorusRadius * FMath::Sin(NextAngle),
				0.0f
			);
            
			DrawDebugLine(
				GetWorld(), 
				Start, 
				End, 
				FColor::Blue, 
				true,
				-1.0f,
				0,
				2.0f
			);

			DrawDebugCircle(
				GetWorld(),
				Start,
				TubeRadius,
				TubeSegments,
				FColor::Blue,
				true,
				-1.0f,
				0,
				2.0f,
				FVector(1,0,0),
				FVector(0,1,0)
			);
		}
	}
}

void AThorusGravityField::ApplyGravity()
{
	// TODO: Implémenter la gravité torique
}
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
	if (bShowDebugField)
	{
		FVector Center = GetActorLocation();
		FVector Normal = GetActorUpVector();
		float Size = GravityRadius;

		for(int32 i = -5; i <= 5; i++)
		{
			FVector Start1 = Center + (GetActorRightVector() * i * Size/5) - (GetActorForwardVector() * Size);
			FVector End1 = Center + (GetActorRightVector() * i * Size/5) + (GetActorForwardVector() * Size);
            
			FVector Start2 = Center + (GetActorForwardVector() * i * Size/5) - (GetActorRightVector() * Size);
			FVector End2 = Center + (GetActorForwardVector() * i * Size/5) + (GetActorRightVector() * Size);
            
			DrawDebugLine(
				GetWorld(), 
				Start1, 
				End1, 
				FColor::Blue, 
				true,
				-1.0f,
				0,
				2.0f 
			);
			DrawDebugLine(
				GetWorld(), 
				Start2, 
				End2, 
				FColor::Blue, 
				true,
				-1.0f,
				0,
				2.0f
			);
		}

		DrawDebugDirectionalArrow(
			GetWorld(),
			Center,
			Center + Normal * 100.0f,
			50.0f,
			FColor::Red,
			true,
			-1.0f,
			0,
			2.0f
		);
	}
}

void APlaneGravityField::ApplyGravity()
{
	// TODO: Implémenter la gravité plane
}
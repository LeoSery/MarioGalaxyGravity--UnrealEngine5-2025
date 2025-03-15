#pragma once

#include "CoreMinimal.h"
#include "Components/LineBatchComponent.h"

class MGG_API GravityFieldDrawer
{
public:
	GravityFieldDrawer(ULineBatchComponent* InDebugLines);

	void DrawSphere(const FVector& Center, float Radius, int32 Segments, const FColor& Color);
	void DrawCube(const FVector& Center, const FVector& Extent, const FRotator& Rotation, const FColor& Color);
	void DrawTorus(const FVector& Center, float TorusRadius, float TubeRadius, int32 Segments, const FColor& Color);
	void DrawPlane(const FVector& Center, const FVector& Normal, const FRotator& Rotation, float Size, float Height, const FColor& Color);

private:
	FORCEINLINE void DrawLine(const FVector& Start, const FVector& End, const FColor& Color)
	{
		if (DebugLines)
		{
			DebugLines->DrawLine(Start, End, Color, 0, 5.0f);
		}
	}
    
	UPROPERTY()
	ULineBatchComponent* DebugLines;
	
};

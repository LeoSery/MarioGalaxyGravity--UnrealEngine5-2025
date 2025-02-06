#pragma once

#include "CoreMinimal.h"

class ULineBatchComponent;

class MGG_API GravityFieldDrawer
{
public:
	GravityFieldDrawer(ULineBatchComponent* InDebugLines);

	void DrawSphere(const FVector& Center, float Radius, int32 Segments, const FColor& Color);
	void DrawCube(const FVector& Center, const FVector& Extent, const FRotator& Rotation, const FColor& Color);
	void DrawTorus(const FVector& Center, float TorusRadius, float TubeRadius, int32 Segments, const FColor& Color);
	
	void DrawPlane(const FVector& Center, const FVector& Normal, float Size, const FColor& Color);
	void DrawPlane(const FVector& Center, const FVector& Normal, float Size, float Height, const FColor& Color);

private:
	void DrawLine(const FVector& Start, const FVector& End, const FColor& Color);
    
	UPROPERTY()
	ULineBatchComponent* DebugLines;
	
};

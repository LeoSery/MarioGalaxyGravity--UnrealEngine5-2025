#pragma once

#include "CoreMinimal.h"
#include "Components/LineBatchComponent.h"

class MGG_API GravityFieldDrawer
{
public:
	//////// CONSTRUCTOR ////////
	GravityFieldDrawer(ULineBatchComponent* InDebugLines);

	//////// METHODS ////////
	//// Drawing methods
	void DrawSphere(const FVector& Center, float Radius, int32 Segments, const FColor& Color);
	void DrawCube(const FVector& Center, const FVector& Extent, const FRotator& Rotation, const FColor& Color);
	void DrawTorus(const FVector& Center, float TorusRadius, float TubeRadius, int32 Segments, const FColor& Color);
	void DrawPlane(const FVector& Center, const FVector& Normal, const FRotator& Rotation, float Size, float Height, const FColor& Color);

private:
	//////// INLINE METHODS ////////
	//// Helper methods
	
	/**
	 * @brief Draws a line between two points for internal use.
	 *
	 * @details Helper method that forwards line drawing requests to the ULineBatchComponent.
	 * Used by all other drawing methods to create their wireframe representations.
	 *
	 * @param Start The start position of the line.
	 * @param End The end position of the line.
	 * @param Color The color to use for drawing the line.
	 */
	FORCEINLINE void DrawLine(const FVector& Start, const FVector& End, const FColor& Color)
	{
		if (DebugLines)
		{
			DebugLines->DrawLine(Start, End, Color, 0, 5.0f);
		}
	}

	//////// FIELDS ////////
	//// Component fields
	UPROPERTY()
	ULineBatchComponent* DebugLines;
	
};

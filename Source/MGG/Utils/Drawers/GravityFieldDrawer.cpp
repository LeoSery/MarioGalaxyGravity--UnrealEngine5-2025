#include "GravityFieldDrawer.h"
#include "Components/LineBatchComponent.h"
#include "Kismet/KismetMathLibrary.h"

/**
 * @brief Constructor for the gravity field drawer.
 *
 * @details Initializes the drawer with a reference to the debug line component
 * that will be used for rendering visualization elements.
 *
 * @param InDebugLines The line batch component to use for debug drawing.
 */
GravityFieldDrawer::GravityFieldDrawer(ULineBatchComponent* InDebugLines) : DebugLines(InDebugLines)
{
	
}

/**
 * @brief Draws a sphere shape for debug visualization.
 *
 * @details Creates a wireframe representation of a sphere by drawing multiple
 * circular cross-sections and longitude lines. The implementation:
 * 1. Draws latitude circles at different heights along the sphere
 * 2. Draws longitude lines from pole to pole
 * 3. Uses the specified segments count to determine resolution
 *
 * @param Center The center position of the sphere.
 * @param Radius The radius of the sphere.
 * @param Segments The number of segments to use for the sphere (higher = smoother).
 * @param Color The color to use for drawing the sphere.
 */
void GravityFieldDrawer::DrawSphere(const FVector& Center, float Radius, int32 Segments, const FColor& Color)
{
    const int32 NumLatitudes = 5;
    const float LatitudeStep = PI / (NumLatitudes + 1);

	const int32 NumLongitudes = 8;
	const float LongitudeStep = (2.0f * PI) / NumLongitudes;
    
    for(int32 lat = 1; lat <= NumLatitudes; lat++)
    {
        float phi = LatitudeStep * lat;
        float currentRadius = Radius * FMath::Sin(phi);
        float z = Radius * FMath::Cos(phi);
        
        for(int32 i = 0; i < Segments; i++)
        {
            float angle = (2.0f * PI * i) / Segments;
            float nextAngle = (2.0f * PI * (i + 1)) / Segments;
            
            FVector start(
                currentRadius * FMath::Cos(angle),
                currentRadius * FMath::Sin(angle),
                z
            );
            
            FVector end(
                currentRadius * FMath::Cos(nextAngle),
                currentRadius * FMath::Sin(nextAngle),
                z
            );
            
            DrawLine(Center + start, Center + end, Color);
        }
    }
	
    for(int32 lon = 0; lon < NumLongitudes; lon++)
    {
        float theta = LongitudeStep * lon;
        
        for(int32 i = 0; i < Segments; i++)
        {
            float phi = (PI * i) / Segments;
            float nextPhi = (PI * (i + 1)) / Segments;
            
            FVector start(
                Radius * FMath::Sin(phi) * FMath::Cos(theta),
                Radius * FMath::Sin(phi) * FMath::Sin(theta),
                Radius * FMath::Cos(phi)
            );
            
            FVector end(
                Radius * FMath::Sin(nextPhi) * FMath::Cos(theta),
                Radius * FMath::Sin(nextPhi) * FMath::Sin(theta),
                Radius * FMath::Cos(nextPhi)
            );
            
            DrawLine(Center + start, Center + end, Color);
        }
    }
}

/**
 * @brief Draws a cube shape for debug visualization.
 *
 * @details Creates a wireframe representation of a cube by drawing 12 lines
 * connecting the 8 corners of the cube. The implementation:
 * 1. Calculates the 8 corner points based on center, extent and rotation
 * 2. Draws lines for the front face, back face, and connections between them
 * 3. Applies the specified rotation to orient the cube correctly
 *
 * @param Center The center position of the cube.
 * @param Extent The extent (half-dimensions) of the cube.
 * @param Rotation The rotation of the cube.
 * @param Color The color to use for drawing the cube.
 */
void GravityFieldDrawer::DrawCube(const FVector& Center, const FVector& Extent, const FRotator& Rotation, const FColor& Color)
{
	const FVector Forward = Rotation.Vector();
	const FVector Right = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
	const FVector Up = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Z);

	// Points du cube
	const FVector Points[] = {
		Center + (Forward * Extent.X + Right * Extent.Y + Up * Extent.Z),
		Center + (Forward * Extent.X + Right * Extent.Y - Up * Extent.Z),
		Center + (Forward * Extent.X - Right * Extent.Y + Up * Extent.Z),
		Center + (Forward * Extent.X - Right * Extent.Y - Up * Extent.Z),
		Center + (-Forward * Extent.X + Right * Extent.Y + Up * Extent.Z),
		Center + (-Forward * Extent.X + Right * Extent.Y - Up * Extent.Z),
		Center + (-Forward * Extent.X - Right * Extent.Y + Up * Extent.Z),
		Center + (-Forward * Extent.X - Right * Extent.Y - Up * Extent.Z)
	};

	// Face avant
	DrawLine(Points[0], Points[1], Color);
	DrawLine(Points[1], Points[3], Color);
	DrawLine(Points[3], Points[2], Color);
	DrawLine(Points[2], Points[0], Color);

	// Face arrière
	DrawLine(Points[4], Points[5], Color);
	DrawLine(Points[5], Points[7], Color);
	DrawLine(Points[7], Points[6], Color);
	DrawLine(Points[6], Points[4], Color);

	// Connections
	DrawLine(Points[0], Points[4], Color);
	DrawLine(Points[1], Points[5], Color);
	DrawLine(Points[2], Points[6], Color);
	DrawLine(Points[3], Points[7], Color);
}

/**
 * @brief Draws a torus shape for debug visualization.
 *
 * @details Creates a wireframe representation of a torus by drawing circles
 * along the main ring. The implementation:
 * 1. Draws concentric circles around the torus's main ring
 * 2. Draws connecting lines between points on adjacent circles
 * 3. Uses the specified segments count to determine resolution
 *
 * @param Center The center position of the torus.
 * @param TorusRadius The main radius of the torus (distance from center to ring center).
 * @param TubeRadius The tube radius of the torus (thickness of the ring).
 * @param Segments The number of segments to use for the torus (higher = smoother).
 * @param Color The color to use for drawing the torus.
 */
void GravityFieldDrawer::DrawTorus(const FVector& Center, float TorusRadius, float TubeRadius, int32 Segments, const FColor& Color)
{
    const int32 MainSegments = 16;
    const int32 TubeSegments = 8;
    const float MainStep = (2.0f * PI) / MainSegments;
    const float TubeStep = (2.0f * PI) / TubeSegments;
	
    for (int32 i = 0; i < MainSegments; i++)
    {
        float Angle = MainStep * i;
        float NextAngle = MainStep * (i + 1);
    	
        for (int32 j = 0; j < TubeSegments; j++)
        {
            float TubeAngle = TubeStep * j;
            float NextTubeAngle = TubeStep * (j + 1);
        	
            FVector Current = FVector(
                (TorusRadius + TubeRadius * FMath::Cos(TubeAngle)) * FMath::Cos(Angle),
                (TorusRadius + TubeRadius * FMath::Cos(TubeAngle)) * FMath::Sin(Angle),
                TubeRadius * FMath::Sin(TubeAngle)
            );

            FVector NextOnTube = FVector(
                (TorusRadius + TubeRadius * FMath::Cos(NextTubeAngle)) * FMath::Cos(Angle),
                (TorusRadius + TubeRadius * FMath::Cos(NextTubeAngle)) * FMath::Sin(Angle),
                TubeRadius * FMath::Sin(NextTubeAngle)
            );

            FVector NextOnMain = FVector(
                (TorusRadius + TubeRadius * FMath::Cos(TubeAngle)) * FMath::Cos(NextAngle),
                (TorusRadius + TubeRadius * FMath::Cos(TubeAngle)) * FMath::Sin(NextAngle),
                TubeRadius * FMath::Sin(TubeAngle)
            );
        	
            DrawLine(Center + Current, Center + NextOnTube, Color);
            DrawLine(Center + Current, Center + NextOnMain, Color);
        }
    }
}

/**
 * @brief Draws a plane shape for debug visualization.
 *
 * @details Creates a wireframe representation of a plane with a volume above it. The implementation:
 * 1. Draws a grid of lines on the base plane to represent its surface
 * 2. Draws vertical lines from the corners of the base to the top plane
 * 3. Draws the top plane outline to form a complete volume
 * 4. Adds a normal vector indicator to show the plane's orientation
 *
 * @param Center The center position of the plane.
 * @param Normal The normal vector of the plane.
 * @param Rotation The rotation of the plane.
 * @param Size The width/length of the plane.
 * @param Height The height of the volume above the plane.
 * @param Color The color to use for drawing the plane.
 */
void GravityFieldDrawer::DrawPlane(const FVector& Center, const FVector& Normal, const FRotator& Rotation, float Size, float Height, const FColor& Color)
{
	FRotationMatrix RotationMatrix(Rotation);
	FVector AxisX = RotationMatrix.GetScaledAxis(EAxis::X);
	FVector AxisY = RotationMatrix.GetScaledAxis(EAxis::Y);
    
	const int32 NumLines = 10;
	const float Step = Size / float(NumLines);
    
	FVector VolumeBase = Center;
	
    for(int32 i = -NumLines/2; i <= NumLines/2; i++)
    {
        // X lines
        DrawLine(
            VolumeBase + AxisX * Size * 0.5f + AxisY * (Step * i),
            VolumeBase - AxisX * Size * 0.5f + AxisY * (Step * i),
            Color
        );
        
        // Y lines
        DrawLine(
            VolumeBase + AxisY * Size * 0.5f + AxisX * (Step * i),
            VolumeBase - AxisY * Size * 0.5f + AxisX * (Step * i),
            Color
        );
    }
	
    FVector TopCenter = VolumeBase + Normal * Height;
    
    // Corner vertical lines
    DrawLine(VolumeBase + AxisX * Size * 0.5f + AxisY * Size * 0.5f, 
             TopCenter + AxisX * Size * 0.5f + AxisY * Size * 0.5f, Color);
             
    DrawLine(VolumeBase + AxisX * Size * 0.5f - AxisY * Size * 0.5f,
             TopCenter + AxisX * Size * 0.5f - AxisY * Size * 0.5f, Color);
             
    DrawLine(VolumeBase - AxisX * Size * 0.5f + AxisY * Size * 0.5f,
             TopCenter - AxisX * Size * 0.5f + AxisY * Size * 0.5f, Color);
             
    DrawLine(VolumeBase - AxisX * Size * 0.5f - AxisY * Size * 0.5f,
             TopCenter - AxisX * Size * 0.5f - AxisY * Size * 0.5f, Color);
    
    // Volume Box
    DrawLine(TopCenter + AxisX * Size * 0.5f + AxisY * Size * 0.5f,
             TopCenter + AxisX * Size * 0.5f - AxisY * Size * 0.5f, Color);
             
    DrawLine(TopCenter + AxisX * Size * 0.5f - AxisY * Size * 0.5f,
             TopCenter - AxisX * Size * 0.5f - AxisY * Size * 0.5f, Color);
             
    DrawLine(TopCenter - AxisX * Size * 0.5f - AxisY * Size * 0.5f,
             TopCenter - AxisX * Size * 0.5f + AxisY * Size * 0.5f, Color);
             
    DrawLine(TopCenter - AxisX * Size * 0.5f + AxisY * Size * 0.5f,
             TopCenter + AxisX * Size * 0.5f + AxisY * Size * 0.5f, Color);

    // Norml arrow
    DrawLine(VolumeBase, VolumeBase + Normal * (Size * 0.2f), FColor::Red);
}

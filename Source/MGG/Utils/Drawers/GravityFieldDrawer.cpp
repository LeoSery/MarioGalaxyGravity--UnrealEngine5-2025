#include "GravityFieldDrawer.h"
#include "Components/LineBatchComponent.h"

GravityFieldDrawer::GravityFieldDrawer(ULineBatchComponent* InDebugLines) : DebugLines(InDebugLines)
{
	
}

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

void GravityFieldDrawer::DrawPlane(const FVector& Center, const FVector& Normal, float Size, const FColor& Color)
{
    FVector AxisX, AxisY;
    Normal.FindBestAxisVectors(AxisX, AxisY);
    
    const int32 NumLines = 10;
    const float Step = Size / float(NumLines);
    const float Height = Size;
	
    FVector VolumeCenter = Center;
    FVector GridCenter = Center - (Normal * (Height * 0.5f));
    
    // Dessine la grille au sol en utilisant GridCenter
    for(int32 i = -NumLines/2; i <= NumLines/2; i++)
    {
        DrawLine(
            GridCenter + AxisX * Size * 0.5f + AxisY * (Step * i),
            GridCenter - AxisX * Size * 0.5f + AxisY * (Step * i),
            Color
        );
        
        DrawLine(
            GridCenter + AxisY * Size * 0.5f + AxisX * (Step * i),
            GridCenter - AxisY * Size * 0.5f + AxisX * (Step * i),
            Color
        );
    }
	
    FVector TopCenter = VolumeCenter + Normal * (Height * 0.5f);
    FVector BottomCenter = VolumeCenter - Normal * (Height * 0.5f);
    
    // Lignes verticales aux coins
    DrawLine(BottomCenter + AxisX * Size * 0.5f + AxisY * Size * 0.5f, 
             TopCenter + AxisX * Size * 0.5f + AxisY * Size * 0.5f, Color);
             
    DrawLine(BottomCenter + AxisX * Size * 0.5f - AxisY * Size * 0.5f,
             TopCenter + AxisX * Size * 0.5f - AxisY * Size * 0.5f, Color);
             
    DrawLine(BottomCenter - AxisX * Size * 0.5f + AxisY * Size * 0.5f,
             TopCenter - AxisX * Size * 0.5f + AxisY * Size * 0.5f, Color);
             
    DrawLine(BottomCenter - AxisX * Size * 0.5f - AxisY * Size * 0.5f,
             TopCenter - AxisX * Size * 0.5f - AxisY * Size * 0.5f, Color);
    
    // Lignes du carré du haut
    DrawLine(TopCenter + AxisX * Size * 0.5f + AxisY * Size * 0.5f,
             TopCenter + AxisX * Size * 0.5f - AxisY * Size * 0.5f, Color);
             
    DrawLine(TopCenter + AxisX * Size * 0.5f - AxisY * Size * 0.5f,
             TopCenter - AxisX * Size * 0.5f - AxisY * Size * 0.5f, Color);
             
    DrawLine(TopCenter - AxisX * Size * 0.5f - AxisY * Size * 0.5f,
             TopCenter - AxisX * Size * 0.5f + AxisY * Size * 0.5f, Color);
             
    DrawLine(TopCenter - AxisX * Size * 0.5f + AxisY * Size * 0.5f,
             TopCenter + AxisX * Size * 0.5f + AxisY * Size * 0.5f, Color);

    // Draw normal arrow au centre du volume
	DrawLine(GridCenter, GridCenter + Normal * (Size * 0.2f), FColor::Red);
}

void GravityFieldDrawer::DrawLine(const FVector& Start, const FVector& End, const FColor& Color)
{
	if (DebugLines)
	{
		DebugLines->DrawLine(Start, End, Color, 0, 2.0f);
	}
}

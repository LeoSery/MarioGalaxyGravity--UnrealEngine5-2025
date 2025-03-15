#include "TorusMeshComponent.h"

UTorusMeshComponent::UTorusMeshComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	bUseComplexAsSimpleCollision = true;
	bUseAsyncCooking = true;

	SetCollisionProfileName(TEXT("BlockAll"));
	SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetGenerateOverlapEvents(true);
	
	SetCastShadow(true);
	SetVisibility(true);
}

void UTorusMeshComponent::OnRegister()
{
	Super::OnRegister();
	GenerateTorusMesh();
}

void UTorusMeshComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UTorusMeshComponent, TorusRadius) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UTorusMeshComponent, TubeRadius) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UTorusMeshComponent, TorusSegments) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UTorusMeshComponent, TubeSegments))
	{
		GenerateTorusMesh();
	}
}

void UTorusMeshComponent::GenerateTorusMesh()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UV0;

	const int32 NumVertices = TorusSegments * TubeSegments;
	const int32 NumTriangles = TorusSegments * TubeSegments * 6;

	Vertices.Reserve(NumVertices);
	Triangles.Reserve(NumTriangles);
	Normals.Reserve(NumVertices);
	UV0.Reserve(NumVertices);
	
    for (int32 i = 0; i < TorusSegments; i++)
    {
        float Angle1 = (2.0f * PI * i) / TorusSegments;
    	
        FVector CircleCenter(
            TorusRadius * FMath::Cos(Angle1),
            TorusRadius * FMath::Sin(Angle1),
            0
        );
    	
        FVector RadialDir = CircleCenter.GetSafeNormal();
        
        for (int32 j = 0; j < TubeSegments; j++)
        {
            float Angle2 = (2.0f * PI * j) / TubeSegments;
        	
            FVector UpDir(0, 0, 1);
        	 
            FVector TangentDir = FVector::CrossProduct(RadialDir, UpDir).GetSafeNormal();
        	
            FVector PointOnTube = CircleCenter + (RadialDir * FMath::Cos(Angle2) + UpDir * FMath::Sin(Angle2)) * TubeRadius;
            
            Vertices.Add(PointOnTube);
        	
            FVector Normal = (PointOnTube - CircleCenter).GetSafeNormal();
            Normals.Add(Normal);
        	
            UV0.Add(FVector2D(static_cast<float>(i) / TorusSegments, static_cast<float>(j) / TubeSegments));
        }
    }
	
    for (int32 i = 0; i < TorusSegments; i++)
    {
        int32 NextI = (i + 1) % TorusSegments;
        
        for (int32 j = 0; j < TubeSegments; j++)
        {
            int32 NextJ = (j + 1) % TubeSegments;
            
            int32 Current = i * TubeSegments + j;
            int32 Next = i * TubeSegments + NextJ;
            int32 NextRow = NextI * TubeSegments + j;
            int32 NextRowNext = NextI * TubeSegments + NextJ;
            
            // Premier triangle
            Triangles.Add(Current);
            Triangles.Add(Next);
            Triangles.Add(NextRow);
            
            // Second triangle
            Triangles.Add(Next);
            Triangles.Add(NextRowNext);
            Triangles.Add(NextRow);
        }
    }
	
    CreateMeshSection_LinearColor(
        0,
        Vertices,
        Triangles,
        Normals,
        UV0,
        TArray<FLinearColor>(),
        TArray<FProcMeshTangent>(),
        true
    );
	
    if (GetNumMaterials() == 0)
    {
        UMaterial* DefaultMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
        if (DefaultMaterial)
        {
            SetMaterial(0, DefaultMaterial);
        }
    }
}

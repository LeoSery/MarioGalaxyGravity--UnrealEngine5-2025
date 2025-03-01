#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "TorusMeshComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MGG_API UTorusMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	UTorusMeshComponent(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh Settings")
	float TorusRadius = 200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh Settings")
	float TubeRadius = 50.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh Settings", meta = (ClampMin = "4", ClampMax = "64"))
	int32 TorusSegments = 16;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh Settings", meta = (ClampMin = "3", ClampMax = "32"))
	int32 TubeSegments = 8;
	
	UFUNCTION(Category = "Mesh Settings")
	void GenerateTorusMesh();
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    
protected:
	virtual void OnRegister() override;
};
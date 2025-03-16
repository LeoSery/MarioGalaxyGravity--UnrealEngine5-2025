#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "TorusMeshComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MGG_API UTorusMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	//////// CONSTRUCTOR ////////
	UTorusMeshComponent(const FObjectInitializer& ObjectInitializer);

	//////// UNREAL LIFECYCLE ///////
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	//////// FIELDS ////////
	//// Mesh configuration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh Settings")
	float TorusRadius = 200.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh Settings")
	float TubeRadius = 50.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh Settings", meta = (ClampMin = "4", ClampMax = "64"))
	int32 TorusSegments = 16;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh Settings", meta = (ClampMin = "3", ClampMax = "32"))
	int32 TubeSegments = 8;

	//////// METHODS ////////
	//// Procedural mesh generation
	UFUNCTION(Category = "Mesh Settings")
	void GenerateTorusMesh();

protected:
	//////// UNREAL LIFECYCLE ///////
	virtual void OnRegister() override;
};

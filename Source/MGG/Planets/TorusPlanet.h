#pragma once

#include "CoreMinimal.h"
#include "BasePlanet.h"
#include "MGG/GravityFields/TorusGravityFieldComponent.h"
#include "MGG/Utils/MeshGenerator/TorusMeshComponent.h"
#include "TorusPlanet.generated.h"

UCLASS()
class MGG_API ATorusPlanet : public ABasePlanet
{
	GENERATED_BODY()

public:
	//////// CONSTRUCTOR ////////
	ATorusPlanet();

	//////// UNREAL LIFECYCLE ////////
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	//////// UNREAL LIFECYCLE ////////
	virtual void BeginPlay() override;

	//////// METHODS ////////
	//// Procedural mesh methods
	void SyncTorusMeshSettings();

	//////// FIELDS ////////
	//// Component fields
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTorusGravityFieldComponent* TorusGravityField;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTorusMeshComponent* TorusMesh;

	//// Procedural mesh configuration
	UPROPERTY(EditAnywhere, Category = "Planet Settings|Procedural Mesh", meta = (DisplayName = "Torus Radius"))
	float TorusRadius = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Planet Settings|Procedural Mesh", meta = (DisplayName = "Tube Radius"))
	float TubeRadius = 25.0f;
	UPROPERTY(EditAnywhere, Category = "Planet Settings|Procedural Mesh", meta = (DisplayName = "Torus Segments", ClampMin = "4", ClampMax = "64"))
	int32 TorusSegments = 16;
	UPROPERTY(EditAnywhere, Category = "Planet Settings|Procedural Mesh", meta = (DisplayName = "Tube Segments", ClampMin = "3", ClampMax = "32"))
	int32 TubeSegments = 8;
	
};
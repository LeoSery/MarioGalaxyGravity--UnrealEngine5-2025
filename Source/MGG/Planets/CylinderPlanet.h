#pragma once

#include "CoreMinimal.h"
#include "BasePlanet.h"
#include "MGG/GravityFields/CylinderGravityFieldComponent.h"
#include "CylinderPlanet.generated.h"

UCLASS()
class MGG_API ACylinderPlanet : public ABasePlanet
{
	GENERATED_BODY()

public:
	//////// CONSTRUCTOR ////////
	ACylinderPlanet();

	//////// UNREAL LIFECYCLE ////////
	virtual void OnConstruction(const FTransform& Transform) override;

	//////// FIELDS ////////
	//// Planet configuration
	UPROPERTY(EditAnywhere, Category = "Planet Settings", meta = (DisplayName = "Cylinder Height Ratio"))
	float CylinderHeightRatio = 2.0f;
	
protected:
	//////// UNREAL LIFECYCLE ////////
	virtual void BeginPlay() override;

	//////// FIELDS ////////
	//// Component fields
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCylinderGravityFieldComponent* CylinderGravityField;
};

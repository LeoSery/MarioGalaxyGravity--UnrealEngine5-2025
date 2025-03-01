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
	ACylinderPlanet();
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, Category = "Planet Settings", meta = (DisplayName = "Cylinder Height Ratio"))
	float CylinderHeightRatio = 2.0f;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCylinderGravityFieldComponent* CylinderGravityField;
};
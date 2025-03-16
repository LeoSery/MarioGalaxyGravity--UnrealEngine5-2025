#pragma once

#include "CoreMinimal.h"
#include "BasePlanet.h"
#include "MGG/GravityFields/CubeGravityFieldComponent.h"
#include "CubicPlanet.generated.h"

UCLASS()
class MGG_API ACubicPlanet : public ABasePlanet
{
	GENERATED_BODY()

public:
	//////// CONSTRUCTOR ////////
	ACubicPlanet();

	//////// UNREAL LIFECYCLE ////////
	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:
	//////// UNREAL LIFECYCLE ////////
	virtual void BeginPlay() override;

	//////// FIELDS ////////
	//// Component fields
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCubeGravityFieldComponent* CubeGravityField;
	
};

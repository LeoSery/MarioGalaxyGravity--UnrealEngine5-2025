#pragma once

#include "CoreMinimal.h"
#include "BasePlanet.h"
#include "MGG/GravityFields/SphereGravityFieldComponent.h"
#include "SpherePlanet.generated.h"

UCLASS()
class MGG_API ASpherePlanet : public ABasePlanet
{
	GENERATED_BODY()

public:
	//////// CONSTRUCTOR ////////
	ASpherePlanet();

	//////// UNREAL LIFECYCLE ////////
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	//////// UNREAL LIFECYCLE ////////
	virtual void BeginPlay() override;

	//////// FIELDS ////////
	//// Component fields
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereGravityFieldComponent* SphereGravityField;
	
};
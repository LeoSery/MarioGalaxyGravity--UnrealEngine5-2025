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
	ACubicPlanet();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCubeGravityFieldComponent* CubeGravityField;
	
};

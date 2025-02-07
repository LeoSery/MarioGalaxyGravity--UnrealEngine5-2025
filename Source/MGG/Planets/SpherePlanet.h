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
	ASpherePlanet();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereGravityFieldComponent* SphereGravityField;
	
};
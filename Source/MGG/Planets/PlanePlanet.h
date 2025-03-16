#pragma once

#include "CoreMinimal.h"
#include "BasePlanet.h"
#include "MGG/GravityFields/PlaneGravityFieldComponent.h"
#include "PlanePlanet.generated.h"

UCLASS()
class MGG_API APlanePlanet : public ABasePlanet
{
	GENERATED_BODY()

public:
	//////// CONSTRUCTOR ////////
	APlanePlanet();

	//////// UNREAL LIFECYCLE ////////
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	//////// UNREAL LIFECYCLE ////////
	virtual void BeginPlay() override;

	//////// FIELDS ////////
	//// Component fields
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPlaneGravityFieldComponent* PlaneGravityField;

	//// Planet configuration
	UPROPERTY(EditAnywhere, Category = "Planet Settings", meta = (DisplayName = "Initial Offset"))
	FVector InitialOffset = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, Category = "Planet Settings", meta = (DisplayName = "Initial Rotation"))
	FRotator InitialRotation = FRotator(0.0f, 0.0f, 0.0f);
	
};
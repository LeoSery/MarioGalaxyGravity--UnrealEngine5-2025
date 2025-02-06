#pragma once

#include "CoreMinimal.h"
#include "BasePlanet.h"
#include "MGG/GravityFields/TorusGravityFieldComponent.h"
#include "TorusPlanet.generated.h"

UCLASS()
class MGG_API ATorusPlanet : public ABasePlanet
{
	GENERATED_BODY()

public:
	ATorusPlanet();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTorusGravityFieldComponent* TorusGravityField;

	virtual void InitializeGravityField();
};
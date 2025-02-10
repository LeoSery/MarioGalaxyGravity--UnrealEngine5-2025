#pragma once

#include "CoreMinimal.h"
#include "BaseGravityFieldComponent.h"
#include "SphereGravityFieldComponent.generated.h"

UCLASS()
class MGG_API USphereGravityFieldComponent : public UBaseGravityFieldComponent
{
	GENERATED_BODY()

public:
	USphereGravityFieldComponent();

protected:
	virtual void DrawDebugGravityField() override;
	virtual FVector CalculateGravityVector(const FVector& TargetLocation) const override;
	virtual FGravityFieldDimensions CalculateFieldDimensions() const override;
	virtual void UpdateGravityVolume() override;
};
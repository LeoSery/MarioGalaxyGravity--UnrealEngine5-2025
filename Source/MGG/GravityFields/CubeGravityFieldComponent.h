#pragma once

#include "CoreMinimal.h"
#include "MGG/GravityFields/BaseGravityFieldComponent.h"
#include "CubeGravityFieldComponent.generated.h"

UCLASS()
class MGG_API UCubeGravityFieldComponent : public UBaseGravityFieldComponent
{
	GENERATED_BODY()

public:
	UCubeGravityFieldComponent();

protected:
	virtual void DrawDebugGravityField() override;
	virtual FVector CalculateGravityVector(const FVector& TargetLocation) const override;
	virtual FGravityFieldDimensions CalculateFieldDimensions() const override;
	virtual void UpdateGravityVolume() override;
};
#pragma once

#include "CoreMinimal.h"
#include "BaseGravityFieldComponent.h"
#include "PlaneGravityFieldComponent.generated.h"

UCLASS()
class MGG_API UPlaneGravityFieldComponent : public UBaseGravityFieldComponent
{
	GENERATED_BODY()

public:
	UPlaneGravityFieldComponent();
	virtual void UpdateGravityVolume() override;
protected:
	virtual void DrawDebugGravityField() override;
	virtual FVector CalculateGravityVector(const FVector& TargetLocation) const override;
	virtual FGravityFieldDimensions CalculateFieldDimensions() const override;
	
};
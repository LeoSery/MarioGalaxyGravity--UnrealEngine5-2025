#pragma once

#include "CoreMinimal.h"
#include "BaseGravityFieldComponent.h"
#include "TorusGravityFieldComponent.generated.h"

UCLASS()
class MGG_API UTorusGravityFieldComponent : public UBaseGravityFieldComponent
{
	GENERATED_BODY()

public:
	UTorusGravityFieldComponent();

protected:
	virtual void DrawDebugGravityField() override;
	virtual FVector CalculateGravityVector(const FVector& TargetLocation) const override;
	virtual FGravityFieldDimensions CalculateFieldDimensions() const override;
	virtual void UpdateGravityVolume() override;
};
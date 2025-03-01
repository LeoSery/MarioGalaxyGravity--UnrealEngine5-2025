#pragma once

#include "CoreMinimal.h"
#include "BaseGravityFieldComponent.h"
#include "CylinderGravityFieldComponent.generated.h"

UCLASS()
class MGG_API UCylinderGravityFieldComponent : public UBaseGravityFieldComponent
{
	GENERATED_BODY()

public:
	UCylinderGravityFieldComponent();
	virtual void UpdateGravityVolume() override;

	UPROPERTY(EditAnywhere, Category = "Gravity Field", meta = (DisplayName = "Cylinder Height"))
	float CylinderHeight = 200.0f;

protected:
	virtual void DrawDebugGravityField() override;
	virtual FVector CalculateGravityVector(const FVector& TargetLocation) const override;
	virtual FGravityFieldDimensions CalculateFieldDimensions() const override;

	FORCEINLINE virtual bool RequiresConstantGravityUpdate() const override { return true; }
};
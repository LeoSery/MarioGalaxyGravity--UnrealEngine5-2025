#pragma once

#include "CoreMinimal.h"
#include "BaseGravityFieldComponent.h"
#include "CylinderGravityFieldComponent.generated.h"

UCLASS()
class MGG_API UCylinderGravityFieldComponent : public UBaseGravityFieldComponent
{
	GENERATED_BODY()

public:
	//////// CONSTRUCTOR ////////
	UCylinderGravityFieldComponent();

	//////// METHODS ////////
	//// Gravity field methods
	virtual void UpdateGravityVolume() override;

	//////// FIELDS ////////
	//// Config fields
	UPROPERTY(EditAnywhere, Category = "Gravity Field", meta = (DisplayName = "Cylinder Height"))
	float CylinderHeight = 200.0f;

protected:
	//////// METHODS ////////
	//// Debug methods
	virtual void DrawDebugGravityField() override;

	//// Gravity field methods
	virtual FVector CalculateGravityVector(const FVector& TargetLocation) const override;
	virtual FGravityFieldDimensions CalculateFieldDimensions() const override;

	//////// INLINE METHODS ////////
	//// Gravity state methods
	FORCEINLINE virtual bool RequiresConstantGravityUpdate() const override { return true; }
};
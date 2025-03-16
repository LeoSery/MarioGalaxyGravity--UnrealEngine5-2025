#pragma once

#include "CoreMinimal.h"
#include "BaseGravityFieldComponent.h"
#include "PlaneGravityFieldComponent.generated.h"

UCLASS()
class MGG_API UPlaneGravityFieldComponent : public UBaseGravityFieldComponent
{
	GENERATED_BODY()

public:
	//////// CONSTRUCTOR ////////
	UPlaneGravityFieldComponent();

	//////// METHODS ////////
	//// Gravity field methods
	virtual void UpdateGravityVolume() override;
	
protected:
	//////// METHODS ////////
	//// Debug methods
	virtual void DrawDebugGravityField() override;

	//// Gravity field methods
	virtual FVector CalculateGravityVector(const FVector& TargetLocation) const override;
	virtual FGravityFieldDimensions CalculateFieldDimensions() const override;

	//////// INLINE METHODS ////////
	//// Gravity state methods
	FORCEINLINE virtual bool RequiresConstantGravityUpdate() const override { return false; } // constant force on this shape, no need to update gravity
};
#pragma once

#include "CoreMinimal.h"
#include "BaseGravityFieldComponent.h"
#include "TorusGravityFieldComponent.generated.h"

UCLASS()
class MGG_API UTorusGravityFieldComponent : public UBaseGravityFieldComponent
{
	GENERATED_BODY()

public:
	//////// CONSTRUCTOR ////////
	UTorusGravityFieldComponent();

protected:
	//////// METHODS ////////
	//// Debug methods
	virtual void DrawDebugGravityField() override;

	//// Gravity field methods
	virtual FVector CalculateGravityVector(const FVector& TargetLocation) const override;
	virtual FGravityFieldDimensions CalculateFieldDimensions() const override;
	virtual void UpdateGravityVolume() override;

	//////// INLINE METHODS ////////
	//// Gravity state methods
	FORCEINLINE virtual bool RequiresConstantGravityUpdate() const override { return true; }

private:
	//////// METHODS ////////
	//// Helper methods
	float GetTorusScaleFactor() const;
};
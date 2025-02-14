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
	virtual void UpdateGravityVolume() override;

protected:
	virtual void DrawDebugGravityField() override;
	virtual FVector CalculateGravityVector(const FVector& TargetLocation) const override;
	virtual FGravityFieldDimensions CalculateFieldDimensions() const override;

	FORCEINLINE virtual bool RequiresConstantGravityUpdate() const override { return true; }

private:
	struct FCubePositionFlags
	{
		static constexpr uint8 Inside = 0;
		static constexpr uint8 Forward = 1;
		static constexpr uint8 Behind = 2;
        
		uint8 X : 2;
		uint8 Y : 2;
		uint8 Z : 2; 
	};

	FCubePositionFlags CalculatePositionFlags(const FVector& RelativePosition, const FVector& Extent) const;
	FVector CalculateBlendFactors(const FVector& RelativePosition, const FVector& Extent, const FCubePositionFlags& Flags) const;
	FVector ConstructGravityComponentVector(const FCubePositionFlags& Flags, const FVector& Factors = FVector(1.0f)) const;
};
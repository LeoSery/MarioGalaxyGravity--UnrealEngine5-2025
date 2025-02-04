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
	virtual void ApplyGravity() override;

protected:
	virtual void DrawDebugGravityField() override;
};
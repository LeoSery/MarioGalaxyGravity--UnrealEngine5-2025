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
	virtual void ApplyGravity() override;

protected:
	virtual void DrawDebugGravityField() override;
};
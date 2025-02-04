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
	virtual void ApplyGravity() override;

protected:
	virtual void DrawDebugGravityField() override;
};
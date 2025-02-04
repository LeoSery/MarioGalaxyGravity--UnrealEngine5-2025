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
	virtual void ApplyGravity() override;

protected:
	virtual void DrawDebugGravityField() override;
};
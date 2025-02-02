#pragma once

#include "CoreMinimal.h"
#include "BaseGravityField.h"
#include "SphereGravityField.generated.h"

UCLASS()
class MGG_API ASphereGravityField : public ABaseGravityField
{
	GENERATED_BODY()

public:
	ASphereGravityField();
	virtual void ApplyGravity() override;

protected:
	virtual void DrawDebugGravityField() override;
	virtual void BeginPlay() override;
};
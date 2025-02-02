#pragma once

#include "CoreMinimal.h"
#include "BaseGravityField.h"
#include "PlaneGravityField.generated.h"

UCLASS()
class MGG_API APlaneGravityField : public ABaseGravityField
{
	GENERATED_BODY()

public:
	APlaneGravityField();
	virtual void ApplyGravity() override;

protected:
	virtual void DrawDebugGravityField() override;
	virtual void BeginPlay() override;
};
#pragma once

#include "CoreMinimal.h"
#include "BaseGravityField.h"
#include "CubeGravityField.generated.h"

UCLASS()
class MGG_API ACubeGravityField : public ABaseGravityField
{
	GENERATED_BODY()

public:
	ACubeGravityField();
	virtual void ApplyGravity() override;

protected:
	virtual void DrawDebugGravityField() override;
	virtual void BeginPlay() override;
};
#pragma once

#include "CoreMinimal.h"
#include "BaseGravityField.h"
#include "ThorusGravityField.generated.h"

UCLASS()
class MGG_API AThorusGravityField : public ABaseGravityField
{
	GENERATED_BODY()

public:
	AThorusGravityField();
	virtual void ApplyGravity() override;

protected:
	virtual void DrawDebugGravityField() override;
	virtual void BeginPlay() override;
};
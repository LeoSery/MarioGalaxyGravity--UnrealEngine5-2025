#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GravityAffected.generated.h"

UINTERFACE()
class UGravityAffected : public UInterface
{
	GENERATED_BODY()
};

class MGG_API IGravityAffected
{
	GENERATED_BODY()

public:
	virtual FVector& GetGravityVector() = 0;
	virtual void OnEnterGravityField(const FVector& NewGravityVector) = 0;
	virtual void OnExitGravityField() = 0;
};

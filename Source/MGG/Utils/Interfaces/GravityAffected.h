﻿#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GravityAffected.generated.h"

class UBaseGravityFieldComponent;

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
	virtual void UpdateCurrentGravityField() = 0;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnEnterGravityField(const FVector& NewGravityVector);
	virtual void OnEnterGravityField_Implementation(const FVector& NewGravityVector) = 0;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnExitGravityField();
	virtual void OnExitGravityField_Implementation() = 0;
	
	TArray<UBaseGravityFieldComponent*> GravityFields;
	UBaseGravityFieldComponent* GetActiveGravityField();
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MGG/Utils/Interfaces/GravityAffected.h"
#include <EnhancedInputLibrary.h>
#include "MGG_Mario.generated.h"


UCLASS()
class MGG_API AMGG_Mario : public APawn, public IGravityAffected
{
	GENERATED_BODY()

private:
	bool bIsInGravityField = false;

public:
	AMGG_Mario();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input,meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	virtual void Tick(float DeltaTime) override;

	void PhysicProcess(float DeltaTime);
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//IGravityAffected Interface methods
	virtual void OnEnterGravityField(const FVector& NewGravityVector) override;
	virtual void OnExitGravityField() override;
	virtual FVector& GetGravityVector() override { return GravityVector; }

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FVector GravityVector;
	

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Jump();

};

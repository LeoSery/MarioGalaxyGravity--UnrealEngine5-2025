#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MGG/Utils/Interfaces/GravityAffected.h"
#include <EnhancedInputLibrary.h>
#include "MGG_Mario.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

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
	virtual void OnEnterGravityField_Implementation(const FVector& NewGravityVector) override;
	virtual void OnExitGravityField_Implementation() override;
	virtual FVector& GetGravityVector() override { return GravityVector; }
	virtual void UpdateCurrentGravityField() override;

	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;
	UPROPERTY(BlueprintReadOnly)
	FVector GravityVector;

	UPROPERTY(EditAnywhere, Category = Movement)
	float Speed = 500.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	float CameraYaw = 0.0f;
	UPROPERTY()
	float CameraPitch = 0.0f;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Jump();
	void StopJumping();

	void RotatingMario();
};

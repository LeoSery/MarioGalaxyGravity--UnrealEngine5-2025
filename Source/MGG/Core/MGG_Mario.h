#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MGG/Utils/Interfaces/GravityAffected.h"
#include <EnhancedInputLibrary.h>
#include "MGG_Mario.generated.h"

//////// FORWARD DECLARATION ////////
//// Class
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

//// Struct
struct FInputActionValue;

UCLASS()
class MGG_API AMGG_Mario : public APawn, public IGravityAffected
{
	GENERATED_BODY()

public:
	//////// CONSTRUCTOR ////////
	AMGG_Mario();

	//////// UNREAL LIFECYCLE ////////
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//////// INTERFACE IMPLEMENTATIONS ////////
	//// IGravityAffected implementation
	virtual void OnEnterGravityField_Implementation(const FVector& NewGravityVector) override;
	virtual void OnExitGravityField_Implementation() override;

	//////// METHODS ////////
	//// IGravityAffected implementation
	virtual void UpdateCurrentGravityField() override;
	
	//////// INLINE METHODS ////////
	//// IGravityAffected implementation
	FORCEINLINE virtual FVector& GetGravityVector() override { return GravityVector; }
	
	//////// FIELDS ////////
	//// Input fields
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input,meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	//// Movement fields
	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;
	UPROPERTY(BlueprintReadOnly)
	FVector GravityVector;
	UPROPERTY(EditAnywhere, Category = Movement)
	float Speed = 500.0f;

	//// Components fields
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	//// Camera fields
	UPROPERTY()
	float CameraYaw = 0.0f;
	UPROPERTY()
	float CameraPitch = 0.0f;

protected:
	//////// UNREAL LIFECYCLE ////////
	virtual void BeginPlay() override;

	//////// METHODS ////////
	//// Input methods
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJumping();

	//// Physics methods
	void PhysicProcess(float DeltaTime);
	void RotatingMario();

private:
	//////// FIELDS ////////
	//// State fields
	bool bIsInGravityField = false;
};

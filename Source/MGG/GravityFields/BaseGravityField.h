#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGravityField.generated.h"

UENUM(BlueprintType)
enum GravityType
{
	NONE,
	PLANE UMETA(DisplayName = "Plane"),
	SPHERE UMETA(DisplayName = "Sphere"),
	CUBE UMETA(DisplayName = "Cube"),
	TORUS UMETA(DisplayName = "Torus"),    
};

UCLASS(Abstract)
class MGG_API ABaseGravityField : public AActor
{
	GENERATED_BODY()

public:
	ABaseGravityField();
    
	virtual void Tick(float DeltaTime) override;
	
	virtual void DrawDebugGravityField() PURE_VIRTUAL(ABaseGravityField::DrawDebugGravityField,);
	virtual void ApplyGravity() PURE_VIRTUAL(ABaseGravityField::ApplyGravity,);

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowDebugField = true;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Gravity")
	float GravityStrength;

	UPROPERTY(EditAnywhere, Category = "Gravity")
	int32 GravityFieldPriority;

	UPROPERTY(EditAnywhere, Category = "Gravity")
	TEnumAsByte<GravityType> GravityType;

	UPROPERTY(EditAnywhere, Category = "Gravity")
	float GravityRadius = 500.0f;
};
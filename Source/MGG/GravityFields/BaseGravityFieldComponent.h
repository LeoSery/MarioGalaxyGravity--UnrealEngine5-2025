#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MGG/Utils/Drawers/GravityFieldDrawer.h"
#include "MGG/Utils/Data/GravityData.h"
#include "BaseGravityFieldComponent.generated.h"

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MGG_API UBaseGravityFieldComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UBaseGravityFieldComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport = ETeleportType::None) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    
	virtual void DrawDebugGravityField() PURE_VIRTUAL(UBaseGravityFieldComponent::DrawDebugGravityField,);
	virtual void ApplyGravity() PURE_VIRTUAL(UBaseGravityFieldComponent::ApplyGravity,);

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowDebugField = true;
    
protected:
	virtual void OnRegister() override;

	void RedrawDebugField();

	UPROPERTY(EditAnywhere, Category = "Gravity")
	float GravityStrength;

	UPROPERTY(EditAnywhere, Category = "Gravity")
	int32 GravityFieldPriority;

	UPROPERTY(EditAnywhere, Category = "Gravity")
	TEnumAsByte<EGravityFieldType> GravityType;

	UPROPERTY(EditAnywhere, Category = "Gravity")
	float GravityRadius = 500.0f;

	TUniquePtr<GravityFieldDrawer> currentDrawer;

	UPROPERTY()
	ULineBatchComponent* DebugLines;
};
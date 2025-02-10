#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MGG/Utils/Drawers/GravityFieldDrawer.h"
#include "BaseGravityFieldComponent.generated.h"

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MGG_API UBaseGravityFieldComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UBaseGravityFieldComponent();
	
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport = ETeleportType::None) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	virtual void DrawDebugGravityField() PURE_VIRTUAL(UBaseGravityFieldComponent::DrawDebugGravityField,);

	// Getters
	float GetGravityStrength() const { return GravityStrength; }
	int32 GetGravityFieldPriority() const { return GravityFieldPriority; }
	float GetGravityInfluenceRange() const { return GravityInfluenceRange; }

	float GetTotalGravityRadius() const;

	// Setters
	void SetGravityStrength(float NewGravityStrength) { GravityStrength = NewGravityStrength; }
	void SetGravityFieldPriority(int32 NewGravityFieldPriority) { GravityFieldPriority = NewGravityFieldPriority; }
	void SetGravityInfluenceRange(float NewGravityRadius) { GravityInfluenceRange = NewGravityRadius; }

	void RedrawDebugField();

	UFUNCTION()
	void OnGravityVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnGravityVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowDebugField = true;
    
protected:
	struct FGravityFieldDimensions
	{
		FVector Size;
		FVector Center;
	};
	
	virtual void OnRegister() override;
	
	float GravityStrength;
	int32 GravityFieldPriority;
	float GravityInfluenceRange;

	TUniquePtr<GravityFieldDrawer> currentDrawer;

	UPROPERTY()
	ULineBatchComponent* DebugLines;

	UPROPERTY(VisibleAnywhere)
	UShapeComponent* GravityVolume;
	
	void UpdateFieldDimensions();

	virtual void UpdateGravityVolume() PURE_VIRTUAL(UBaseGravityFieldComponent::UpdateGravityVolume,);
	
	FGravityFieldDimensions CurrentDimensions;
	
	virtual FGravityFieldDimensions CalculateFieldDimensions() const PURE_VIRTUAL(UBaseGravityFieldComponent::CalculateFieldDimensions, return FGravityFieldDimensions(););
	
	virtual FVector CalculateGravityVector(const FVector& TargetLocation) const PURE_VIRTUAL(UBaseGravityFieldComponent::CalculateGravityVector, return FVector::ZeroVector;);
};
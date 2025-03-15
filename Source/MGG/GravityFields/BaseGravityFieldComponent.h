#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "MGG/Utils/Drawers/GravityFieldDrawer.h"
#include "BaseGravityFieldComponent.generated.h"

class ULineBatchComponent;
class UShapeComponent;

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MGG_API UBaseGravityFieldComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UBaseGravityFieldComponent();
	
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport = ETeleportType::None) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void BeginDestroy() override;
	
	virtual void DrawDebugGravityField() PURE_VIRTUAL(UBaseGravityFieldComponent::DrawDebugGravityField,);

	// Getters
	FORCEINLINE float GetGravityStrength() const { return GravityStrength; }
	FORCEINLINE int32 GetGravityFieldPriority() const { return GravityFieldPriority; }
	FORCEINLINE float GetGravityInfluenceRange() const { return GravityInfluenceRange; }

	float GetTotalGravityRadius() const;

	// Setters
	FORCEINLINE void SetGravityStrength(float NewGravityStrength) { GravityStrength = NewGravityStrength; }
	FORCEINLINE void SetGravityFieldPriority(int32 NewGravityFieldPriority) { GravityFieldPriority = NewGravityFieldPriority; }
	FORCEINLINE void SetGravityInfluenceRange(float NewGravityRadius) { GravityInfluenceRange = NewGravityRadius; }

	void RedrawDebugField();
	void UpdateFieldDimensions();
	virtual void UpdateGravityVolume() PURE_VIRTUAL(UBaseGravityFieldComponent::UpdateGravityVolume, );
	UFUNCTION()
	void OnGravityVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnGravityVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE bool IsActorInGravityField(AActor* Actor) const
	{
		if (GravityVolume && Actor)
		{
			return GravityVolume->IsOverlappingActor(Actor);
		}
		return false;
	}
	
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowDebugField = true;

	virtual bool RequiresConstantGravityUpdate() const PURE_VIRTUAL(UBaseGravityFieldComponent::RequiresConstantGravityUpdate, return false;);
	virtual FVector CalculateGravityVector(const FVector& TargetLocation) const PURE_VIRTUAL(UBaseGravityFieldComponent::CalculateGravityVector, return FVector::ZeroVector;);
	
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
	
	FGravityFieldDimensions CurrentDimensions;
	virtual FGravityFieldDimensions CalculateFieldDimensions() const PURE_VIRTUAL(UBaseGravityFieldComponent::CalculateFieldDimensions, return FGravityFieldDimensions(););
};
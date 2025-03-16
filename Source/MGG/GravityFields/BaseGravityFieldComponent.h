#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "MGG/Utils/Drawers/GravityFieldDrawer.h"
#include "BaseGravityFieldComponent.generated.h"

//////// FORWARD DECLARATION ////////
//// Class
class ULineBatchComponent;
class UShapeComponent;


UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MGG_API UBaseGravityFieldComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	//////// CONSTRUCTOR ////////
	UBaseGravityFieldComponent();

	//////// UNREAL LIFECYCLE ////////
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport = ETeleportType::None) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void BeginDestroy() override;

	//////// FIELDS ////////
	//// Debug Fields
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowDebugField = true;

	//////// METHODS ////////
	//// Debug methods
	virtual void DrawDebugGravityField() PURE_VIRTUAL(UBaseGravityFieldComponent::DrawDebugGravityField,);
	void RedrawDebugField();

	//// Gravity state methods
	virtual bool RequiresConstantGravityUpdate() const PURE_VIRTUAL(UBaseGravityFieldComponent::RequiresConstantGravityUpdate, return false;);
	virtual FVector CalculateGravityVector(const FVector& TargetLocation) const PURE_VIRTUAL(UBaseGravityFieldComponent::CalculateGravityVector, return FVector::ZeroVector;);

	//// Gravity field methods
	void UpdateFieldDimensions();
	virtual void UpdateGravityVolume() PURE_VIRTUAL(UBaseGravityFieldComponent::UpdateGravityVolume, );
	float GetTotalGravityRadius() const;

	//// Overlap methods
	UFUNCTION()
	void OnGravityVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnGravityVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//////// INLINE METHODS ////////
	//// Check methods
	FORCEINLINE bool IsActorInGravityField(AActor* Actor) const { if (GravityVolume && Actor) { return GravityVolume->IsOverlappingActor(Actor); } return false; }
	
	//// Getters accessors
	FORCEINLINE float GetGravityStrength() const { return GravityStrength; }
	FORCEINLINE int32 GetGravityFieldPriority() const { return GravityFieldPriority; }
	FORCEINLINE float GetGravityInfluenceRange() const { return GravityInfluenceRange; }

	//// Setters accessors
	FORCEINLINE void SetGravityStrength(float NewGravityStrength) { GravityStrength = NewGravityStrength; }
	FORCEINLINE void SetGravityFieldPriority(int32 NewGravityFieldPriority) { GravityFieldPriority = NewGravityFieldPriority; }
	FORCEINLINE void SetGravityInfluenceRange(float NewGravityRadius) { GravityInfluenceRange = NewGravityRadius; }

protected:
	//////// UNREAL LIFECYCLE ////////
	virtual void OnRegister() override;

	//////// STRUCTS ////////
	struct FGravityFieldDimensions
	{
		FVector Size;
		FVector Center;
	};

	//////// FIELDS ////////
	//// Gravity fields
	float GravityStrength;
	int32 GravityFieldPriority;
	float GravityInfluenceRange;
	FGravityFieldDimensions CurrentDimensions;

	//// Gravity fields
	TUniquePtr<GravityFieldDrawer> currentDrawer;
	UPROPERTY()
	ULineBatchComponent* DebugLines;
	UPROPERTY(VisibleAnywhere)
	UShapeComponent* GravityVolume;

	//////// METHODS ////////
	//// Gravity field methods
	virtual FGravityFieldDimensions CalculateFieldDimensions() const PURE_VIRTUAL(UBaseGravityFieldComponent::CalculateFieldDimensions, return FGravityFieldDimensions(););
};
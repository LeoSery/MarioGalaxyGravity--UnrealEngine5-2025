#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePlanet.generated.h"

//////// FORWARD DECLARATION ////////
//// Class
class UStaticMeshComponent;
class UStaticMesh;
class UBaseGravityFieldComponent;

UCLASS(Abstract)
class MGG_API ABasePlanet : public AActor
{
	GENERATED_BODY()

public:
	//////// CONSTRUCTOR ////////
	ABasePlanet();

	//////// UNREAL LIFECYCLE ////////
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;

	//////// FIELDS ////////
	//// Planet configuration
	UPROPERTY(EditAnywhere, Category = "Planet Settings", meta = (DisplayName = "Planet Radius"))
	float PlanetRadius;

	//// Gravity configuration
	UPROPERTY(EditAnywhere, Category = "Planet Settings|Gravity", meta = (DisplayName = "Gravity Strength"))
	float GravityStrength;
	UPROPERTY(EditAnywhere, Category = "Planet Settings|Gravity", meta = (DisplayName = "Field Priority"))
	int32 GravityFieldPriority;
	UPROPERTY(EditAnywhere, Category = "Planet Settings|Gravity", meta = (DisplayName = "Influence Range"))
	float GravityInfluenceRange;
	
protected:
	//////// UNREAL LIFECYCLE ////////
	virtual void BeginPlay() override;

	//////// METHODS ////////
	//// Planet methods
	virtual void UpdatePlanetScale();
	void UpdatePlanetMesh();

	//// Gravity methods
	virtual void SyncGravityFieldSettings();

	//////// FIELDS ////////
	//// Component fields
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* PlanetMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Planet settings")
	UStaticMesh* DefaultMesh;

	//// Gravity fields
	UPROPERTY(Transient)
	UBaseGravityFieldComponent* CachedGravityField;

};

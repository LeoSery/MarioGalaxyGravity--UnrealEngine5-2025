#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePlanet.generated.h"

UCLASS(Abstract)
class MGG_API ABasePlanet : public AActor
{
	GENERATED_BODY()

public:
	ABasePlanet();

	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;

	UPROPERTY(EditAnywhere, Category = "Planet Settings", meta = (DisplayName = "Planet Radius"))
	float PlanetRadius;

	UPROPERTY(EditAnywhere, Category = "Planet Settings|Gravity", meta = (DisplayName = "Gravity Strength"))
	float GravityStrength;

	UPROPERTY(EditAnywhere, Category = "Planet Settings|Gravity", meta = (DisplayName = "Field Priority"))
	int32 GravityFieldPriority;

	UPROPERTY(EditAnywhere, Category = "Planet Settings|Gravity", meta = (DisplayName = "Influence Range"))
	float GravityInfluenceRange;
	
protected:
	virtual void BeginPlay() override;

	virtual void UpdatePlanetScale();
	void UpdatePlanetMesh();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* PlanetMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Planet settings")
	UStaticMesh* DefaultMesh;

	virtual void SyncGravityFieldSettings();
};

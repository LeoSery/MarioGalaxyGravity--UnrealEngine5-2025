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
	
protected:
	virtual void BeginPlay() override;

	virtual void UpdatePlanetScale();

	void UpdatePlanetMesh();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* PlanetMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Planet settings")
	UStaticMesh* DefaultMesh;

	UPROPERTY(EditAnywhere, Category = "Planet settings")
	float PlanetRadius;

	UPROPERTY(EditAnywhere, Category = "Planet settings")
	float PlanetGravity;
};

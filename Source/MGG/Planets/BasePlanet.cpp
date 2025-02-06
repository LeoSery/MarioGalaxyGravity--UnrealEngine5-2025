#include "BasePlanet.h"

#include "MGG/GravityFields/BaseGravityFieldComponent.h"

ABasePlanet::ABasePlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	PlanetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlanetMesh"));
	RootComponent = PlanetMesh;
	
	PlanetRadius = 1000.0f;
	PlanetGravity = 9.81f;

	if (PlanetMesh && DefaultMesh)
	{
		PlanetMesh->SetStaticMesh(DefaultMesh);
	}
}

void ABasePlanet::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePlanet::UpdatePlanetScale()
{
	if (PlanetMesh)
	{
		float const ScaleFactor = PlanetRadius / 100.0f;
		PlanetMesh->SetWorldScale3D(FVector(ScaleFactor));
	}
}

void ABasePlanet::UpdatePlanetMesh()
{
	if (PlanetMesh && DefaultMesh && PlanetMesh->GetStaticMesh() != DefaultMesh)
	{
		PlanetMesh->SetStaticMesh(DefaultMesh);
		UpdatePlanetScale();
	}
}

void ABasePlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasePlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdatePlanetMesh();
	UpdatePlanetScale();
}

void ABasePlanet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ABasePlanet, PlanetRadius))
	{
		UpdatePlanetScale();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(ABasePlanet, DefaultMesh))
	{
		UpdatePlanetMesh();
	}
	
	if (UBaseGravityFieldComponent* GravityField = GetComponentByClass<UBaseGravityFieldComponent>()) //work but not optimized -> disable it after level design
	{
		GravityField->RedrawDebugField();
	}
}

void ABasePlanet::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (UBaseGravityFieldComponent* GravityField = GetComponentByClass<UBaseGravityFieldComponent>())
	{
		GravityField->RedrawDebugField();
	}
}

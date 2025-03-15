#include "BasePlanet.h"

#include "MGG/GravityFields/BaseGravityFieldComponent.h"

ABasePlanet::ABasePlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	PlanetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlanetMesh"));
	PlanetMesh->SetGenerateOverlapEvents(false);
	RootComponent = PlanetMesh;
	
	PlanetRadius = 1000.0f;
	GravityStrength = 981.0f;
	GravityFieldPriority = 0;
	GravityInfluenceRange = 1000.0f;

	if (PlanetMesh && DefaultMesh)
	{
		PlanetMesh->SetStaticMesh(DefaultMesh);
	}
}

void ABasePlanet::BeginPlay()
{
	CachedGravityField = GetComponentByClass<UBaseGravityFieldComponent>();
	
	if (CachedGravityField)
	{
		CachedGravityField->UpdateGravityVolume();
		CachedGravityField->RedrawDebugField();
	}
	
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
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(ABasePlanet, GravityStrength) ||
			 PropertyName == GET_MEMBER_NAME_CHECKED(ABasePlanet, GravityFieldPriority) ||
			 PropertyName == GET_MEMBER_NAME_CHECKED(ABasePlanet, GravityInfluenceRange))
	{
		SyncGravityFieldSettings();
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

void ABasePlanet::SyncGravityFieldSettings()
{
	if (!CachedGravityField)
	{
		CachedGravityField = GetComponentByClass<UBaseGravityFieldComponent>();
	}
	
	if (CachedGravityField)
	{
		if (CachedGravityField->GetGravityStrength() != GravityStrength)
			CachedGravityField->SetGravityStrength(GravityStrength);
            
		if (CachedGravityField->GetGravityFieldPriority() != GravityFieldPriority)
			CachedGravityField->SetGravityFieldPriority(GravityFieldPriority);
            
		if (CachedGravityField->GetGravityInfluenceRange() != GravityInfluenceRange)
			CachedGravityField->SetGravityInfluenceRange(GravityInfluenceRange);
	}
}
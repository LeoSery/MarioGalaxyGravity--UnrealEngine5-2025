#include "TorusPlanet.h"

ATorusPlanet::ATorusPlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	TorusGravityField = CreateDefaultSubobject<UTorusGravityFieldComponent>(TEXT("TorusGravityField"));
	TorusGravityField->SetupAttachment(RootComponent);
}

void ATorusPlanet::BeginPlay()
{
	Super::BeginPlay();
}

void ATorusPlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATorusPlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SyncGravityFieldSettings();
}
#include "TorusPlanet.h"

ATorusPlanet::ATorusPlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	TorusMesh = CreateDefaultSubobject<UTorusMeshComponent>(TEXT("TorusMesh"));
	TorusMesh->SetupAttachment(RootComponent);

	TorusMesh->TorusRadius = 300.0f;
	TorusMesh->TubeRadius = 50.0f;

	TorusGravityField = CreateDefaultSubobject<UTorusGravityFieldComponent>(TEXT("TorusGravityField"));
	TorusGravityField->SetupAttachment(RootComponent);

	TorusMesh->SetCollisionProfileName(TEXT("BlockAll"));
	TorusMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ATorusPlanet::BeginPlay()
{
	Super::BeginPlay();
	SyncGravityFieldSettings();

	if (TorusMesh)
	{
		TorusMesh->GenerateTorusMesh();
	}
    
	if (TorusGravityField)
	{
		TorusGravityField->UpdateFieldDimensions();
		TorusGravityField->RedrawDebugField();
	}
}

void ATorusPlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SyncGravityFieldSettings();

	if (TorusMesh)
	{
		TorusMesh->GenerateTorusMesh();
	}
    
	if (TorusGravityField)
	{
		TorusGravityField->UpdateFieldDimensions();
		TorusGravityField->RedrawDebugField();
	}
}

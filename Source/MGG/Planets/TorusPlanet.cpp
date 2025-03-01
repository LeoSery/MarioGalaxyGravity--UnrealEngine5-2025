#include "TorusPlanet.h"

ATorusPlanet::ATorusPlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	TorusMesh = CreateDefaultSubobject<UTorusMeshComponent>(TEXT("TorusMesh"));
	TorusMesh->SetupAttachment(RootComponent);
	
	TorusGravityField = CreateDefaultSubobject<UTorusGravityFieldComponent>(TEXT("TorusGravityField"));
	TorusGravityField->SetupAttachment(RootComponent);

	TorusMesh->SetCollisionProfileName(TEXT("BlockAll"));
	TorusMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ATorusPlanet::BeginPlay()
{
	Super::BeginPlay();
	SyncTorusMeshSettings();
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

void ATorusPlanet::SyncTorusMeshSettings()
{
	if (TorusMesh)
	{
		TorusMesh->TorusRadius = TorusRadius;
		TorusMesh->TubeRadius = TubeRadius;
		TorusMesh->TorusSegments = TorusSegments;
		TorusMesh->TubeSegments = TubeSegments;
		TorusMesh->GenerateTorusMesh();
	}
}

void ATorusPlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SyncTorusMeshSettings();
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

void ATorusPlanet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ATorusPlanet, TorusRadius) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(ATorusPlanet, TubeRadius) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(ATorusPlanet, TorusSegments) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(ATorusPlanet, TubeSegments))
	{
		SyncTorusMeshSettings();
		
		if (TorusGravityField)
		{
			TorusGravityField->UpdateFieldDimensions();
			TorusGravityField->RedrawDebugField();
		}
	}
}

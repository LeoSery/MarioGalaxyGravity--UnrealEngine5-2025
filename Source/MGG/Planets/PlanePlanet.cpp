#include "PlanePlanet.h"

APlanePlanet::APlanePlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	PlaneGravityField = CreateDefaultSubobject<UPlaneGravityFieldComponent>(TEXT("PlaneGravityField"));
	PlaneGravityField->SetupAttachment(RootComponent);
}

void APlanePlanet::BeginPlay()
{
	Super::BeginPlay();
	SyncGravityFieldSettings();
}

void APlanePlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlanePlanet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	static bool bFirstConstruction = true;
	if (bFirstConstruction && PlanetMesh && PlanetMesh->GetStaticMesh())
	{
		FBoxSphereBounds bounds = PlanetMesh->GetStaticMesh()->GetBounds();
		float meshHeight = bounds.BoxExtent.Z * 2.0f;
		
		FVector meshOffset = FVector(0.0f, 0.0f, -meshHeight * 0.5f);
		FVector totalOffset = InitialOffset + meshOffset;
		
		//PlanetMesh->SetRelativeLocation(totalOffset);
		//PlaneGravityField->SetRelativeLocation(totalOffset);
	}
	
	SyncGravityFieldSettings();
}

// void APlanePlanet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
//
// 	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
//
// 	if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanePlanet, InitialOffset) ||
// 		PropertyName == GET_MEMBER_NAME_CHECKED(APlanePlanet, InitialRotation))
// 	{
// 		if (PlanetMesh)
// 		{
// 			//PlanetMesh->SetRelativeLocation(InitialOffset);
// 			//PlanetMesh->SetRelativeRotation(InitialRotation);
// 		}
// 	}
// }
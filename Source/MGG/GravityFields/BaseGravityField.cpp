#include "BaseGravityField.h"

ABaseGravityField::ABaseGravityField()
{
	PrimaryActorTick.bCanEverTick = true;
    
	GravityStrength = 9.81f;
	GravityFieldPriority = 0;
	GravityType = NONE;
}

void ABaseGravityField::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseGravityField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bShowDebugField)
	{
		DrawDebugGravityField();
	}
	ApplyGravity();
}

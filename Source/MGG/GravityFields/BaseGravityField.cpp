#include "BaseGravityField.h"

#include "Components/LineBatchComponent.h"

ABaseGravityField::ABaseGravityField()
{
	PrimaryActorTick.bCanEverTick = true;
    
	GravityStrength = 9.81f;
	GravityFieldPriority = 0;
	GravityType = NONE;
	
	DebugLines = CreateDefaultSubobject<ULineBatchComponent>(TEXT("DebugLines"));
}

void ABaseGravityField::BeginPlay()
{
	Super::BeginPlay();
	
	currentDrawer = MakeUnique<GravityFieldDrawer>(DebugLines);
	
	if(bShowDebugField)
	{
		DrawDebugGravityField();
	}
}

void ABaseGravityField::RedrawDebugField()
{
	if (bShowDebugField && DebugLines)
	{
		DebugLines->Flush();  
		DrawDebugGravityField();
	}
}

void ABaseGravityField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ApplyGravity();
}

void ABaseGravityField::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	RedrawDebugField();
}

void ABaseGravityField::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ABaseGravityField, GravityRadius))
	{
		RedrawDebugField();
	}
}

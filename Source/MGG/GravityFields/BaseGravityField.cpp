#include "BaseGravityField.h"

TArray<ABaseGravityField*> ABaseGravityField::AllGravityFields;

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
	AllGravityFields.Add(this);
	
	if(bShowDebugField)
	{
		DrawDebugGravityField();
	}
}

void ABaseGravityField::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AllGravityFields.Remove(this);
}

void ABaseGravityField::RedrawDebugField()
{
	if (bShowDebugField && GetWorld())
	{
		FlushPersistentDebugLines(GetWorld());
		FlushPersistentDebugLines(GetWorld());
		
		for (ABaseGravityField* Field : AllGravityFields)
		{
			if (Field && Field->bShowDebugField)
			{
				Field->DrawDebugGravityField();
			}
		}
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

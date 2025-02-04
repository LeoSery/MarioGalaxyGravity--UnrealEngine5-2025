#include "BaseGravityFieldComponent.h"
#include "Components/LineBatchComponent.h"

UBaseGravityFieldComponent::UBaseGravityFieldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	GravityStrength = 9.81f;
	GravityFieldPriority = 0;
	GravityType = NONE;
    
	DebugLines = CreateDefaultSubobject<ULineBatchComponent>(TEXT("DebugLines"));
}

void UBaseGravityFieldComponent::OnRegister()
{
	Super::OnRegister();
    
	if (!DebugLines)
	{
		DebugLines = CreateDefaultSubobject<ULineBatchComponent>(TEXT("DebugLines"));
	}

	currentDrawer = MakeUnique<GravityFieldDrawer>(DebugLines);
	if(bShowDebugField)
	{
		DrawDebugGravityField();
	}
}

void UBaseGravityFieldComponent::RedrawDebugField()
{
	if (bShowDebugField && DebugLines)
	{
		DebugLines->Flush();  
		DrawDebugGravityField();
	}
}

void UBaseGravityFieldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ApplyGravity();
}

void UBaseGravityFieldComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);
	RedrawDebugField();
}

void UBaseGravityFieldComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? 
						 PropertyChangedEvent.Property->GetFName() : NAME_None;
                         
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UBaseGravityFieldComponent, GravityRadius))
	{
		RedrawDebugField();
	}
}
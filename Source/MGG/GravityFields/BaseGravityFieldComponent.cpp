#include "BaseGravityFieldComponent.h"
#include "Components/LineBatchComponent.h"

UBaseGravityFieldComponent::UBaseGravityFieldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	GravityStrength = 9.81f;
	GravityFieldPriority = 0;
    
	DebugLines = CreateDefaultSubobject<ULineBatchComponent>(TEXT("DebugLines"));
}

void UBaseGravityFieldComponent::OnRegister()
{
	UE_LOG(LogTemp, Warning, TEXT("BaseGravityFieldComponent::OnRegister called"));
	Super::OnRegister();

	if (!currentDrawer)
	{
		currentDrawer = MakeUnique<GravityFieldDrawer>(DebugLines);
		if(bShowDebugField)
		{
			DebugLines->Flush();
			DrawDebugGravityField();
		}
	}
}

float UBaseGravityFieldComponent::GetTotalGravityRadius() const
{
	if (AActor* Owner = GetOwner())
	{
		if (UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>())
		{
			FVector Extent = MeshComp->Bounds.BoxExtent;
			float MeshRadius = Extent.GetMax();
			
			return FMath::Max(MeshRadius, MeshRadius + GravityInfluenceRange);
		}
	}
	return GravityInfluenceRange;
}

void UBaseGravityFieldComponent::RedrawDebugField()
{
	UE_LOG(LogTemp, Warning, TEXT("BaseGravityFieldComponent::RedrawDebugField called"));
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

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
                         
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UBaseGravityFieldComponent, GravityInfluenceRange))
	{
		RedrawDebugField();
	}
}
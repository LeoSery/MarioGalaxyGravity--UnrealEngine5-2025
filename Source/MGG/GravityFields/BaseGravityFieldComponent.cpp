#include "BaseGravityFieldComponent.h"
#include "MGG//Utils/Interfaces/GravityAffected.h"
#include "Components/LineBatchComponent.h"
#include "Components/ShapeComponent.h"
#include "MGG/Utils/Drawers/GravityFieldDrawer.h"

UBaseGravityFieldComponent::UBaseGravityFieldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	GravityStrength = 9.81f;
	GravityFieldPriority = 0;
    
	DebugLines = CreateDefaultSubobject<ULineBatchComponent>(TEXT("DebugLines"));
}

void UBaseGravityFieldComponent::OnRegister()
{
	Super::OnRegister();

	UpdateFieldDimensions();

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

void UBaseGravityFieldComponent::UpdateFieldDimensions()
{
	CurrentDimensions = CalculateFieldDimensions();
	UpdateGravityVolume();
}

void UBaseGravityFieldComponent::OnGravityVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->Implements<UGravityAffected>())
	{
		if (IGravityAffected* AffectedActor = Cast<IGravityAffected>(OtherActor))
		{
			AffectedActor->GravityFields.Add(this);
			UBaseGravityFieldComponent* NewActiveField = AffectedActor->GetActiveGravityField();

			FVector GravityVector = NewActiveField->CalculateGravityVector(OtherActor->GetActorLocation());
			IGravityAffected::Execute_OnEnterGravityField(OtherActor, GravityVector);
		}
	}
}

void UBaseGravityFieldComponent::OnGravityVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->Implements<UGravityAffected>())
	{
		if (IGravityAffected* AffectedActor = Cast<IGravityAffected>(OtherActor))
		{
			bool wasActiveField = false;
		
			if (AffectedActor->GetActiveGravityField() == this)
			{
				wasActiveField = true;
			}
			
			AffectedActor->GravityFields.Remove(this);
    
			if (wasActiveField)
			{
				if (AffectedActor->GravityFields.Num() > 0)
				{
					UBaseGravityFieldComponent* NewActiveField = AffectedActor->GetActiveGravityField();
					FVector GravityVector = NewActiveField->CalculateGravityVector(OtherActor->GetActorLocation());
					IGravityAffected::Execute_OnEnterGravityField(OtherActor, GravityVector);
				}
				else
				{
					IGravityAffected::Execute_OnExitGravityField(OtherActor);
				}
			}
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
	if (bShowDebugField && DebugLines)
	{
		DebugLines->Flush();  
		DrawDebugGravityField();
	}
}

void UBaseGravityFieldComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);
	UpdateFieldDimensions();
	RedrawDebugField();
}

void UBaseGravityFieldComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
                         
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UBaseGravityFieldComponent, GravityInfluenceRange))
	{
		UpdateFieldDimensions();
		RedrawDebugField();
	}
}

void UBaseGravityFieldComponent::BeginDestroy()
{
	if (GravityVolume)
	{
		GravityVolume->OnComponentBeginOverlap.RemoveDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeBeginOverlap);
		GravityVolume->OnComponentEndOverlap.RemoveDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeEndOverlap);
	}
	Super::BeginDestroy();
}

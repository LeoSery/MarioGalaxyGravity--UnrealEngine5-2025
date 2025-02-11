#include "PlaneGravityFieldComponent.h"

#include "Components/BoxComponent.h"

UPlaneGravityFieldComponent::UPlaneGravityFieldComponent()
{
	UBoxComponent* BoxVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("GravityVolume"));
	GravityVolume = BoxVolume;
	GravityVolume->SetupAttachment(this);
	
	GravityVolume->SetCollisionProfileName(TEXT("OverlapAll"));
	GravityVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GravityVolume->SetGenerateOverlapEvents(true);

	
	BoxVolume->SetBoxExtent(FVector(1000.0f, 1000.0f, 500.0f));

	GravityVolume->OnComponentBeginOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeBeginOverlap);
	GravityVolume->OnComponentEndOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeEndOverlap);
}

void UPlaneGravityFieldComponent::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawPlane(
			CurrentDimensions.Center,
			GetUpVector(), 
			CurrentDimensions.Size.X,
			CurrentDimensions.Size.Z,
			FColor::Orange
		);
	}
}

FVector UPlaneGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
	return -GetUpVector() * GravityStrength;
}

UBaseGravityFieldComponent::FGravityFieldDimensions UPlaneGravityFieldComponent::CalculateFieldDimensions() const
{
	FGravityFieldDimensions Dimensions;
	
	float baseSize = 0.0f;
	if (AActor* Owner = GetOwner())
	{
		if (UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>())
		{
			FVector Extent = MeshComp->Bounds.BoxExtent;
			baseSize = FMath::Max(Extent.X, Extent.Y) * 2.0f;
		}
	}
    
	Dimensions.Size = FVector(baseSize, baseSize, GravityInfluenceRange);
	Dimensions.Center = GetComponentLocation();
    
	return Dimensions;
}

void UPlaneGravityFieldComponent::UpdateGravityVolume()
{
	if (UBoxComponent* BoxVolume = Cast<UBoxComponent>(GravityVolume))
	{
		BoxVolume->SetBoxExtent(CurrentDimensions.Size * 0.5f);
		FVector AdjustedCenter = CurrentDimensions.Center + FVector(0, 0, CurrentDimensions.Size.Z * 0.5f);
		BoxVolume->SetWorldLocation(AdjustedCenter);
	}
}

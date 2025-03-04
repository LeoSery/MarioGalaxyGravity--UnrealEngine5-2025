﻿#include "SphereGravityFieldComponent.h"
#include "Components/SphereComponent.h"

USphereGravityFieldComponent::USphereGravityFieldComponent()
{
	USphereComponent* SphereVolume = CreateDefaultSubobject<USphereComponent>(TEXT("GravityVolume"));
	GravityVolume = SphereVolume;
	GravityVolume->SetupAttachment(this);
    
	GravityVolume->SetCollisionProfileName(TEXT("OverlapAll"));
	GravityVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GravityVolume->SetGenerateOverlapEvents(true);

	if (SphereVolume)
	{
		SphereVolume->SetHiddenInGame(false);
		SphereVolume->SetVisibility(true);
	}

	GravityVolume->OnComponentBeginOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeBeginOverlap);
	GravityVolume->OnComponentEndOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeEndOverlap);
}

void USphereGravityFieldComponent::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawSphere(CurrentDimensions.Center, CurrentDimensions.Size.X, 32, FColor::Red);
	}
}

FVector USphereGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
	FVector DirectionToCenter = GetComponentLocation() - TargetLocation;
	return DirectionToCenter.GetSafeNormal() * GravityStrength;
}

UBaseGravityFieldComponent::FGravityFieldDimensions USphereGravityFieldComponent::CalculateFieldDimensions() const
{
	FGravityFieldDimensions Dimensions;
	float radius = 0.0f;
    
	if (AActor* Owner = GetOwner())
	{
		if (UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>())
		{
			FVector Extent = MeshComp->GetStaticMesh()->GetBoundingBox().GetExtent();
			FVector ActorScale = Owner->GetActorScale3D();
			Extent = Extent * ActorScale;
			radius = Extent.GetMax() + GravityInfluenceRange;
		}
	}
    
	Dimensions.Size = FVector(radius);
	Dimensions.Center = GetComponentLocation();
    
	return Dimensions;
}

void USphereGravityFieldComponent::UpdateGravityVolume()
{
	if (USphereComponent* SphereVolume = Cast<USphereComponent>(GravityVolume))
	{
		SphereVolume->SetSphereRadius(CurrentDimensions.Size.X);
		SphereVolume->SetWorldLocation(CurrentDimensions.Center);
		SphereVolume->SetWorldRotation(GetComponentRotation());
	}
}

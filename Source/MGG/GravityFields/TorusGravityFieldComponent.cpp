#include "TorusGravityFieldComponent.h"

#include "Components/SphereComponent.h"
#include "MGG/Planets/BasePlanet.h"
#include "MGG/Utils/MeshGenerator/TorusMeshComponent.h"

UTorusGravityFieldComponent::UTorusGravityFieldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
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

void UTorusGravityFieldComponent::DrawDebugGravityField()
{
	// if (bShowDebugField && currentDrawer && GetOwner())
	// {
	// 	UTorusMeshComponent* TorusMesh = GetOwner()->FindComponentByClass<UTorusMeshComponent>();
	// 	if (TorusMesh)
	// 	{
	// 		float TorusRadius = TorusMesh->TorusRadius;
	// 		float TubeRadius = TorusMesh->TubeRadius;
	// 		
	// 		float ScaleFactor = GetTorusScaleFactor();
	// 		
	// 		TorusRadius *= ScaleFactor;
	// 		TubeRadius *= ScaleFactor;
 //            
	// 		currentDrawer->DrawTorus(GetComponentLocation(), TorusRadius, TubeRadius, 32, FColor::Red);
	// 	}
	// }
}

FVector UTorusGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
    if (AActor* Owner = GetOwner())
    {
	    if (UTorusMeshComponent* TorusMesh = Owner->FindComponentByClass<UTorusMeshComponent>())
        {
            FVector TorusCenter = GetComponentLocation();
	    	
            FVector gu = FVector(0, 0, 1);
            FVector gr = FVector(1, 0, 0);
	    	
            float BaseRadius = TorusMesh->TorusRadius;
            float ScaledRadius = BaseRadius * GetTorusScaleFactor();
	    	
            FVector V = (TargetLocation - TorusCenter).GetSafeNormal();
	    	
            float dotProduct = FVector::DotProduct(V, gr);
            dotProduct = FMath::Clamp(dotProduct, -1.0f, 1.0f);
            float angle = FMath::Acos(dotProduct);
            float sign = FMath::Sign(FVector::DotProduct(FVector::CrossProduct(V, gr), gu));
	    	
            FVector rotatedGr = gr.RotateAngleAxis(angle * sign * 180.0f / PI, gu);
            FVector P = TorusCenter + rotatedGr * ScaledRadius;
            FVector GravityDirection = (P - TargetLocation).GetSafeNormal();
            
            return GravityDirection * GravityStrength;
        }
    }
	
    return FVector(0, 0, -1) * GravityStrength;
}

UBaseGravityFieldComponent::FGravityFieldDimensions UTorusGravityFieldComponent::CalculateFieldDimensions() const
{
	FGravityFieldDimensions Dimensions;
	
	float meshRadiusBase = 0.0f;
	float scaleFactor = GetTorusScaleFactor();
    
	if (AActor* Owner = GetOwner())
	{
		if (UTorusMeshComponent* TorusMesh = Owner->FindComponentByClass<UTorusMeshComponent>())
		{
			meshRadiusBase = TorusMesh->TorusRadius + TorusMesh->TubeRadius;
		}
	}
	
	float actualMeshRadius = meshRadiusBase * scaleFactor;
	float totalRadius = actualMeshRadius + GravityInfluenceRange;
    
	Dimensions.Size = FVector(totalRadius);
	Dimensions.Center = GetComponentLocation();
    
	return Dimensions;
}

void UTorusGravityFieldComponent::UpdateGravityVolume()
{
	if (USphereComponent* SphereVolume = Cast<USphereComponent>(GravityVolume))
	{
		if (AActor* Owner = GetOwner())
		{
			if (UTorusMeshComponent* TorusMesh = Owner->FindComponentByClass<UTorusMeshComponent>())
			{
				float MainRadius = TorusMesh->TorusRadius;
				float TubeRadius = TorusMesh->TubeRadius;
				float TotalRadius = (MainRadius + TubeRadius) * GetTorusScaleFactor() + GravityInfluenceRange;
                
				SphereVolume->SetSphereRadius(TotalRadius);
				SphereVolume->SetWorldLocation(GetComponentLocation());
			}
			else
			{
				SphereVolume->SetSphereRadius(CurrentDimensions.Size.X);
				SphereVolume->SetWorldLocation(CurrentDimensions.Center);
			}
		}
	}
}

float UTorusGravityFieldComponent::GetTorusScaleFactor() const
{
	float ScaleFactor = 1.0f;
	if (AActor* Owner = GetOwner())
	{
		if (ABasePlanet* Planet = Cast<ABasePlanet>(Owner))
		{
			ScaleFactor = Planet->PlanetRadius / 100.0f;
		}
	}
	return ScaleFactor;
}

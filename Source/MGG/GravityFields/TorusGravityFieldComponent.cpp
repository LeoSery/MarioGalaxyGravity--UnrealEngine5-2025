#include "TorusGravityFieldComponent.h"

#include "Components/SphereComponent.h"
#include "MGG/Planets/BasePlanet.h"
#include "MGG/Utils/MeshGenerator/TorusMeshComponent.h"

UTorusGravityFieldComponent::UTorusGravityFieldComponent()
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

void UTorusGravityFieldComponent::DrawDebugGravityField()
{
	if (bShowDebugField && currentDrawer)
	{
		// Obtenez les dimensions du tore depuis le TorusMeshComponent
		float TorusRadius = CurrentDimensions.Size.X * 0.5f;  // Une estimation
		float TubeRadius = TorusRadius * 0.2f;                // Une estimation
        
		if (AActor* Owner = GetOwner())
		{
			if (UTorusMeshComponent* TorusMesh = Owner->FindComponentByClass<UTorusMeshComponent>())
			{
				TorusRadius = TorusMesh->TorusRadius;
				TubeRadius = TorusMesh->TubeRadius;
			}
		}
        
		currentDrawer->DrawTorus(GetComponentLocation(), TorusRadius, TubeRadius, 32, FColor::Red);
	}
}

FVector UTorusGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
	if (AActor* Owner = GetOwner())
	{
		UTorusMeshComponent* TorusMesh = Owner->FindComponentByClass<UTorusMeshComponent>();
		if (TorusMesh)
		{
			// Position relative au centre du tore
			FVector LocalPosition = TargetLocation - GetComponentLocation();
            
			// Projetez la position sur le plan XY
			FVector ProjectedPosition = FVector(LocalPosition.X, LocalPosition.Y, 0);
			float DistanceFromCenter = ProjectedPosition.Size();
            
			// Direction vers l'anneau principal
			FVector DirectionToRing;
            
			if (DistanceFromCenter < 0.001f)  // Si presque au centre
			{
				// Choisissez une direction arbitraire dans le plan XY
				DirectionToRing = FVector(1, 0, 0);
			}
			else
			{
				// Normalisez pour obtenir la direction vers/depuis l'anneau
				DirectionToRing = ProjectedPosition / DistanceFromCenter;
			}
            
			// Distance idéale = rayon du tore
			float IdealDistance = TorusMesh->TorusRadius;
            
			// Composante dans le plan XY (vers l'anneau)
			FVector GravityXY = DirectionToRing * (DistanceFromCenter - IdealDistance);
            
			// Composante en Z (vers le plan XY)
			FVector GravityZ = FVector(0, 0, -LocalPosition.Z);
            
			// Combine les deux composantes
			FVector GravityDirection = (GravityXY + GravityZ).GetSafeNormal();
            
			return GravityDirection * GravityStrength;
		}
	}
    
	// Fallback vers une gravité standard vers le bas
	return FVector(0, 0, -1) * GravityStrength;
}

UBaseGravityFieldComponent::FGravityFieldDimensions UTorusGravityFieldComponent::CalculateFieldDimensions() const
{
	FGravityFieldDimensions Dimensions;
    
	// Valeurs par défaut
	float meshRadiusBase = 0.0f;
	float scaleFactor = 1.0f;
    
	if (AActor* Owner = GetOwner())
	{
		// Récupérer le facteur d'échelle basé sur PlanetRadius
		if (ABasePlanet* Planet = Cast<ABasePlanet>(Owner))
		{
			scaleFactor = Planet->PlanetRadius / 100.0f;
		}
        
		// Récupérer les dimensions du tore
		UTorusMeshComponent* TorusMesh = Owner->FindComponentByClass<UTorusMeshComponent>();
		if (TorusMesh)
		{
			// Rayon de base du tore (sans mise à l'échelle)
			meshRadiusBase = TorusMesh->TorusRadius + TorusMesh->TubeRadius;
		}
	}
    
	// Rayon réel après mise à l'échelle
	float actualMeshRadius = meshRadiusBase * scaleFactor;
    
	// Ajouter l'InfluenceRange
	float totalRadius = actualMeshRadius + GravityInfluenceRange;
    
	Dimensions.Size = FVector(totalRadius);
	Dimensions.Center = GetComponentLocation();
    
	return Dimensions;
}

void UTorusGravityFieldComponent::UpdateGravityVolume()
{
	if (USphereComponent* SphereVolume = Cast<USphereComponent>(GravityVolume))
	{
		SphereVolume->SetSphereRadius(CurrentDimensions.Size.X);
		SphereVolume->SetWorldLocation(CurrentDimensions.Center);
	}
}

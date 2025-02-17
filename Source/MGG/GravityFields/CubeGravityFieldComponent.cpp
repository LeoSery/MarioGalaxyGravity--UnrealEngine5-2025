#include "CubeGravityFieldComponent.h"
#include "Components/BoxComponent.h"

UCubeGravityFieldComponent::UCubeGravityFieldComponent()
{
	UBoxComponent* CubeVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("GravityVolume"));
	GravityVolume = CubeVolume;
	GravityVolume->SetupAttachment(this);

	GravityVolume->SetCollisionProfileName(TEXT("OverlapAll"));
	GravityVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GravityVolume->SetGenerateOverlapEvents(true);
	
	if (CubeVolume)
	{
		CubeVolume->SetHiddenInGame(false);
		CubeVolume->SetVisibility(true);
	}
	
	CubeVolume->SetBoxExtent(FVector(GetTotalGravityRadius()));

	GravityVolume->OnComponentBeginOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeBeginOverlap);
	GravityVolume->OnComponentEndOverlap.AddDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeEndOverlap);
}

void UCubeGravityFieldComponent::DrawDebugGravityField()
{
	UE_LOG(LogTemp, Warning, TEXT("Drawing debug cube gravity field"));
	if (bShowDebugField && currentDrawer)
	{
		currentDrawer->DrawCube(
			CurrentDimensions.Center, 
			CurrentDimensions.Size, 
			GetComponentRotation(), 
			FColor::Red
		);
	}
}

FVector UCubeGravityFieldComponent::CalculateGravityVector(const FVector& TargetLocation) const
{
	FVector RelativePosition = TargetLocation - CurrentDimensions.Center;

	UE_LOG(LogTemp, Warning, TEXT("Target Location: %s"), *TargetLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Center: %s"), *CurrentDimensions.Center.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Relative Position: %s"), *RelativePosition.ToString());

	FVector MeshSize;
	if (AActor* Owner = GetOwner())
	{
		if (UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>())
		{
			MeshSize = MeshComp->Bounds.BoxExtent;
			UE_LOG(LogTemp, Warning, TEXT("Mesh Size: %s"), *MeshSize.ToString());
		}
	}
	
	FCubePositionFlags Flags = CalculatePositionFlags(RelativePosition, MeshSize);
	UE_LOG(LogTemp, Warning, TEXT("Flags - X: %d, Y: %d, Z: %d"), Flags.X, Flags.Y, Flags.Z);
	
	int32 OutsideAxesCount = (Flags.X != FCubePositionFlags::Inside ? 1 : 0) + (Flags.Y != FCubePositionFlags::Inside ? 1 : 0) + (Flags.Z != FCubePositionFlags::Inside ? 1 : 0);
    
	FVector GravityVector;
	
	if (OutsideAxesCount == 1)
	{
		GravityVector = ConstructGravityComponentVector(Flags, FVector(1.0f));
	}
	else if (OutsideAxesCount > 1)
	{
		FVector BlendFactors = CalculateBlendFactors(RelativePosition, MeshSize, Flags);
		GravityVector = ConstructGravityComponentVector(Flags, BlendFactors);
	}
	
	return GravityVector.GetSafeNormal() * GravityStrength;
}

UBaseGravityFieldComponent::FGravityFieldDimensions UCubeGravityFieldComponent::CalculateFieldDimensions() const
{
	FGravityFieldDimensions Dimensions;
	float Radius = 0.0f;

	if (AActor* Owner = GetOwner())
	{
		if (UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>())
		{
			FVector Extent = MeshComp->GetStaticMesh()->GetBoundingBox().GetExtent();
			FVector ActorScale = Owner->GetActorScale3D();
			Extent = Extent * ActorScale;
			Radius = FMath::Max3(Extent.X, Extent.Y, Extent.Z) + GravityInfluenceRange;
		}
	}
	
	Dimensions.Size = FVector(Radius);
	Dimensions.Center = GetComponentLocation();
    
	return Dimensions;
}

void UCubeGravityFieldComponent::UpdateGravityVolume()
{
	if (UBoxComponent* CubeVolume = Cast<UBoxComponent>(GravityVolume))
	{
		CubeVolume->SetBoxExtent(CurrentDimensions.Size);
		CubeVolume->SetWorldLocation(CurrentDimensions.Center);
		CubeVolume->SetWorldRotation(GetComponentRotation());
	}
}

UCubeGravityFieldComponent::FCubePositionFlags UCubeGravityFieldComponent::CalculatePositionFlags(const FVector& RelativePosition, const FVector& Extent) const
{
	FCubePositionFlags Flags = {FCubePositionFlags::Inside, FCubePositionFlags::Inside, FCubePositionFlags::Inside};
	
	if (RelativePosition.X < -Extent.X)
	{
		Flags.X = FCubePositionFlags::Behind;
	}
	else if (RelativePosition.X > Extent.X)
	{
		Flags.X = FCubePositionFlags::Forward;
	}
    
	if (RelativePosition.Y < -Extent.Y)
	{
		Flags.Y = FCubePositionFlags::Behind;
	}
	else if (RelativePosition.Y > Extent.Y)
	{
		Flags.Y = FCubePositionFlags::Forward;
	}
    
	if (RelativePosition.Z < -Extent.Z)
	{
		Flags.Z = FCubePositionFlags::Behind;
	}
	else if (RelativePosition.Z > Extent.Z)
	{
		Flags.Z = FCubePositionFlags::Forward;
	}

	return Flags;
}

FVector UCubeGravityFieldComponent::CalculateBlendFactors(const FVector& RelativePosition, const FVector& Extent, const FCubePositionFlags& Flags) const
{
	const float EdgeBlendDistance = Extent.X * 0.1f;
    
	FVector BlendFactors;
	
	if (Flags.X != FCubePositionFlags::Inside)
	{
		float Distance = FMath::Abs(RelativePosition.X) - (Extent.X - EdgeBlendDistance);
		BlendFactors.X = FMath::Clamp(Distance / EdgeBlendDistance, 0.0f, 1.0f);
	}
    
	if (Flags.Y != FCubePositionFlags::Inside)
	{
		float Distance = FMath::Abs(RelativePosition.Y) - (Extent.Y - EdgeBlendDistance);
		BlendFactors.Y = FMath::Clamp(Distance / EdgeBlendDistance, 0.0f, 1.0f);
	}
    
	if (Flags.Z != FCubePositionFlags::Inside)
	{
		float Distance = FMath::Abs(RelativePosition.Z) - (Extent.Z - EdgeBlendDistance);
		BlendFactors.Z = FMath::Clamp(Distance / EdgeBlendDistance, 0.0f, 1.0f);
	}
    
	return BlendFactors;
}

FVector UCubeGravityFieldComponent::ConstructGravityComponentVector(const FCubePositionFlags& Flags, const FVector& Factors) const
{
	return FVector(
		Flags.X == FCubePositionFlags::Forward ? -Factors.X : Flags.X == FCubePositionFlags::Behind ? Factors.X : 0.0f,
		Flags.Y == FCubePositionFlags::Forward ? -Factors.Y : Flags.Y == FCubePositionFlags::Behind ? Factors.Y : 0.0f,
		Flags.Z == FCubePositionFlags::Forward ? -Factors.Z : Flags.Z == FCubePositionFlags::Behind ? Factors.Z : 0.0f
	);
}

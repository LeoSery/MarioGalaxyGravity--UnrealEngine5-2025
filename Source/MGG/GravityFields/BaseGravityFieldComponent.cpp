#include "BaseGravityFieldComponent.h"
#include "MGG//Utils/Interfaces/GravityAffected.h"
#include "Components/LineBatchComponent.h"
#include "Components/ShapeComponent.h"
#include "MGG/Utils/Drawers/GravityFieldDrawer.h"

/**
 * @brief Constructor for the base gravity field component.
 *
 * @details Initializes the component with default values, creates a debug line component
 * and sets initial gravity parameters.
 */
UBaseGravityFieldComponent::UBaseGravityFieldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	GravityStrength = 9.81f;
	GravityFieldPriority = 0;
    
	DebugLines = CreateDefaultSubobject<ULineBatchComponent>(TEXT("DebugLines"));
}

/**
 * @brief Called when the component is registered with the scene.
 *
 * @details Initializes the field dimensions and sets up the debug drawing system
 * if debug visualization is enabled.
 */
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

/**
 * @brief Updates the dimensions of the gravity field.
 *
 * @details Calculates the current dimensions of the gravity field based on the owner's
 * properties and updates the collision volume accordingly.
 */
void UBaseGravityFieldComponent::UpdateFieldDimensions()
{
	CurrentDimensions = CalculateFieldDimensions();
	UpdateGravityVolume();
}

/**
 * @brief Handles an actor entering the gravity field.
 *
 * @details When an actor implementing the IGravityAffected interface enters the gravity field,
 * this method adds the field to the actor's list of active fields and notifies the actor.
 *
 * @param OverlappedComp The component that was overlapped.
 * @param OtherActor The actor that entered the field.
 * @param OtherComp The component of the actor that caused the overlap.
 * @param OtherBodyIndex The body index of the component that caused the overlap.
 * @param bFromSweep Whether the overlap was triggered by a sweep.
 * @param SweepResult Information about the sweep that triggered the overlap.
 */
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

/**
 * @brief Handles an actor exiting the gravity field.
 *
 * @details When an actor implementing the IGravityAffected interface exits the gravity field,
 * this method removes the field from the actor's list of active fields and updates the actor's
 * gravity if necessary.
 *
 * @param OverlappedComponent The component that was overlapped.
 * @param OtherActor The actor that exited the field.
 * @param OtherComp The component of the actor that ended the overlap.
 * @param OtherBodyIndex The body index of the component that ended the overlap.
 */
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

/**
 * @brief Calculates the total radius of gravity influence.
 *
 * @details Determines the maximum radius of influence for this gravity field,
 * taking into account both the physical mesh dimensions and the additional influence range.
 *
 * @return The total radius of gravity influence in world units.
 */
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

/**
 * @brief Redraws the debug visualization of the gravity field.
 *
 * @details Clears previous debug drawings and calls DrawDebugGravityField to create
 * a new visualization of the current gravity field if debug visualization is enabled.
 */
void UBaseGravityFieldComponent::RedrawDebugField()
{
	if (bShowDebugField && DebugLines)
	{
		DebugLines->Flush();  
		DrawDebugGravityField();
	}
}

/**
 * @brief Called when the component's transform is updated.
 *
 * @details Updates the field dimensions and debug visualization to reflect
 * the component's new position and orientation.
 * 
 * @param UpdateTransformFlags Flags indicating what aspects of the transform changed.
 * @param Teleport The type of teleportation that occurred, if any.
 */
void UBaseGravityFieldComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);
	UpdateFieldDimensions();
	RedrawDebugField();
}

/**
 * @brief Called when a property of the component is changed in the editor.
 *
 * @details Updates the field dimensions and debug visualization if relevant properties
 * such as the gravity influence range are modified.
 *
 * @param PropertyChangedEvent Information about the property that was changed.
 */
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

/**
 * @brief Called before the component is destroyed.
 *
 * @details Cleans up resources and unregisters event handlers to prevent memory leaks
 * and unexpected behavior during destruction.
 */
void UBaseGravityFieldComponent::BeginDestroy()
{
	if (GravityVolume)
	{
		GravityVolume->OnComponentBeginOverlap.RemoveDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeBeginOverlap);
		GravityVolume->OnComponentEndOverlap.RemoveDynamic(this, &UBaseGravityFieldComponent::OnGravityVolumeEndOverlap);
	}
	Super::BeginDestroy();
}

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GravityAffected.generated.h"

//////// FORWARD DECLARATION ////////
//// Class
class UBaseGravityFieldComponent;

/**
 * @brief Interface for objects that can be affected by gravity fields.
 *
 * @details This interface defines the methods that must be implemented by any
 * actor that should respond to the custom gravity system. It enables seamless
 * interaction between gravity field components and objects moving in the game world.
 */
UINTERFACE()
class UGravityAffected : public UInterface
{
	GENERATED_BODY()
};

class MGG_API IGravityAffected
{
	GENERATED_BODY()

public:
	//////// INTERFACE METHODS ////////
	//// Gravity methods

	/**
	 * @brief Returns a reference to the current gravity vector affecting this object.
	 *
	 * @details This method should provide access to the object's internal gravity vector,
	 * which determines the direction and strength of gravitational pull currently affecting it.
	 * This vector is typically used in the object's movement calculations.
	 *
	 * @return Reference to the current gravity vector.
	 */
	virtual FVector& GetGravityVector() = 0;

	/**
	 * @brief Updates the object's current gravity field based on active fields.
	 *
	 * @details This method should evaluate all gravity fields currently affecting the object
	 * and determine which one should be active (typically the highest priority field).
	 * It then updates the object's gravity vector based on the selected field's calculations.
	 * 
	 * This should be called regularly (typically each frame) to ensure the object
	 * always responds to the appropriate gravity field as it moves through the world.
	 */
	virtual void UpdateCurrentGravityField() = 0;

	//// Gravity events
	/**
	 * @brief Notifies the object that it has entered a gravity field.
	 *
	 * @details This event is triggered when the object enters a new gravity field.
	 * The implementation should update the object's physics properties, orientation,
	 * and movement behavior to respond to the new gravity direction.
	 *
	 * @param NewGravityVector The gravity vector of the entered field.
	 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnEnterGravityField(const FVector& NewGravityVector);
	virtual void OnEnterGravityField_Implementation(const FVector& NewGravityVector) = 0;

	/**
	 * @brief Notifies the object that it has exited a gravity field.
	 *
	 * @details This event is triggered when the object exits a gravity field completely.
	 * The implementation should revert to default gravity or handle the transition
	 * to zero-gravity as appropriate for the game mechanics.
	 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnExitGravityField();
	virtual void OnExitGravityField_Implementation() = 0;

	//////// FIELDS ////////
	//// Gravity fields
	TArray<UBaseGravityFieldComponent*> GravityFields;

	//////// METHODS ////////
	//// Helper methods
	UBaseGravityFieldComponent* GetActiveGravityField();
};

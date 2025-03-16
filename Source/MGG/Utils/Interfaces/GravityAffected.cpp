#include "GravityAffected.h"
#include "MGG/GravityFields/BaseGravityFieldComponent.h"

/**
 * @brief Gets the active gravity field affecting this object.
 *
 * @details Analyzes all gravity fields currently overlapping with this object
 * and determines which one has the highest priority. This method is used internally
 * to select which gravity field should be used for physics calculations.
 * 
 * The priority system allows for complex scenarios where multiple gravity fields
 * overlap, ensuring that the most relevant field (e.g., a small planet's gravity
 * overriding a larger background gravity) affects the object.
 *
 * @return Pointer to the highest priority gravity field, or nullptr if no fields are active.
 */
UBaseGravityFieldComponent* IGravityAffected::GetActiveGravityField()
{
	if (GravityFields.Num() == 0)
	{
		return nullptr;
	}
	
	UBaseGravityFieldComponent* ActiveField = GravityFields.Last();
	int32 HighestPriority = ActiveField->GetGravityFieldPriority();

	for (auto* Field : GravityFields)
	{
		if (Field->GetGravityFieldPriority() > HighestPriority)
		{
			HighestPriority = Field->GetGravityFieldPriority();
			ActiveField = Field;
		}
	}
    
	return ActiveField;	
}

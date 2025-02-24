#include "GravityAffected.h"
#include "MGG/GravityFields/BaseGravityFieldComponent.h"

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

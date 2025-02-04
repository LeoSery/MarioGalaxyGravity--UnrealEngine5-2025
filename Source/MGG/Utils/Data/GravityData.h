#pragma once

UENUM(BlueprintType)
enum EGravityFieldType
{
	NONE,
	PLANE UMETA(DisplayName = "Plane"),
	SPHERE UMETA(DisplayName = "Sphere"),
	CUBE UMETA(DisplayName = "Cube"),
	TORUS UMETA(DisplayName = "Torus"),    
};
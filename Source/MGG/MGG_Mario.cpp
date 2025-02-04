// Fill out your copyright notice in the Description page of Project Settings.


#include "MGG_Mario.h"
#include "InputAction.h"

// Sets default values
AMGG_Mario::AMGG_Mario()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMGG_Mario::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMGG_Mario::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {

		const FVector ForwardDirection = FRotator(0, GetWorld()->GetFirstPlayerController()->PlayerCameraManager.Get()->GetCameraRotation().Yaw, 0).Vector();

		const FVector RightDirection = ForwardDirection.RotateAngleAxis(90, { 0,0,1 });

		Velocity += ForwardDirection + RightDirection;

	}
}

void AMGG_Mario::Look(const FInputActionValue& Value)
{
}

void AMGG_Mario::Jump()
{
}

// Called every frame
void AMGG_Mario::Tick(float DeltaTime)
{
	PhysicProcess(DeltaTime);


	Super::Tick(DeltaTime);

}

void AMGG_Mario::PhysicProcess(float DeltaTime)
{
	FVector pos = GetActorLocation();



	Velocity += GravityVector * DeltaTime;

	pos += Velocity * DeltaTime;


}

// Called to bind functionality to input
void AMGG_Mario::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "LumiCameraThird.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ALumiCameraThird::ALumiCameraThird()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	LumiCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("LumiCamera"));
	springArmCom = CreateDefaultSubobject<USpringArmComponent>(TEXT("LumiSpringArm"));

	springArmCom->SetupAttachment(RootComponent);
	springArmCom->SetRelativeLocationAndRotation(FVector(0.0f, 0.f, 130.f), FRotator(-30.f, 0.f, 0.f));
	springArmCom->TargetArmLength = 400.f;
	springArmCom->CameraLagSpeed = 3.f;

	LumiCamera->SetupAttachment(springArmCom, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ALumiCameraThird::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALumiCameraThird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALumiCameraThird::LookUp(float Value)
{
	FRotator NewRotator = springArmCom->GetRelativeRotation();
	NewRotator.Pitch = FMath::Clamp(NewRotator.Pitch - Value, -90.f, 0.f);
	springArmCom->SetRelativeRotation(NewRotator);
}
void ALumiCameraThird::LookHorizon(float Value)
{
	FRotator NewRotator = springArmCom->GetRelativeRotation();
	NewRotator.Yaw += Value;
	springArmCom->SetRelativeRotation(NewRotator);
}

FRotator ALumiCameraThird::GetSpringArmRotation()
{
	return springArmCom->GetRelativeRotation();
}

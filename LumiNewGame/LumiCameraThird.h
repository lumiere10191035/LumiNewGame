// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "LumiCameraThird.generated.h"

UCLASS()
class LUMINEWGAME_API ALumiCameraThird : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALumiCameraThird();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* LumiCamera;

	UPROPERTY(EditAnywhere);
	class USpringArmComponent* springArmCom;

	void LookUp(float Value);
	void LookHorizon(float Value);

	FRotator GetSpringArmRotation();
	
};

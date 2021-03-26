// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "LumiEnemyUnit.generated.h"

UCLASS()
class LUMINEWGAME_API ALumiEnemyUnit : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALumiEnemyUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleDefaultsOnly, Category = BoxCollision)
	UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category = BodyMesh)
	UStaticMeshComponent* BodyMeshComp;

	UFUNCTION()
	void GetDamageBySkill(int damage);

protected:
	int LifePoint;
};

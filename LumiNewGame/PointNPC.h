// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "ReadImportData.h"
#include "PointNPC.generated.h"

UCLASS()
class LUMINEWGAME_API APointNPC : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APointNPC();
	APointNPC(FString meshPath);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnStartOverlap(UPrimitiveComponent* OverlapppedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	void AttachStaticMesh(FString path);
	void UpdateInitData(const FPointObjData& _data);

	//Point Mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	// Collision 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	USphereComponent* ProxSphere;

	// Parameter
	int Score;
	int Exp;

};

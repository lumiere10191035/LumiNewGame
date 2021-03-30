// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ReadImportData.h"
#include "LumiEnemyBullet.generated.h"

UCLASS()
class LUMINEWGAME_API ALumiEnemyBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALumiEnemyBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void DestroyBullet();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	void InitEnemySphereInfo(const FSkillData& _data, int _charaAtk, int _num);
	void FireInDirection(const FVector& _direction);

	UPROPERTY(VisibleDefaultsOnly, Category = SkillSphere)
	USphereComponent* SkillSphereComp;

	UPROPERTY(EditAnywhere, Category = SkillSphere)
	UStaticMeshComponent* SkillMesh;

	UPROPERTY(VisibleAnywhere, Category = SkillSphere)
	UProjectileMovementComponent* moveComponent;

protected:
	float bulletLife;
	float bulletCurLife;
	int SkillDamage;
	int SkillType;
	int CharaATK;

	float WaitTime;
	float curWaitTime;

	FVector direction;
};

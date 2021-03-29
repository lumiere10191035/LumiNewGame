// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ReadImportData.h"
#include "LumiUtility.h"
#include "LumiUMG/UMG_EnemyHP.h"
#include "Components/WidgetComponent.h"
#include "LumiEnemyUnit.generated.h"

struct EnemyStatus {
	GAME_SKILL_TYPE enemyType = SKILL_TYPE_DEFAULT;
	int MaxHp = 0;
	int curHp = 0;
	int MaxMp = 0;
	int curMp = 0;
	float MaxSpeed = 0.f;
	float curSpeed = 0.f;
	float MaxMoveTime = 0.f;
	float curMoveTime = 0.f;
	int skill_Id = 0;
	float AlarmDis = 0.f;
	int UnitExp = 0;
	int UnitScore = 0;
	FString EnemyName = TEXT("");
};

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

	UPROPERTY(VisibleDefaultsOnly, Category = BoxCollision)
	UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category = BodyMesh)
	UStaticMeshComponent* BodyMeshComp;

	UFUNCTION()
	void GetDamageBySkill(int _type, int damage);

	void InitEnemyUnit(const FEnemyData& _data);
	void UpdateLifeBarRotation();

	UPROPERTY(EditAnywhere, Category = LifeBar)
	TSubclassOf<UUserWidget> LifeBarClass;
	
	UPROPERTY(EditAnywhere, Category = WidgetComponent)
	UWidgetComponent* LifeBar;

	bool bLifeBarShow;
	

protected:
	EnemyStatus unitStatus;
	float releaseTime;
	float curReleaseTime;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "LumiEnemyUnit.h"
#include "LumiNewGameGameModeBase.h"

// Sets default values
ALumiEnemyUnit::ALumiEnemyUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EnemyUnit"));
	}
	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

		CollisionComponent->SetupAttachment(RootComponent);
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	BodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMeshComp->SetupAttachment(CollisionComponent);
	BodyMeshComp->SetRelativeLocation(FVector(0.f));
}

// Called when the game starts or when spawned
void ALumiEnemyUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALumiEnemyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALumiEnemyUnit::GetDamageBySkill(int _type, int _damage)
{
	ALumiNewGameGameModeBase* gameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	float damageRatio = gameMode->GetSkillDamageRatio(_type, unitStatus.enemyType);
	damageRatio = damageRatio == 0 ? 1 : damageRatio;
	int trueDamage = FMath::FloorToInt((float)_damage / damageRatio);
	unitStatus.curHp -= trueDamage;
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Log::Enemy Damage %d"), trueDamage > unitStatus.curHp ? unitStatus.curHp : trueDamage));
	if (unitStatus.curHp <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Enemy Die!"));
		gameMode->charaExpDelegate.ExecuteIfBound(unitStatus.UnitExp);
		gameMode->scoreDelegate.ExecuteIfBound(unitStatus.UnitScore);

		Destroy();
	}
}

void ALumiEnemyUnit::InitEnemyUnit(const FEnemyData& _data)
{
	unitStatus.MaxHp = _data.HP;
	unitStatus.curHp = unitStatus.MaxHp;
	unitStatus.MaxMp = _data.MP;
	unitStatus.curMp = unitStatus.MaxMp;
	unitStatus.MaxSpeed = _data.Speed;
	unitStatus.curSpeed = 0.f;
	unitStatus.skill_Id = _data.Skill_Id;
	unitStatus.MaxMoveTime = _data.MoveTime;
	unitStatus.curMoveTime = 0.f;
	unitStatus.AlarmDis = _data.AlarmDistance;
	unitStatus.UnitExp = _data.UnitExp;
}
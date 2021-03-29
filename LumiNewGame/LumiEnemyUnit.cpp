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

	LifeBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("LifeBar"));
	LifeBar->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
	LifeBar->SetRelativeLocation(FVector(0.f, 0.f, -50.f));

	LifeBarClass = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/UMG/Lumi_EnemyHP.Lumi_EnemyHP_C'"));

	//lifebar;
	bLifeBarShow = false;
	releaseTime = 5.f;
	curReleaseTime = -1.f;
}

// Called when the game starts or when spawned
void ALumiEnemyUnit::BeginPlay()
{
	Super::BeginPlay();
	
	//if (LifeBarClass && LifeBar)
	//{
	//	LifeBar->SetWidgetClass(LifeBarClass);

		//UUMG_EnemyHP* enemyHPClass = Cast<UUMG_EnemyHP>(LifeBar->GetUserWidgetObject());
		//enemyHPClass->UpdateHPBar(1);
	//}
}

// Called every frame
void ALumiEnemyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// update pate;
	if (bLifeBarShow)
	{
		// check release pate
		curReleaseTime += DeltaTime;
		if (curReleaseTime >= releaseTime)
		{
			LifeBar->SetWidgetClass(nullptr);

			curReleaseTime = -1.f;
			bLifeBarShow = false;
		}
		else
		{
			UpdateLifeBarRotation();
		}
	}
}

void ALumiEnemyUnit::GetDamageBySkill(int _type, int _damage)
{
	ALumiNewGameGameModeBase* gameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	float damageRatio = gameMode->GetSkillDamageRatio(_type, unitStatus.enemyType);
	FString text = FString::Printf(TEXT("EnemyTYpe::%d, Attacktype::%d, DamageRatio::%.1f"), unitStatus.enemyType, _type, damageRatio);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, text);

	int trueDamage = FMath::FloorToInt((float)_damage * damageRatio);
	unitStatus.curHp -= trueDamage;
	
	//UI
	if (!bLifeBarShow && LifeBarClass)
	{
		LifeBar->SetWidgetClass(LifeBarClass);
	}

	UUMG_EnemyHP* enemyHPBar = Cast<UUMG_EnemyHP>(LifeBar->GetUserWidgetObject());
	if (enemyHPBar != nullptr)
	{
		if (!bLifeBarShow)
		{
			bLifeBarShow = true;
			enemyHPBar->InitEnemyData(unitStatus.EnemyName, unitStatus.MaxHp, unitStatus.curHp);
		}
		else
		{
			enemyHPBar->UpdateHPBar(unitStatus.curHp);
		}
	}
	curReleaseTime = 0.f;
	
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
	unitStatus.UnitScore = _data.UnitScore;
	unitStatus.EnemyName = _data.EnemyName;

	unitStatus.enemyType = (GAME_SKILL_TYPE)_data.EnemyType;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::FromInt(_data.EnemyType));
}

void ALumiEnemyUnit::UpdateLifeBarRotation()
{
	ALumiNewGameGameModeBase* gameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	FRotator cameraRot = gameMode->LumiCameraActor->GetSpringArmRotation();
	FVector newVector = -cameraRot.Vector();

	if (LifeBar != nullptr)
	{
		FRotator originRot = GetActorRotation();
		FVector originVec = originRot.Vector();

		FRotator targetRot = newVector.ToOrientationRotator();
		FRotator nextRot = targetRot - originRot;

		LifeBar->SetRelativeRotation(nextRot);
	}
}
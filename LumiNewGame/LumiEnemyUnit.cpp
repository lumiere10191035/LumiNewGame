// Fill out your copyright notice in the Description page of Project Settings.


#include "LumiEnemyUnit.h"
#include "LumiEnemyBullet.h"
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

}

// Called every frame
void ALumiEnemyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ALumiNewGameGameModeBase* gameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	FVector cameraPos = gameMode->LumiCameraActor->GetActorLocation();
	float distance = FVector::Dist2D(GetActorLocation(), cameraPos);
	// alarming:
	if (unitStatus.moveState != ENEMY_MOVEMENT_ATTACK_WAIT && distance < unitStatus.AlarmDis)
	{
		unitStatus.saveState = unitStatus.moveState;
		unitStatus.moveState = ENEMY_MOVEMENT_ATTACK_WAIT;
		unitStatus.curAttackWaitTime = unitStatus.attackWaitTime;

		FRotator shootDir = (GetActorLocation() - cameraPos).ToOrientationRotator();
		SetActorRotation(shootDir);
	}
	else if (unitStatus.moveState == ENEMY_MOVEMENT_ATTACK_WAIT && distance > unitStatus.AlarmDis)
	{
		unitStatus.moveState = unitStatus.saveState;
		unitStatus.curAttackWaitTime = 0;

		SetActorRotation(unitStatus.moveDir.ToOrientationRotator());
	}

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

	FVector NewLocation = GetActorLocation();
	// enemy move
	switch (unitStatus.moveState)
	{
	case ENEMY_MOVEMENT_MOVE_WAIT:
		unitStatus.curWaitTime += DeltaTime;
		if (unitStatus.curWaitTime >= unitStatus.waitTime)
		{
			StartMovement();
		}
		break;
	case ENEMY_MOVEMENT_MOVE:
		unitStatus.curMoveTime += DeltaTime;
		NewLocation += unitStatus.moveDir * unitStatus.MaxSpeed * DeltaTime;
		SetActorLocation(NewLocation);
		if (unitStatus.curMoveTime >= unitStatus.MaxMoveTime)
		{
			StartMovementWait();
		}
		break;
	case ENEMY_MOVEMENT_ATTACK_WAIT:
		unitStatus.curAttackWaitTime += DeltaTime;
		if (unitStatus.curAttackWaitTime >= unitStatus.attackWaitTime)
		{
			unitStatus.curAttackWaitTime = 0.f;
			SkillShoot();
		}
		break;
	default:
		break;
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
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Log::Enemy Damage %d"), trueDamage > unitStatus.curHp ? unitStatus.curHp : trueDamage));
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

	unitStatus.waitTime = 3.f;
	unitStatus.curWaitTime = -1.f;
	unitStatus.attackWaitTime = 2.f;
	unitStatus.curAttackWaitTime = -1.f;

	unitStatus.enemyType = (GAME_SKILL_TYPE)_data.EnemyType;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::FromInt(_data.EnemyType));

	// random status;
	float r_p = FMath::FRandRange(-180.f, 180.f);
	FRotator MoveRotation = FRotator(0.f, r_p, 0.f);
	unitStatus.moveDir = MoveRotation.Vector();

	float randNum = FMath::FRandRange(0.f, unitStatus.waitTime + unitStatus.MaxMoveTime);
	if (randNum < unitStatus.waitTime)
	{
		unitStatus.moveState = ENEMY_MOVEMENT_MOVE_WAIT;
		unitStatus.curWaitTime = randNum;
	}
	else
	{
		unitStatus.moveState = ENEMY_MOVEMENT_MOVE;
		unitStatus.curMoveTime = randNum - unitStatus.waitTime;
	}

	// init bullet skill
	ALumiNewGameGameModeBase* gameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));

	bulletData = gameMode->gameUtility->EnemySkillData[unitStatus.skill_Id - 1];
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

void ALumiEnemyUnit::StartMovement()
{
	unitStatus.moveState = ENEMY_MOVEMENT_MOVE;
	unitStatus.moveDir = -unitStatus.moveDir;
	unitStatus.curWaitTime = 0.f;

	FRotator actorRot = unitStatus.moveDir.Rotation();
	SetActorRotation(actorRot);
}

void ALumiEnemyUnit::StartMovementWait()
{
	unitStatus.moveState = ENEMY_MOVEMENT_MOVE_WAIT;
	unitStatus.curMoveTime = 0.f;
}

//shoot
void ALumiEnemyUnit::SkillShoot()
{
	int skillId = unitStatus.skill_Id;
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (bulletData.Skill_Id == 0)
	{
		return;
	}

	unitStatus.curAttackWaitTime = 0.f;

	FVector targetPos = lumiGameMode->LumiCameraActor->GetActorLocation();
	FVector StartVector = GetActorLocation() + FVector(0.f, 0.f, 180.f);
	FRotator BulletRotation = (targetPos - StartVector).Rotation();

	FVector MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(50.f, 0.f, 0.f));
	FRotator MuzzleRotation = BulletRotation;

	TSubclassOf<class ALumiEnemyBullet> LumiBulletClass = lumiGameMode->LumiEnemyBulletClassList[skillId - 1];
	if (LumiBulletClass)
	{
		switch (bulletData.BallNum)
		{
		case 1:
			LaunchBallSkill(MuzzleLocation, MuzzleRotation, 1);
			break;
		case 2:
			MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(50.f, -10.f, -20.f));
			LaunchBallSkill(MuzzleLocation, MuzzleRotation, 1);
			MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(50.f, 10.f, -20.f));
			LaunchBallSkill(MuzzleLocation, MuzzleRotation, 2);
			break;
		default:
			break;
		}
	}
}

void ALumiEnemyUnit::LaunchBallSkill(FVector startLocation, FRotator startRotation, int num)
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	TSubclassOf<class ALumiEnemyBullet> LumiBulletClass = lumiGameMode->LumiEnemyBulletClassList[unitStatus.skill_Id - 1];
	if (LumiBulletClass)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();
		ALumiEnemyBullet* bullet = GetWorld()->SpawnActor<ALumiEnemyBullet>(LumiBulletClass, startLocation, startRotation, spawnParams);
		if (bullet)
		{
			bullet->InitEnemySphereInfo(bulletData, 0, num);
			bullet->FireInDirection(startRotation.Vector());
		}
	}
}

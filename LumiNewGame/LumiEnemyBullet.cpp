// Fill out your copyright notice in the Description page of Project Settings.


#include "LumiEnemyBullet.h"
#include "LumiEnemyUnit.h"
#include "LumiCharacter.h"
#include "LumiNewGameGameModeBase.h"

// Sets default values
ALumiEnemyBullet::ALumiEnemyBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LumiSkillScene"));
	}
	if (!SkillSphereComp)
	{
		SkillSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereMesh"));
		SkillSphereComp->SetupAttachment(RootComponent);
		SkillSphereComp->OnComponentHit.__Internal_AddDynamic(this, &ALumiEnemyBullet::OnHit, "OnHit");
		SkillSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	if (!SkillMesh)
	{
		SkillMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LumiMesh"));
		SkillMesh->SetupAttachment(SkillSphereComp);
		SkillMesh->SetRelativeLocation(FVector(0.f));
		SkillMesh->SetRelativeScale3D(FVector(1.0f));
	}
	if (!moveComponent)
	{
		moveComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComponent"));
		moveComponent->SetUpdatedComponent(SkillSphereComp);
	}

	bulletCurLife = 0.0f;
	bulletLife = 10000.f;
}

// Called when the game starts or when spawned
void ALumiEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALumiEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bulletCurLife += DeltaTime;
	if (bulletCurLife >= bulletLife)
	{
		DestroyBullet();
	}

	// wait time
	if (curWaitTime > -1.f)
	{
		curWaitTime += DeltaTime;
		if (curWaitTime >= WaitTime)
		{
			moveComponent->Velocity = direction * moveComponent->InitialSpeed;

			curWaitTime = -1;
		}
	}
}

void ALumiEnemyBullet::InitEnemySphereInfo(const FSkillData& _data, int _charaAtk, int _num)
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (SkillSphereComp)
	{
		SkillSphereComp->InitSphereRadius(_data.BallRadius);
	}
	if (SkillMesh)
	{
		float scale = _data.BallRadius / 32.f * 0.1f;
		SkillMesh->SetRelativeScale3D(FVector(scale));
	}
	if (moveComponent)
	{
		moveComponent->InitialSpeed = _data.BallSpeed;
		moveComponent->MaxSpeed = _data.BallSpeed;
		moveComponent->bRotationFollowsVelocity = true;
		moveComponent->bShouldBounce = false;
		moveComponent->ProjectileGravityScale = 0.f;

		bulletLife = _data.BallTime;
		SkillDamage = _data.Damage;
		SkillType = _data.SkillType;
		CharaATK = _charaAtk;

		float startWaitTime = _data.StartWait;
		float singleWaitTime = _data.SingleWait;
		WaitTime = startWaitTime + (_num - 1) * singleWaitTime;
		curWaitTime = 0.f;

		moveComponent->Velocity = FVector::ZeroVector;
	}
}

void ALumiEnemyBullet::FireInDirection(const FVector& _direction)
{
	direction = _direction;
}

void ALumiEnemyBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		if (Cast<ALumiCharacter>(OtherActor) != nullptr)
		{
			// Hit Enemy
			ALumiCharacter* character = Cast<ALumiCharacter>(OtherActor);
			character->GetDamageByEnemy(SkillDamage + CharaATK);
		}
	}
	DestroyBullet();
}

void ALumiEnemyBullet::DestroyBullet()
{
	Destroy();
}
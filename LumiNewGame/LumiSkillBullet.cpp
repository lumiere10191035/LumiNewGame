// Fill out your copyright notice in the Description page of Project Settings.


#include "LumiSkillBullet.h"
#include "LumiEnemyUnit.h"
#include "LumiNewGameGameModeBase.h"

// Sets default values
ALumiSkillBullet::ALumiSkillBullet()
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
		SkillSphereComp->OnComponentHit.__Internal_AddDynamic(this, &ALumiSkillBullet::OnHit, "OnHit");
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
void ALumiSkillBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALumiSkillBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bulletCurLife += DeltaTime;
	if (bulletCurLife >= bulletLife)
	{
		DestroyBullet();
	}
}

void ALumiSkillBullet::InitSkillSphereInfo(int skill_id)
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));

	FSkillData data;
	if (!lumiGameMode->GetSkillDataById(data, skill_id))
		return;

	if (SkillSphereComp)
	{
		SkillSphereComp->InitSphereRadius(data.BallRadius);
	}
	if (SkillMesh)
	{
		SkillMesh->SetRelativeScale3D(FVector(0.1f));
	}
	if (moveComponent)
	{
		moveComponent->InitialSpeed = data.BallSpeed;
		moveComponent->MaxSpeed = data.BallSpeed;
		moveComponent->bRotationFollowsVelocity = true;
		moveComponent->bShouldBounce = false;
		moveComponent->ProjectileGravityScale = 0.0f;

		bulletLife = data.BallTime;
		SkillDamage = data.Damage;
		SkillType = data.SkillType;
	}
}

void ALumiSkillBullet::FireInDirection(const FVector& _direction)
{
	moveComponent->Velocity = _direction * moveComponent->InitialSpeed;
}

void ALumiSkillBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		if (Cast<ALumiEnemyUnit>(OtherActor) != nullptr)
		{
			// Hit Enemy
			int damage = 10;
			ALumiEnemyUnit* enemy = Cast<ALumiEnemyUnit>(OtherActor);
			enemy->GetDamageBySkill(SkillDamage);
		}	
	}
	DestroyBullet();
}

void ALumiSkillBullet::DestroyBullet()
{
	Destroy();
}


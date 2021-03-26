// Fill out your copyright notice in the Description page of Project Settings.


#include "LumiEnemyUnit.h"

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

	LifePoint = 100;
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

// Called to bind functionality to input
void ALumiEnemyUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALumiEnemyUnit::GetDamageBySkill(int _damage)
{
	LifePoint -= _damage;
	
	if (LifePoint <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Enemy Die!"));
		Destroy();
	}
}


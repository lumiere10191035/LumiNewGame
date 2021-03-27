// Fill out your copyright notice in the Description page of Project Settings.


#include "PointNPC.h"
#include "LumiCharacter.h"
#include "LumiNewGameGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APointNPC::APointNPC()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PointMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProxSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	ProxSphere->SetupAttachment(StaticMeshComp);
	ProxSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProxSphere->SetRelativeLocation(FVector::ZeroVector);
	//ProxSphere->SetSphereRadius = 100.f;
	ProxSphere->OnComponentBeginOverlap.__Internal_AddDynamic(this, &APointNPC::OnStartOverlap, TEXT("OnStartOverlap"));
}

APointNPC::APointNPC(FString meshPath)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PointMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);

	AttachStaticMesh(meshPath);
}

// Called when the game starts or when spawned
void APointNPC::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APointNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APointNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APointNPC::AttachStaticMesh(FString path)
{
	if (StaticMeshComp != nullptr)
	{
		UStaticMesh* curMesh = LoadObject<UStaticMesh>(NULL, *path);
		if (curMesh != nullptr)
		{
			StaticMeshComp->SetStaticMesh(curMesh);
		}
	}
}

void APointNPC::UpdateInitData(const FPointObjData& _data)
{
	AttachStaticMesh(_data.Path);

	Score = _data.Score;
	Exp = _data.Exp;
	if (ProxSphere != nullptr)
	{
		ProxSphere->SetWorldScale3D(FVector(_data.Size));
		ProxSphere->SetSphereRadius(_data.Radius);
	}

	if (StaticMeshComp != nullptr)
	{
		StaticMeshComp->SetWorldScale3D(FVector(_data.Size));
	}
}

void APointNPC::OnStartOverlap(UPrimitiveComponent* OverlapppedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (Cast<ALumiCharacter>(otherActor) == nullptr)
	{
		return;
	}

	// Add Exp
	ALumiNewGameGameModeBase* gameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	gameMode->charaExpDelegate.ExecuteIfBound(Exp);
	gameMode->scoreDelegate.ExecuteIfBound(Score);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("DESTROY POINT"));
	// Destroy self
	Destroy();
}

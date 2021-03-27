// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "ReadImportData.h"
#include "LumiCameraThird.h"
#include "LumiCharacter.generated.h"

#define MAX_CHARACTER_LEVEL 10

struct LumiCharacterStatus {
	int maxHP;
	int curHP;
	int maxMP;
	int curMP;
	int maxExp;
	int curExp;
};

struct LumiCharaFlash {
	int FlashCost;
	float FlashCoolDown;
	float FlashCurCoolDown;
	float FlashDistance;
	bool FlashInCoolDown;
};

enum BOOST_STATE {
	DEFAULT			= 0,
	BOOST_HIDE			= 1,
	BOOST_REDUCE			= 2,
	BOOST_RECOVER_WAIT	= 3,
	BOOST_RECOVER			= 4,
	BOOST_HIDE_WAIT		= 5,
};

UCLASS()
class LUMINEWGAME_API ALumiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALumiCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*UPROPERTY(VisibleAnywhere)
	UCameraComponent* LumiCamera;

	UPROPERTY(EditAnywhere);
	class USpringArmComponent* springArmCom;*/

	//Camera
	ALumiCameraThird* LumiCamera;

	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void MoveRight(float Value);
	UFUNCTION()
	void SpeedUp();
	UFUNCTION()
	void NoSpeedUp();
	UFUNCTION()
	void LookUp(float Value);
	UFUNCTION()
	void LookHorizon(float Value);
	UFUNCTION()
	void LumiFlash();
	UFUNCTION()
	void ExitGameOption();

	UFUNCTION()
	void LifePlus();
	UFUNCTION()
	void LifeMinus();
	UFUNCTION()
	void LifeChange(int Value);
	UFUNCTION()
	void MagicPlus();
	UFUNCTION()
	void MagicMinus();
	UFUNCTION()
	void MagicChange(int Value);
	UFUNCTION()
	void AddCharacterExpTest();
	UFUNCTION()
	void ResetCharacterExpTest();
	UFUNCTION()
	void AddCharacterExp(int Value);


	UFUNCTION()
	void UpdateCharacterStatus();
	UFUNCTION()
	void UpdateStatusShow();
	UFUNCTION()
	void UpdateBoostShow();
	UFUNCTION()
	void CharaExpPlus(int Exp);

	UFUNCTION()
	void TestSkillShoot();

	UPROPERTY(EditAnywhere)
	float MoveSpeed;

	FLumiImportData LevelData;
	int CharacterLevel;
	LumiCharacterStatus lumiStatus;
	LumiCharaFlash skillFlash;
	TMap<int, float> SkillCoolTimeList;
	TMap<int, float> MaxSkillCoolTime;
	TMap<int, int> SkillInCoolTime;

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class ALumiSkillBullet> LumiBulletClass;

protected:
	bool CheckSkillEnabled(int skillId);

	void LuanchBallSkill(int skillId, int skillType, FVector startLocation, FRotator startRotation);

	class UCharacterMovementComponent* characterMove;
	UCapsuleComponent* capsuleCom;
	
	FVector2D MoveDirect;
	bool bSpeedUp;

	float BoostMax;
	float BoostCur;
	float BoostPer;

	float CurWaitTime;
	float WaitTime;
	float RecoverSpeed;

	float DisappearTime;
	float CurHideTime;

	BOOST_STATE boostState;
};

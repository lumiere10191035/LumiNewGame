// Fill out your copyright notice in the Description page of Project Settings.


#include "LumiCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LumiNewGameGameModeBase.h"
#include "ReadImportData.h"
#include "LumiSkillBullet.h"

// Sets default values
ALumiCharacter::ALumiCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//LumiCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("LumiCamera"));
	//springArmCom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	bUseControllerRotationYaw = false;

	// CharacterMovement
	characterMove = GetCharacterMovement();
	characterMove->MaxAcceleration = 16384.f;
	//characterMove->bOrientRotationToMovement = true;
	
	/*{
		springArmCom->SetupAttachment(RootComponent);
		springArmCom->SetRelativeLocation(FVector(0.0f, 0.f, 50.f));
		springArmCom->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
		springArmCom->TargetArmLength = 400.f;
		springArmCom->CameraLagSpeed = 3.f;

		LumiCamera->SetupAttachment(springArmCom, USpringArmComponent::SocketName);
	}*/

	//init character
	CharacterLevel = 1;
	/*lumiStatus.curHP = 100;
	lumiStatus.maxHP = 300;
	lumiStatus.curMP = 300;
	lumiStatus.maxMP = 400;*/
	UpdateCharacterStatus();
	lumiStatus.curHP = lumiStatus.maxHP;
	lumiStatus.curMP = lumiStatus.maxMP;
	lumiStatus.curExp = 0;

	MPRecoverCount = 0.f;
	HPRecoverCount = 0.f;
}

// Called when the game starts or when spawned
void ALumiCharacter::BeginPlay()
{
	Super::BeginPlay();

	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->updateRequest.BindUFunction(this, "UpdateStatusShow");
	lumiGameMode->charaExpDelegate.BindUFunction(this, "CharaExpPlus");

	//initial UMG;
	UpdateStatusShow();
	//Initial Param;
	FLumiCharaParam charaParam;
	if (UReadImportData::ImportCharaParam(charaParam))
	{
		BoostMax = charaParam.BoostMax;
		BoostPer = charaParam.BoostPer;
		BoostCur = BoostMax;

		WaitTime = charaParam.WaitTime;
		CurWaitTime = 0.f;
		RecoverSpeed = charaParam.RecoverSpeed;

		DisappearTime = charaParam.DisappearTime;
		CurHideTime = 0.f;
		StaticCoolTime = charaParam.StaticCoolTime;
		CurStaticCoolTime = -1;

		boostBarState = BOOST_HIDE;
	}

	TArray<FSkillData> allSkillData;
	if (UReadImportData::ImportAllSkillData(allSkillData))
	{
		lumiSkillList.Empty();
		for (auto data : allSkillData)
		{
			LumiSkillStruct newSkill;
			newSkill.skillData = data;
			newSkill.bUnlock = data.UnlockLevel <= CharacterLevel;
			newSkill.skillId = data.Skill_Id;
			newSkill.skillDamage = data.Damage;
			newSkill.curCoolTime = -1;
			lumiSkillList.Emplace(newSkill);
		}
		lumiSkillList.Sort([](const LumiSkillStruct& a, const LumiSkillStruct& b) {
			return a.skillId < b.skillId;
		});
	}

	FLumiDash dashParam;
	if (UReadImportData::ImportCharaDash(dashParam))
	{
		boostStatus.BoostCost = dashParam.BoostCost;
		boostStatus.BoostTime = dashParam.MoveTime;
		boostStatus.SpeedRatio = dashParam.SpeedUp;

		boostStatus.curBoostTime = -1.f;
	}
}

// Called every frame
void ALumiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (lumiGameMode->GetBGameExitShow())
		return;

	if (boostStatus.curBoostTime > -1.f)
	{
		boostStatus.curBoostTime += DeltaTime;
		if (boostStatus.curBoostTime > boostStatus.BoostTime)
		{
			boostStatus.curBoostTime = -1.f;
			bDash = false;
		}
	}

	bool checkSpeedUp = bSpeedUp;
	if (checkSpeedUp && !MoveDirect.IsZero())
	{
		if (BoostCur == 0.f)
		{
			boostBarState = BOOST_REDUCE;
			checkSpeedUp = false;
		}
		else
		{
			BoostCur = FMath::Clamp(BoostCur - BoostPer * DeltaTime, 0.f, BoostMax);
			boostBarState = BOOST_REDUCE;

			UpdateBoostShow();
		}
	}
	else
	{
		switch (boostBarState)
		{
		case BOOST_HIDE:
			break;
		case BOOST_REDUCE:
			if (BoostCur < BoostMax)
			{
				boostBarState = BOOST_RECOVER_WAIT;
				CurWaitTime = 0.f;
			}
			else
			{
				boostBarState = BOOST_HIDE_WAIT;
				CurHideTime = 0.f;
			}
			break;
		case BOOST_HIDE_WAIT:
			CurHideTime += DeltaTime;
			if (CurHideTime >= DisappearTime)
			{
				boostBarState = BOOST_HIDE;
				UpdateBoostShow();
				CurHideTime = 0.f;
			}
			break;
		case BOOST_RECOVER:
			BoostCur += RecoverSpeed * DeltaTime;
			if (BoostCur >= BoostMax)
			{
				BoostCur = BoostMax;
				boostBarState = BOOST_HIDE_WAIT;
				CurHideTime = 0.f;
			}
			UpdateBoostShow();
			break;
		case BOOST_RECOVER_WAIT:
			CurWaitTime += DeltaTime;
			if (CurWaitTime >= WaitTime)
			{
				CurWaitTime = 0.f;
				boostBarState = BOOST_RECOVER;
			}
			break;
		default:
			break;
		}
	}
	if (bDash)
	{
		boostBarState = BOOST_RECOVER_WAIT;
		CurWaitTime = 0.f;
	}

	{
		if (!MoveDirect.IsZero())
		{
			float currentRatio = bDash ? boostStatus.SpeedRatio :
				(checkSpeedUp ? 2.f : 1.f);
			float currentSpeed = MoveSpeed * currentRatio;
			//calculate true direction by camera;
			FRotator cameraRotator = lumiGameMode->LumiCameraActor->GetSpringArmRotation();

			FVector NewDirection = FRotationMatrix(cameraRotator).TransformVector(FVector(MoveDirect, 0.f));
			NewDirection.Z = 0.f;

			characterMove->MaxWalkSpeed = currentSpeed;
			AddMovementInput(NewDirection, 1.f);

			// move camera;
			lumiGameMode->LumiCameraActor->SetActorLocation(GetActorLocation());

			// Model Direction
			SetActorRelativeRotation(NewDirection.Rotation());
		}
	}

	{
		//Static cool down
		if (CurStaticCoolTime > -1)
		{
			// in cool time
			CurStaticCoolTime += DeltaTime;
			if (CurStaticCoolTime >= StaticCoolTime)
			{
				CurStaticCoolTime = -1.f;
				lumiGameMode->updateSkillListDelegate.ExecuteIfBound();
			}
		}

		// skill cool down
		for (auto& a : lumiSkillList)
		{
			if (a.curCoolTime > -1.f)
			{
				a.curCoolTime += DeltaTime;
				FSkillData data = a.skillData;
				if (a.curCoolTime >= data.CoolTime)
				{
					a.curCoolTime = -1;
					lumiGameMode->updateSingleIconDelegate.ExecuteIfBound(data.Skill_Id);
				}
			}
		}
	}

	{
		//MP recover
		if (lumiStatus.curMP == lumiStatus.maxMP)
		{
			if (MPRecoverCount > 0.f)
			{
				MPRecoverCount = 0.f;
			}
			// donothing;
		}
		else
		{
			MPRecoverCount += DeltaTime;
			if (MPRecoverCount >= RECOVER_TIME)
			{
				MagicChange(lumiStatus.MPRecover);
				MPRecoverCount = 0.f;
			}
		}
	}
}

// Called to bind functionality to input
void ALumiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALumiCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALumiCharacter::MoveRight);
	PlayerInputComponent->BindAction("SpeedUp", IE_Pressed, this, &ALumiCharacter::SpeedUp);
	PlayerInputComponent->BindAction("SpeedUp", IE_Released, this, &ALumiCharacter::NoSpeedUp);
	//PlayerInputComponent->BindAction("LifeUp", IE_Pressed, this, &ALumiCharacter::LifePlus);
	//PlayerInputComponent->BindAction("LifeDown", IE_Pressed, this, &ALumiCharacter::LifeMinus);
	//PlayerInputComponent->BindAction("MagicUp", IE_Pressed, this, &ALumiCharacter::MagicPlus);
	//PlayerInputComponent->BindAction("MagicDown", IE_Pressed, this, &ALumiCharacter::MagicMinus);
	//PlayerInputComponent->BindAction("GetExp", IE_Pressed, this, &ALumiCharacter::AddCharacterExpTest);
	//PlayerInputComponent->BindAction("ResetExp", IE_Pressed, this, &ALumiCharacter::ResetCharacterExpTest);
	PlayerInputComponent->BindAxis("LookUp", this, &ALumiCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookHorizon", this, &ALumiCharacter::LookHorizon);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALumiCharacter::Jump);
	PlayerInputComponent->BindAction("Flash", IE_Pressed, this, &ALumiCharacter::LumiFlash);
	PlayerInputComponent->BindAction("ExitGame", IE_Pressed, this, &ALumiCharacter::ExitGameOption);

	// Skill
	PlayerInputComponent->BindAction("NormalAttack", IE_Pressed, this, &ALumiCharacter::SkillNormal);
	PlayerInputComponent->BindAction("Skill1", IE_Pressed, this, &ALumiCharacter::SkillFirst);
	PlayerInputComponent->BindAction("Skill2", IE_Pressed, this, &ALumiCharacter::SkillSecond);
	PlayerInputComponent->BindAction("Skill3", IE_Pressed, this, &ALumiCharacter::SkillThird);

	//PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ALumiCharacter::TestSkillShoot);
}

void ALumiCharacter::ExitGameOption()
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->ShowExitGameUMG();
}

void ALumiCharacter::MoveForward(float Value)
{
	MoveDirect.X = FMath::Clamp<float>(Value, -1.0f, 1.0f);
}

void ALumiCharacter::MoveRight(float Value)
{
	MoveDirect.Y = FMath::Clamp<float>(Value, -1.0f, 1.0f);
}

void ALumiCharacter::LookUp(float Value)
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!lumiGameMode->GetBGameExitShow())
	{
		lumiGameMode->LumiCameraActor->LookUp(Value);
	}
	
	/*FRotator NewRotator = springArmCom->GetRelativeRotation();
	NewRotator.Pitch = FMath::Clamp(NewRotator.Pitch - Value, -90.f, 0.f);
	springArmCom->SetRelativeRotation(NewRotator);*/
}

void ALumiCharacter::LookHorizon(float Value)
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!lumiGameMode->GetBGameExitShow())
	{
		lumiGameMode->LumiCameraActor->LookHorizon(Value);
	}
	
	/*FRotator NewRotator = springArmCom->GetRelativeRotation();
	NewRotator.Yaw += Value;
	springArmCom->SetRelativeRotation(NewRotator);*/
}

void ALumiCharacter::LumiFlash()
{
	if (bDash)
		return;

	//boost
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (lumiGameMode->GetBGameExitShow())
		return;

	if (BoostCur < boostStatus.BoostCost)
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Can't use Flash, because BoostBar isn't enough!!"));
		return;
	}
	
	bDash = true;
	boostStatus.curBoostTime = 0.f;
	bSpeedUp = false;
	BoostCur -= boostStatus.BoostCost;
	UpdateBoostShow();
}

void ALumiCharacter::SpeedUp()
{
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Get SpeedUp");

	if (bDash)
	{
		// donothing
	}
	else
	{
		bSpeedUp = true;
	}
}

void ALumiCharacter::NoSpeedUp()
{
	bSpeedUp = false;
}

void ALumiCharacter::LifePlus()
{
	lumiStatus.curHP = FMath::Clamp(lumiStatus.curHP + 6, 0, lumiStatus.maxHP);
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->lifeDelegate.Execute(lumiStatus.curHP);
}

void ALumiCharacter::LifeMinus()
{
	lumiStatus.curHP = FMath::Clamp(lumiStatus.curHP - 10, 0, lumiStatus.maxHP);
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->lifeDelegate.Execute(lumiStatus.curHP);
}

void ALumiCharacter::LifeChange(int Value)
{
	lumiStatus.curHP = FMath::Clamp(lumiStatus.curHP + Value, 0, lumiStatus.maxHP);
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->lifeDelegate.Execute(lumiStatus.curHP);
}

void ALumiCharacter::MagicPlus()
{
	lumiStatus.curMP = FMath::Clamp(lumiStatus.curMP + 7, 0, lumiStatus.maxMP);
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->MagicDelegate.Execute(lumiStatus.curMP);
}

void ALumiCharacter::MagicMinus()
{
	lumiStatus.curMP = FMath::Clamp(lumiStatus.curMP - 10, 0, lumiStatus.maxMP);
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->MagicDelegate.Execute(lumiStatus.curMP);
}

void ALumiCharacter::MagicChange(int Value)
{
	lumiStatus.curMP = FMath::Clamp(lumiStatus.curMP + Value, 0, lumiStatus.maxMP);
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->MagicDelegate.Execute(lumiStatus.curMP);
}

void ALumiCharacter::AddCharacterExp(int Value)
{
	if (CharacterLevel == MAX_CHARACTER_LEVEL)
	{
		return;
	}

	int newExp = lumiStatus.curExp + Value;

	if (newExp >= lumiStatus.maxExp)
	{
		while (newExp >= lumiStatus.maxExp)
		{
			CharacterLevel += 1;
			newExp -= lumiStatus.maxExp;
			lumiStatus.curExp = newExp;

			DoLevelUp();

			lumiStatus.curExp = CharacterLevel == MAX_CHARACTER_LEVEL ? 0 :
				newExp - lumiStatus.maxExp;
			if (CharacterLevel == MAX_CHARACTER_LEVEL)
				break;
		}

		UpdateStatusShow();
	}
	else
	{
		lumiStatus.curExp = newExp;
		ALumiNewGameGameModeBase* lumiGameMode;
		lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
		lumiGameMode->UIExpDelegate.ExecuteIfBound(lumiStatus.curExp);
	}
}

void ALumiCharacter::AddCharacterExpTest()
{
	AddCharacterExp(5);
}

void ALumiCharacter::ResetCharacterExpTest()
{
	CharacterLevel = 1;
	FLumiLevelData data;
	if (UReadImportData::ImportLevelData(data, CharacterLevel))
	{
		lumiStatus.maxHP = data.MaxHP;
		lumiStatus.maxMP = data.MaxMP;
		lumiStatus.maxExp = data.NeedExp;

		lumiStatus.HPRecover = data.HPRecover;
		lumiStatus.MPRecover = data.MPRecover;
	}
	lumiStatus.curHP = FMath::Clamp(lumiStatus.curHP, 0, lumiStatus.maxHP);
	lumiStatus.curMP = FMath::Clamp(lumiStatus.curMP, 0, lumiStatus.maxMP);
	lumiStatus.curExp = 0;

	UpdateStatusShow();
}

void ALumiCharacter::UpdateCharacterStatus()
{
	FLumiLevelData data;
	if (UReadImportData::ImportLevelData(data, CharacterLevel))
	{
		lumiStatus.maxHP = data.MaxHP;
		lumiStatus.maxMP = data.MaxMP;
		lumiStatus.maxExp = data.NeedExp;
		lumiStatus.ATK = data.ATK;
		lumiStatus.DEF = data.DEF;
		lumiStatus.HPRecover = data.HPRecover;
		lumiStatus.MPRecover = data.MPRecover;
	}
}

void ALumiCharacter::UpdateStatusShow()
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->updateStateDelegate.ExecuteIfBound(lumiStatus.maxHP,
												lumiStatus.curHP,
												lumiStatus.maxMP,
												lumiStatus.curMP,
												lumiStatus.maxExp,
												lumiStatus.curExp,
												CharacterLevel);
}

void ALumiCharacter::CharaExpPlus(int Exp)
{
	AddCharacterExp(Exp);
}

void ALumiCharacter::UpdateBoostShow()
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->updateBoostDelegate.ExecuteIfBound(BoostMax == 0.f ? 1.f : BoostCur / BoostMax, boostBarState != BOOST_HIDE);
}

void ALumiCharacter::SkillNormal()
{
	int skillId = 1;
	SkillShoot(skillId);
}

void ALumiCharacter::SkillFirst()
{
	int skillId = 2;
	SkillShoot(skillId);
}

void ALumiCharacter::SkillSecond()
{
	int skillId = 3;
	SkillShoot(skillId);
}

void ALumiCharacter::SkillThird()
{
	int skillId = 4;
	SkillShoot(skillId);
}

/////////////
void ALumiCharacter::SkillShoot(int _skillId)
{
	if (!CheckSkillEnabled(_skillId))
	{
		return;
	}

	CurStaticCoolTime = 0.f;

	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));

	
	if (!(lumiSkillList.Num() >= _skillId))
	{
		return;
	}
	LumiSkillStruct& curSkill = lumiSkillList[_skillId - 1];
	FSkillData data = curSkill.skillData;

	curSkill.curCoolTime = 0.f;

	lumiGameMode->updateSkillListDelegate.ExecuteIfBound();

	//SkillInCoolTime.Emplace(_skillId, 1);
	//SkillCoolTimeList.Emplace(_skillId, 0.f);
	//if (!MaxSkillCoolTime.Contains(_skillId))
	//{
	//	MaxSkillCoolTime.Emplace(_skillId, data.CoolTime);
	//}

	// MP cost
	MagicChange(-data.MPCost);

	FVector StartVector = GetActorLocation() + FVector(0.f, 0.f, 180.f);
	FRotator BulletRotation = lumiGameMode->LumiCameraActor->GetSpringArmRotation();

	FVector MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(50.f, 0.f, 0.f));
	FRotator MuzzleRotation = BulletRotation;
	MuzzleRotation.Pitch += 10.f;

	TSubclassOf<class ALumiSkillBullet> LumiBulletClass = LumiBulletClassList[_skillId - 1];
	if (LumiBulletClass)
	{
		switch (data.BallNum)
		{
		case 1:
			LaunchBallSkill(data, MuzzleLocation, MuzzleRotation, 1);
			break;
		case 3:
			LaunchBallSkill(data, MuzzleLocation, MuzzleRotation, 2);
			MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(50.f, -20.f, -20.f));
			LaunchBallSkill(data, MuzzleLocation, MuzzleRotation, 1);
			MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(50.f, 20.f, -20.f));
			LaunchBallSkill(data, MuzzleLocation, MuzzleRotation, 3);
			break;
		case 5:
			LaunchBallSkill(data, MuzzleLocation, MuzzleRotation, 3);
			MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(50.f, -20.f, -20.f));
			LaunchBallSkill(data, MuzzleLocation, MuzzleRotation, 2);
			MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(50.f, 20.f, -20.f));
			LaunchBallSkill(data, MuzzleLocation, MuzzleRotation, 4);
			MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(50.f, -30.f, -20.f));
			LaunchBallSkill(data, MuzzleLocation, MuzzleRotation, 1);
			MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(50.f, 30.f, -20.f));
			LaunchBallSkill(data, MuzzleLocation, MuzzleRotation, 5);
			break;
		default:
			break;
		}
	}
}

void ALumiCharacter::LaunchBallSkill(const FSkillData& _data, FVector startLocation, FRotator startRotation, int num)
{
	TSubclassOf<class ALumiSkillBullet> LumiBulletClass = LumiBulletClassList[_data.Skill_Id - 1];
	if (LumiBulletClass)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();
		ALumiSkillBullet* bullet = GetWorld()->SpawnActor<ALumiSkillBullet>(LumiBulletClass, startLocation, startRotation, spawnParams);
		if (bullet)
		{
			bullet->InitSkillSphereInfo(_data, lumiStatus.ATK, num);
			bullet->FireInDirection(startRotation.Vector());
		}
	}
}

bool ALumiCharacter::CheckSkillEnabled(int skillId)
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (!(lumiSkillList.Num() >= skillId))
	{
		return false;
	}
	LumiSkillStruct& skill = lumiSkillList[skillId - 1];
	const FSkillData& data = skill.skillData;

	if (!skill.bUnlock)
	{

		return false;
	}

	// check mp
	if (lumiStatus.curMP < data.MPCost)
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("SkillID = %d, Not Enough MP"), skillId));

		return false;
	}

	// cool down
	if (CurStaticCoolTime > -1.f)
	{
		return false;
	}

	if (skill.curCoolTime > -1.f)
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("SkillID = %d, Is In Cool Time"), skillId));

		return false;
	}
	
	return true;
}

void ALumiCharacter::DoLevelUp()
{
	FLumiLevelData data;
	if (UReadImportData::ImportLevelData(data, CharacterLevel))
	{
		int deltaHP = data.MaxHP - lumiStatus.maxHP;
		int deltaMP = data.MaxMP - lumiStatus.maxMP;
		lumiStatus.maxHP = data.MaxHP;
		lumiStatus.maxMP = data.MaxMP;
		lumiStatus.maxExp = data.NeedExp;
		lumiStatus.HPRecover = data.HPRecover;
		lumiStatus.MPRecover = data.MPRecover;

		lumiStatus.curHP += deltaHP;
		lumiStatus.curMP += deltaMP;
		lumiStatus.ATK = data.ATK;
		lumiStatus.DEF = data.DEF;
	}
	
	// Check skill unlock
	for (auto& skill : lumiSkillList)
	{
		skill.bUnlock = skill.skillData.UnlockLevel <= CharacterLevel;
	}

	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->updateSkillListDelegate.ExecuteIfBound();
}
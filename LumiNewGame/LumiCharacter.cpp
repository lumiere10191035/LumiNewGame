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

		boostState = BOOST_HIDE;
	}

	FLumiSkill skillParam;
	if (UReadImportData::ImportCharaSkill(skillParam))
	{
		skillFlash.FlashCoolDown = skillParam.ReloadTime;
		skillFlash.FlashCost = skillParam.MPCost;
		skillFlash.FlashCurCoolDown = 0.f;
		skillFlash.FlashDistance = skillParam.MoveDistance;
		skillFlash.FlashInCoolDown = false;
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

	// Flash
	if (skillFlash.FlashInCoolDown)
	{
		skillFlash.FlashCurCoolDown -= DeltaTime;
		if (skillFlash.FlashCurCoolDown <= 0.f)
		{
			skillFlash.FlashInCoolDown = false;
		}
	}

	bool checkSpeedUp = bSpeedUp;
	if (checkSpeedUp && !MoveDirect.IsZero())
	{
		if (BoostCur == 0.f)
		{
			boostState = BOOST_REDUCE;
			checkSpeedUp = false;
		}
		else
		{
			BoostCur = FMath::Clamp(BoostCur - BoostPer * DeltaTime, 0.f, BoostMax);
			boostState = BOOST_REDUCE;

			UpdateBoostShow();
		}
	}
	else
	{
		switch (boostState)
		{
		case BOOST_HIDE:
			break;
		case BOOST_REDUCE:
			if (BoostCur < BoostMax)
			{
				boostState = BOOST_RECOVER_WAIT;
				CurWaitTime = 0.f;
			}
			else
			{
				boostState = BOOST_HIDE_WAIT;
				CurHideTime = 0.f;
			}
			break;
		case BOOST_HIDE_WAIT:
			CurHideTime += DeltaTime;
			if (CurHideTime >= DisappearTime)
			{
				boostState = BOOST_HIDE;
				UpdateBoostShow();
				CurHideTime = 0.f;
			}
			break;
		case BOOST_RECOVER:
			BoostCur += RecoverSpeed * DeltaTime;
			if (BoostCur >= BoostMax)
			{
				BoostCur = BoostMax;
				boostState = BOOST_HIDE_WAIT;
				CurHideTime = 0.f;
			}
			UpdateBoostShow();
			break;
		case BOOST_RECOVER_WAIT:
			CurWaitTime += DeltaTime;
			if (CurWaitTime >= WaitTime)
			{
				CurWaitTime = 0.f;
				boostState = BOOST_RECOVER;
			}
			break;
		default:
			break;
		}
	}

	{
		if (!MoveDirect.IsZero())
		{
			float currentSpeed = checkSpeedUp ? MoveSpeed * 2.f : MoveSpeed;
			
			//calculate true direction by camera;
			FRotator cameraRotator = lumiGameMode->LumiCameraActor->GetSpringArmRotation();

			FVector NewDirection = FRotationMatrix(cameraRotator).TransformVector(FVector(MoveDirect, 0.f));
			NewDirection.Z = 0.f;

			characterMove->MaxWalkSpeed = currentSpeed;
			AddMovementInput(NewDirection, 1.f);
			//FVector DirectionX = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
			//AddMovementInput(DirectionX, MoveDirect.X);
			//FVector DirectionY = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
			//AddMovementInput(DirectionY, MoveDirect.Y);

			// move camera;
			lumiGameMode->LumiCameraActor->SetActorLocation(GetActorLocation());

			// Model Direction
			SetActorRelativeRotation(NewDirection.Rotation());
		}
	}

	{
		// skill cool down
		for (auto& a : SkillCoolTimeList)
		{
			if (SkillInCoolTime.Contains(a.Key))
			{
				if (SkillInCoolTime[a.Key])
				{
					if (a.Value > -1)
					{
						a.Value += DeltaTime;
						if (MaxSkillCoolTime.Contains(a.Key))
						{
							if (a.Value >= MaxSkillCoolTime[a.Key])
							{
								a.Value = -1;
								SkillInCoolTime.Emplace(a.Key, 0);
							}
						}
					}
				}
				
			}
			else {
				SkillInCoolTime.Emplace(a.Key, 1);
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

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ALumiCharacter::TestSkillShoot);
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
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (lumiGameMode->GetBGameExitShow())
		return;

	if (skillFlash.FlashInCoolDown)
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Can't use Flash, because it is now in cooldown!"));
		return;
	}

	if (lumiStatus.curMP < skillFlash.FlashCost)
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Can't use Flash, because MP isn't enough!!"));
		return;
	}
	
	skillFlash.FlashInCoolDown = true;
	skillFlash.FlashCurCoolDown = skillFlash.FlashCoolDown;
	MagicChange(-skillFlash.FlashCost);

	// Move Character;
	FVector FlashDirection = GetActorForwardVector();
	FlashDirection.Z = 0.f;
	FVector NewLocation = GetActorLocation() + FlashDirection.GetSafeNormal() * skillFlash.FlashDistance;
	SetActorLocation(NewLocation);

	lumiGameMode->LumiCameraActor->SetActorLocation(NewLocation);
}

void ALumiCharacter::SpeedUp()
{
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Get SpeedUp");

	bSpeedUp = true;
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
		CharacterLevel += 1;

		FLumiLevelData data;
		if (UReadImportData::ImportLevelData(data, CharacterLevel))
		{
			lumiStatus.curExp = CharacterLevel == MAX_CHARACTER_LEVEL ? 0 :
				newExp - lumiStatus.maxExp;

			int deltaHP = data.MaxHP - lumiStatus.maxHP;
			int deltaMP = data.MaxMP - lumiStatus.maxMP;
			lumiStatus.maxHP = data.MaxHP;
			lumiStatus.maxMP = data.MaxMP;
			lumiStatus.maxExp = data.NeedExp;

			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("LevelUp HPChagne:from %d to %d"), lumiStatus.curHP, deltaHP));

			lumiStatus.curHP += deltaHP;
			lumiStatus.curMP += deltaMP;

			UpdateStatusShow();
		}
	}
	else
	{
		lumiStatus.curExp = newExp;
		ALumiNewGameGameModeBase* lumiGameMode;
		lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
		lumiGameMode->ExpDelegate.ExecuteIfBound(lumiStatus.curExp);
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
	lumiGameMode->updateBoostDelegate.ExecuteIfBound(BoostMax == 0.f ? 1.f : BoostCur / BoostMax, boostState != BOOST_HIDE);
}

/////////////
void ALumiCharacter::TestSkillShoot()
{
	int skillId = 1;

	if (!CheckSkillEnabled(skillId))
	{
		return;
	}

	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	FSkillData data;
	if (!lumiGameMode->GetSkillDataById(data, skillId))
	{
		return;
	}

	SkillInCoolTime.Emplace(skillId, 1);
	SkillCoolTimeList.Emplace(skillId, 0.f);
	if (!MaxSkillCoolTime.Contains(skillId))
	{
		MaxSkillCoolTime.Emplace(skillId, data.CoolTime);
	}

	FVector StartVector = GetActorLocation() + FVector(0.f, 0.f, 180.f);
	FRotator BulletRotation = lumiGameMode->LumiCameraActor->GetSpringArmRotation();

	FVector MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(100.f, 0.f, 0.f));
	FRotator MuzzleRotation = BulletRotation;
	MuzzleRotation.Pitch += 10.f;

	if (LumiBulletClass)
	{
		switch (data.BallNum)
		{
		case 1:
			LuanchBallSkill(skillId, data.SkillType, MuzzleLocation, MuzzleRotation);
			break;
		case 3:
			LuanchBallSkill(skillId, data.SkillType, MuzzleLocation, MuzzleRotation);
			MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(100.f, -50.f, -50.f));
			LuanchBallSkill(skillId, data.SkillType, MuzzleLocation, MuzzleRotation);
			MuzzleLocation = StartVector + FTransform(BulletRotation).TransformVector(FVector(100.f, 50.f, -50.f));
			LuanchBallSkill(skillId, data.SkillType, MuzzleLocation, MuzzleRotation);
			break;
		case 5:
			LuanchBallSkill(skillId, data.SkillType, MuzzleLocation, MuzzleRotation);
			LuanchBallSkill(skillId, data.SkillType, MuzzleLocation, MuzzleRotation);
			LuanchBallSkill(skillId, data.SkillType, MuzzleLocation, MuzzleRotation);
			LuanchBallSkill(skillId, data.SkillType, MuzzleLocation, MuzzleRotation);
			LuanchBallSkill(skillId, data.SkillType, MuzzleLocation, MuzzleRotation);
			break;
		default:
			break;
		}
	}
}

void ALumiCharacter::LuanchBallSkill(int skillId, int skillType, FVector startLocation, FRotator startRotation)
{
	if (LumiBulletClass)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();
		ALumiSkillBullet* bullet = GetWorld()->SpawnActor<ALumiSkillBullet>(LumiBulletClass, startLocation, startRotation, spawnParams);
		if (bullet)
		{
			bullet->InitSkillSphereInfo(skillId);
			bullet->FireInDirection(startRotation.Vector());
		}
	}
}

bool ALumiCharacter::CheckSkillEnabled(int skillId)
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));

	FSkillData data;
	if (!lumiGameMode->GetSkillDataById(data, skillId))
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("SkillID = %d, No skill"), skillId));

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
	if (SkillInCoolTime.Contains(skillId) && (SkillInCoolTime[skillId] == 1))
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("SkillID = %d, Is In Cool Time"), skillId));

		return false;
	}
	
	return true;
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "ReadImportData.generated.h"

USTRUCT(BlueprintType)
struct FLumiImportData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FLumiImportData() : DDD(0), EEE(0), FFF(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int DDD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int EEE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int FFF;
};

USTRUCT(BlueprintType)
struct FLumiLevelData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FLumiLevelData() : MaxHP(0), MaxMP(0), NeedExp(0), MPRecover(0), HPRecover(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int MaxMP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int NeedExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int MPRecover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int HPRecover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int ATK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int DEF;
};

USTRUCT(BlueprintType)
struct FPointObjData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FPointObjData() : Radius(0.f), Size(0), Score(0), Exp(0), Path("") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float Size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int Score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int Exp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	FString Path;
};

USTRUCT(BlueprintType)
struct FLumiCharaParam : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FLumiCharaParam() : BoostMax(0.f), BoostPer(0.f), RecoverSpeed(0.f), WaitTime(0.f), DisappearTime(0.f), StaticCoolTime(0.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float BoostMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float BoostPer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float RecoverSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float WaitTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float DisappearTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float StaticCoolTime;
};

USTRUCT(BlueprintType)
struct FLumiSkill : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FLumiSkill() : ReloadTime(0.f), MPCost(0), MoveDistance(0.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float ReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int MPCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float MoveDistance;
};

USTRUCT(BlueprintType)
struct FLumiGameParam : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FLumiGameParam() : TargetScore(0), GameMaxTime(0.f), PointNum(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int TargetScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float GameMaxTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int PointNum;
};

USTRUCT(BlueprintType)
struct FPointPosSet : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FPointPosSet() : PosX1(0.f), PosX2(0.f),
		PosY1(0.f),  PosY2(0.f), 
		PosZ(0.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float PosX1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float PosX2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float PosY1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float PosY2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float PosZ;
};

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FSkillData() : Skill_Id(0), 
		MPCost(0),
		Damage(0),
		SkillType(0),
		UnlockLevel(0),
		BallRadius(0.f),
		BallSpeed(0.f),
		StartWait(0.f),
		SingleWait(0.f),
		Blueprint("") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int Skill_Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int MPCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int BallNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int SkillType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int UnlockLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float BallRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float BallTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float BallSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float StartWait;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float SingleWait;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		FString Blueprint;
};

USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FEnemyData() : PosX_1(0.f), PosX_2(0.f),
				PosY_1(0.f), PosY_2(0.f),
				PosZ(0.f),
				Level(0), HP(0), MP(0),
				Skill_Id(0), EnemyType(0),
				Speed(0.f), MoveTime(0.f), UnitExp(0),
				AlarmDistance(0.f), Blueprint(TEXT("")){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float PosX_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float PosX_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float PosY_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float PosY_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float PosZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int Skill_Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int EnemyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float MoveTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float AlarmDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int UnitExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		FString Blueprint;
};

/////////////////////////////////////////////////
/**
 * 
 */
UCLASS()
class LUMINEWGAME_API UReadImportData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Character)
	static void TestLoadTable();

	UFUNCTION(BlueprintCallable, Category = Character)
	static void InitImportData();

	UFUNCTION(BlueprintCallable, Category = Character)
	static bool ImportLevelData(FLumiLevelData& Data, int Level);

	UFUNCTION(BlueprintCallable, Category = Character)
	static bool ImportPointObj(TArray<FPointObjData>& DataList);

	UFUNCTION(BlueprintCallable, Category = Character)
	static bool ImportCharaParam(FLumiCharaParam& CharaParam);

	UFUNCTION(BlueprintCallable, Category = Character)
	static bool ImportCharaSkill(FLumiSkill& CharaSkill);

	UFUNCTION(BlueprintCallable, Category = Character)
	static bool ImportGameParam(FLumiGameParam& GameParam);

	UFUNCTION(BlueprintCallable, Category = Character)
	static bool ImportAllSkillData(TArray<FSkillData>& SkillList);

	UFUNCTION(BlueprintCallable, Category = Enemy)
	static bool ImportAllEnemyData(TArray<FEnemyData>& EnemyList);

	UFUNCTION(BlueprintCallable, Category = Enemy)
	static bool ImportAllPointPosData(TArray<FPointPosSet>& objPosSet);
};

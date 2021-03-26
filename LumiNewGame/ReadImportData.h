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
	FLumiLevelData() : MaxHP(0), MaxMP(0), NeedExp(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int MaxMP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	int NeedExp;
};

USTRUCT(BlueprintType)
struct FPointObjData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FPointObjData() : PosX(0.f), PosY(0), PosZ(0), Path("") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float PosX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float PosY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float PosZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float Size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float Score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	float Exp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
	FString Path;
};

USTRUCT(BlueprintType)
struct FLumiCharaParam : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FLumiCharaParam() : BoostMax(0.f), BoostPer(0.f), RecoverSpeed(0.f), WaitTime(0.f), DisappearTime(0.f) {}

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
	FLumiGameParam() : TargetScore(0), GameMaxTime(0.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int TargetScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float GameMaxTime;
};

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FSkillData() : TargetScore(0), GameMaxTime(0.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		int TargetScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LumiData)
		float GameMaxTime;
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
	static bool ImportPointObj(TArray<FPointObjData>& DataList, TArray<FString>& NameList);

	UFUNCTION(BlueprintCallable, Category = Character)
	static bool ImportCharaParam(FLumiCharaParam& CharaParam);

	UFUNCTION(BlueprintCallable, Category = Character)
	static bool ImportCharaSkill(FLumiSkill& CharaSkill);

	UFUNCTION(BlueprintCallable, Category = Character)
	static bool ImportGameParam(FLumiGameParam& CharaSkill);
};

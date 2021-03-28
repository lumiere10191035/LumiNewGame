// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadImportData.h"

void UReadImportData::TestLoadTable()
{
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Data/LumiTest.LumiTest'"));
	FString ContextString;
	TArray<FName> RowNames;
	RowNames = pDataTable->GetRowNames();
	for (auto& name : RowNames)
	{
		FLumiImportData* pRow = pDataTable->FindRow<FLumiImportData>(name, ContextString);
		if (pRow)
		{
			check(GEngine != nullptr);
			TArray<FStringFormatArg> formatArray;
			formatArray.Add(name.ToString());
			FString msg = FString::Format(TEXT("Get New Data!! {0}"), formatArray);
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, msg);
		}
	}
}

void UReadImportData::InitImportData()
{

}

bool UReadImportData::ImportLevelData(FLumiLevelData& Data, int Level)
{
	bool ret = false;
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Data/LumiLevelData.LumiLevelData'"));
	FString ContextString;
	TArray<FName> RowNames;
	RowNames = pDataTable->GetRowNames();
	if (Level < 11)
	{
		FLumiLevelData* tempData = pDataTable->FindRow<FLumiLevelData>(FName(FString::FromInt(Level)), ContextString);
		if (tempData != nullptr)
		{
			Data = *tempData;
			ret = true;
		}
	}
	return ret;
}

bool UReadImportData::ImportPointObj(TArray<FPointObjData>& DataList)
{
	bool ret = false;
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Data/LumiPointObject.LumiPointObject'"));
	FString ContextString;
	TArray<FName> RowNames;
	DataList.Empty();
	RowNames = pDataTable->GetRowNames();
	for (auto name : RowNames)
	{

		FPointObjData* tempData = pDataTable->FindRow<FPointObjData>(name, ContextString);
		if (tempData != nullptr)
		{
			FPointObjData data = *tempData;
			DataList.Emplace(*tempData);
			ret = true;
		}
	}
	
	return ret;
}

bool UReadImportData::ImportCharaParam(FLumiCharaParam& CharaParam)
{
	bool ret = false;
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Data/LumiCharaParam.LumiCharaParam'"));
	FString ContextString;
	TArray<FName> RowNames;
	RowNames = pDataTable->GetRowNames();
	for (auto name : RowNames)
	{

		FLumiCharaParam* tempData = pDataTable->FindRow<FLumiCharaParam>(name, ContextString);
		if (tempData != nullptr)
		{
			CharaParam = *tempData;
			return true;
		}
	}

	return ret;
}

bool UReadImportData::ImportCharaDash(FLumiDash& _dashData)
{
	bool ret = false;
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Data/BoostUpData.BoostUpData'"));
	FString ContextString;
	TArray<FName> RowNames;
	RowNames = pDataTable->GetRowNames();
	for (auto name : RowNames)
	{

		FLumiDash* tempData = pDataTable->FindRow<FLumiDash>(name, ContextString);
		if (tempData != nullptr)
		{
			_dashData = *tempData;
			return true;
		}
	}

	return ret;
}

bool UReadImportData::ImportGameParam(FLumiGameParam& CharaSkill)
{
	bool ret = false;
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Data/LumiGameParam.LumiGameParam'"));
	FString ContextString;
	TArray<FName> RowNames;
	RowNames = pDataTable->GetRowNames();
	for (auto name : RowNames)
	{

		FLumiGameParam* tempData = pDataTable->FindRow<FLumiGameParam>(name, ContextString);
		if (tempData != nullptr)
		{
			CharaSkill = *tempData;
			return true;
		}
	}

	return ret;
}

bool UReadImportData::ImportAllSkillData(TArray<FSkillData>& _SkillList)
{
	bool ret = false;
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Data/SkillData.SkillData'"));
	FString ContextString;
	TArray<FName> RowNames;
	_SkillList.Empty();
	RowNames = pDataTable->GetRowNames();
	for (auto name : RowNames)
	{
		
		FSkillData* tempData = pDataTable->FindRow<FSkillData>(name, ContextString);
		if (tempData != nullptr)
		{
			_SkillList.Emplace(*tempData);
			ret = true;
		}
	}

	return ret;
}

bool UReadImportData::ImportAllEnemyData(TArray<FEnemyData>& EnemyList)
{
	bool ret = false;
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Data/EnemyData.EnemyData'"));
	FString ContextString;
	TArray<FName> RowNames;
	EnemyList.Empty();
	RowNames = pDataTable->GetRowNames();
	for (auto name : RowNames)
	{

		FEnemyData* tempData = pDataTable->FindRow<FEnemyData>(name, ContextString);
		if (tempData != nullptr)
		{
			EnemyList.Emplace(*tempData);
			ret = true;
		}
	}
	return ret;
}

bool UReadImportData::ImportAllPointPosData(TArray<FPointPosSet>& _posList)
{
	bool ret = false;
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Data/PointPosSet.PointPosSet'"));
	FString ContextString;
	TArray<FName> RowNames;
	_posList.Empty();
	RowNames = pDataTable->GetRowNames();
	for (auto name : RowNames)
	{

		FPointPosSet* tempData = pDataTable->FindRow<FPointPosSet>(name, ContextString);
		if (tempData != nullptr)
		{
			_posList.Emplace(*tempData);
			ret = true;
		}
	}
	return ret;
}



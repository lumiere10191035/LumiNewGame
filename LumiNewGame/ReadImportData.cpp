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

bool UReadImportData::ImportPointObj(TArray<FPointObjData>& DataList, TArray<FString>& NameList)
{
	bool ret = false;
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Data/LumiPointObject.LumiPointObject'"));
	FString ContextString;
	TArray<FName> RowNames;
	DataList.Empty();
	NameList.Empty();
	RowNames = pDataTable->GetRowNames();
	for (auto name : RowNames)
	{

		FPointObjData* tempData = pDataTable->FindRow<FPointObjData>(name, ContextString);
		if (tempData != nullptr)
		{
			FPointObjData data = *tempData;
			DataList.Emplace(*tempData);
			NameList.Emplace(name.ToString());
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

bool UReadImportData::ImportCharaSkill(FLumiSkill& CharaSkill)
{
	bool ret = false;
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Data/LumiSkillFlash.LumiSkillFlash'"));
	FString ContextString;
	TArray<FName> RowNames;
	RowNames = pDataTable->GetRowNames();
	for (auto name : RowNames)
	{

		FLumiSkill* tempData = pDataTable->FindRow<FLumiSkill>(name, ContextString);
		if (tempData != nullptr)
		{
			CharaSkill = *tempData;
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

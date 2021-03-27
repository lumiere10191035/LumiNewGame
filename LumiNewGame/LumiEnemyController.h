// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LumiEnemyUnit.h"
#include "ReadImportData.h"

/**
 * 
 */
class LUMINEWGAME_API LumiEnemyController
{
public:
	LumiEnemyController();
	~LumiEnemyController();

	TArray<FEnemyData> EnemyDataList;

	bool SpawnEnemyRandom(int num, FEnemyData& _data);
};

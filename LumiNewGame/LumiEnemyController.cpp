// Fill out your copyright notice in the Description page of Project Settings.


#include "LumiEnemyController.h"
#include "LumiNewGameGameModeBase.h"

LumiEnemyController::LumiEnemyController()
{
}

LumiEnemyController::~LumiEnemyController()
{
}

bool LumiEnemyController::SpawnEnemyRandom(int num, FEnemyData& _data)
{
	if (EnemyDataList.Num() == 0)
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("No Enemy Data"));
		return false;
	}
		
	int enemyNum = num % EnemyDataList.Num();
	_data = EnemyDataList[enemyNum];
	return true;
}

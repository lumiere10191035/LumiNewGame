#include "LumiNewGameGameModeBase.h"
#include "LumiPlayerController.h"
#include "LumiSkillBullet.h"
#include "PointNPC.h"

bool ALumiNewGameGameModeBase::CreateNewUserUMG(FString path)
{
	bool ret = false;

	TSubclassOf<UUserWidget> MyWidgetClass = LoadClass<UUserWidget>(NULL, *path);
	
	if (MyWidgetClass != nullptr)
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			MyWidgetInst = CreateWidget<UUserWidget>(PC, MyWidgetClass);
			if (MyWidgetInst != nullptr)
			{
				MyWidgetInst->AddToViewport();
				ret = true;
			}
		}
	}
	return ret;
}

AActor* ALumiNewGameGameModeBase::CreateNewActorByBP(FString path)
{
	TSubclassOf<UClass> BlueprintClass = StaticLoadClass(ALumiSkillBullet::StaticClass(), NULL, *path);
	AActor* spawnActor = GetWorld()->SpawnActor<AActor>(BlueprintClass);
	if (spawnActor == nullptr)
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Failed to load path::") + path);
	}
	
	return spawnActor;
}

bool ALumiNewGameGameModeBase::GetSkillDataById(FSkillData& _skillData, int _skillId)
{
	return gameUtility->GetSkillDataById(_skillData, _skillId);
}

bool ALumiNewGameGameModeBase::CreateNewEnemyUnit(int _num)
{
	bool ret = false;
	FEnemyData _data;

	if (enemyController->SpawnEnemyRandom(_num, _data))
	{
		FVector SpawnPosition;
		float x = FMath::FRandRange(_data.PosX_1, _data.PosX_2);
		float y = FMath::FRandRange(_data.PosY_1, _data.PosY_2);
		SpawnPosition = FVector(x, y, _data.PosZ);

		float r_p = FMath::FRandRange(-180.f, 180.f);
		FRotator SpawnRotation = FRotator(0.f, r_p, 0.f);

		TSubclassOf<class ALumiEnemyUnit> LumiEnemyClass = LumiEnemyClassList[_data.EnemyType - 1];
		if (LumiEnemyClass)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = GetInstigator();
			ALumiEnemyUnit* spawnActor = GetWorld()->SpawnActor<ALumiEnemyUnit>(LumiEnemyClass, SpawnPosition, SpawnRotation, spawnParams);
			if (spawnActor)
			{
				spawnActor->InitEnemyUnit(_data);
				ret = true;
			}
		}
	}
	return ret;
}

bool ALumiNewGameGameModeBase::CreateNewPointUnit()
{
	bool ret = false;

	float randFloat = FMath::FRand();
	int selectedObj = randFloat > 0.6f ? 1 : 0;
	FPointObjData pointData = gameUtility->PointObjDataList[selectedObj];

	// Rand position
	FVector PointPosition;
	int randMax = gameUtility->PointPosList.Num() - 1;
	int randPosNum = FMath::RandRange(0, randMax);
	if (gameUtility->PointPosList.Num() > randPosNum)
	{
		FPointPosSet pointPos = gameUtility->PointPosList[randPosNum];
		float x = FMath::FRandRange(pointPos.PosX1, pointPos.PosX2);
		float y = FMath::FRandRange(pointPos.PosY1, pointPos.PosY2);
		PointPosition.X = x;
		PointPosition.Y = y;
		PointPosition.Z = pointPos.PosZ;
	}
	else
	{
		return false;
	}

	if (LumiPointClassList.Num() != 0)
	{
		TSubclassOf<class APointNPC> PointNPCClass = LumiPointClassList[0];
		if (PointNPCClass)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = GetInstigator();
			APointNPC* spawnActor = GetWorld()->SpawnActor<APointNPC>(PointNPCClass, PointPosition, FRotator::ZeroRotator, spawnParams);
			if (spawnActor)
			{
				spawnActor->UpdateInitData(pointData);
				ret = true;
			}
		}
	}

	return ret;
}

float ALumiNewGameGameModeBase::GetSkillDamageRatio(int _typeAtk, int _typeDef)
{
	float ret = 1.f;
	if (gameUtility->DamageRatio.Contains(_typeAtk))
	{
		TMap<int, float> nextList = gameUtility->DamageRatio[_typeAtk];
		if (nextList.Contains(_typeDef))
		{
			return nextList[_typeDef];
		}
	}
	return ret;
}
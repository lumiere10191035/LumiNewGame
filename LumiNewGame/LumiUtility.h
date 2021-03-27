// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReadImportData.h"

/**
 * 
 */
class LUMINEWGAME_API LumiUtility
{
public:
	LumiUtility();
	~LumiUtility();

	bool GetSkillDataById(FSkillData& _skillData, int _skillId);

	TMap<int, TMap<int, float>> DamageRatio;
	TMap<int, FSkillData> SystemSkillData;
};


enum GAME_SET_STATE {
	GAME_RESULT_DEFAULT = 0,
	GAME_RESULT_WIN = 1,
	GAME_RESULT_TIMEUP = 2,
	GAME_RESULT_PLAYERDEAD = 3,
};

enum GAME_SKILL_TYPE {
	SKILL_TYPE_DEFAULT = 0,
	SKILL_TYPE_FIRE = 1,
	SKILL_TYPE_WATER = 2,
	SKILL_TYPE_THUNDER = 3,
	SKILL_TYPE_DEVINE = 4,
};
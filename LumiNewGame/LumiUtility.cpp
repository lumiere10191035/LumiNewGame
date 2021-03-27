// Fill out your copyright notice in the Description page of Project Settings.


#include "LumiUtility.h"

LumiUtility::LumiUtility()
{
}

LumiUtility::~LumiUtility()
{
}

bool LumiUtility::GetSkillDataById(FSkillData& _skillData, int _skillId)
{
	bool ret = false;

	if (SystemSkillData.Contains(_skillId))
	{
		_skillData = SystemSkillData[_skillId];
		ret = true;
	}
	return ret;
}
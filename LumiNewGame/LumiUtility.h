// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LUMINEWGAME_API LumiUtility
{
public:
	LumiUtility();
	~LumiUtility();
};


enum GAME_SET_STATE {
	GAME_RESULT_DEFAULT = 0,
	GAME_RESULT_WIN = 1,
	GAME_RESULT_TIMEUP = 2,
	GAME_RESULT_PLAYERDEAD = 3,
};

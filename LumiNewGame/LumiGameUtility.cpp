#include "LumiNewGameGameModeBase.h"
#include "LumiPlayerController.h"
#include "LumiSkillBullet.h"

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
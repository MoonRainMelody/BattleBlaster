// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UBattleBlasterGameInstance::ChangeLevel(int32 LevelIndex)
{
	if(LevelIndex > 0 && LevelIndex <= LastLevelIndex)
	{
		CurrentLevelIndex = LevelIndex;
		FString LevelName = FString::Printf(TEXT("Level_%d"), CurrentLevelIndex);
		UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
	}
}

void UBattleBlasterGameInstance::LoadNextLevel() 
{	
	if (CurrentLevelIndex < LastLevelIndex)
	{
		ChangeLevel(CurrentLevelIndex + 1);
	}
	else
	{
		RestartGame();
	}
}


void UBattleBlasterGameInstance::RestartCurrentLevel()
{
	ChangeLevel(CurrentLevelIndex);
}


void UBattleBlasterGameInstance::RestartGame()
{
	ChangeLevel(1);
}


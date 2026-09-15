// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BattleBlasterGameInstance.h"
#include "EnemyTower.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyTower::StaticClass(), Towers);

	TowerCount = Towers.Num();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (Tank == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find the Tank Actor!"));
		}
	}

	int32 LoopIndex = 0;
	while (LoopIndex < TowerCount)
	{
		AActor* TowerActor = Towers[LoopIndex];
		if (TowerActor)
		{
			AEnemyTower* Tower = Cast<AEnemyTower>(TowerActor);
			if(Tower && Tank)
			{
				Tower->Tank = Tank;
				UE_LOG(LogTemp, Display, TEXT("%s setting the Tank Variable!"),*Tower->GetActorNameOrLabel());
			}
	
		}
		LoopIndex++;
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		ScreenMessage = CreateWidget<UScreenMessage>(PlayerController, ScreenClass);
		if (ScreenMessage)
		{
			ScreenMessage->AddToPlayerScreen();
			ScreenMessage->SetMessageText("Get Ready!");
		}
		
	}



	CountdownSeconds = CountdownDelay;

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountdownTimerOut,1.0f, true);
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;

	if(DeadActor == Tank)
	{
		if (Tank)
		{
			Tank->HandleDestruction();
		}
		IsGameOver = true;
	}
	else
	{
		AEnemyTower* DestroyedTower = Cast<AEnemyTower>(DeadActor);
		if(DestroyedTower)
		{
			DestroyedTower->HandleDestruction();
			TowerCount--;
			if(TowerCount <= 0)
			{
				IsGameOver = true;
				IsPlayerWinner = true;
			}
		}
	}

	if(IsGameOver)
	{
		FString GameOverMessage = IsPlayerWinner ? TEXT("You Win!") : TEXT("You Lose!");
		if (ScreenMessage)
		{
			ScreenMessage->SetVisibility(ESlateVisibility::Visible);
			ScreenMessage->SetMessageText(GameOverMessage);
		}
		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBlasterGameMode::OnGameOverTimerOut, StartDelay , false);
	}
}

void ABattleBlasterGameMode::OnCountdownTimerOut()
{
	CountdownSeconds--;
	if(CountdownSeconds > 0)
	{	
		if (ScreenMessage)
		{
			ScreenMessage->SetMessageText(FString::FromInt(CountdownSeconds));
		}
	}
	else if( CountdownSeconds == 0)
	{	
		if (ScreenMessage)
		{
			ScreenMessage->SetMessageText("Go!!");
		}
		if (Tank)
		{
			Tank->SetPlayerEnabledState(true);
		}
	}
	else {
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		if (ScreenMessage)
		{
			ScreenMessage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABattleBlasterGameMode::OnGameOverTimerOut()
{
	UGameInstance* GameInstance =  GetGameInstance();
	if(GameInstance)
	{
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if(BattleBlasterGameInstance)
		{
			if (IsPlayerWinner)
			{
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else
			{
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}

}

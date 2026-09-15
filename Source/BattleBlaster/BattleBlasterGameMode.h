// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Tank.h"
#include "ScreenMessage.h"
#include "BattleBlasterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** 玩家坦克引用：UPROPERTY 保证被 GC 追踪（原为裸指针，存在悬空风险） */
	UPROPERTY()
	TObjectPtr<ATank> Tank;

	int32 TowerCount;

	void ActorDied(AActor* DeadActor);

	UPROPERTY(EditAnywhere)
	float StartDelay = 3.0f;

	UPROPERTY(EditAnywhere)
	int32 CountdownDelay = 3;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenMessage> ScreenClass;

	/** HUD 消息控件：UPROPERTY 保证被 GC 追踪 */
	UPROPERTY()
	TObjectPtr<UScreenMessage> ScreenMessage;

	int32 CountdownSeconds;

	FTimerHandle CountdownTimerHandle;

	void OnCountdownTimerOut();

	void OnGameOverTimerOut();

	bool IsPlayerWinner = false;
};

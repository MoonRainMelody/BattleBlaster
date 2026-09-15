// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.h"
#include "EnemyTower.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API AEnemyTower : public ABasePawn
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** 目标坦克（由 GameMode 注入）：UPROPERTY 保证 GC 追踪 */
	UPROPERTY()
	TObjectPtr<ATank> Tank;


	UPROPERTY(EditAnywhere)
	float FireRange = 300.0f;

	UPROPERTY(EditAnywhere)
	float FireRate = 2.0f;

	bool InFireRange();

	void CheckFireCondition();

	void HandleDestruction();

protected:
	/** 循环开火定时器（原为 BeginPlay 内的局部变量，句柄丢失后无法清除/暂停） */
	FTimerHandle FireRateTimerHandle;
};

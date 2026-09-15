// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTower.h"

void AEnemyTower::BeginPlay()
{
	Super::BeginPlay();
	// 使用成员句柄保存定时器，便于在销毁时清除（原为局部变量，句柄丢失后无法取消）
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &AEnemyTower::CheckFireCondition, FireRate, true);
}
bool AEnemyTower::InFireRange()
{
	bool result = false;
	if (Tank)
	{
		float DistanceToTank = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		result = (DistanceToTank <= FireRange);
		
	}
	return result;
}
void AEnemyTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(InFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
}


void AEnemyTower::CheckFireCondition()
{
	if (Tank && InFireRange() && Tank->bAlive)
	{	
		UE_LOG(LogTemp, Display, TEXT("%s is firing!"), *GetActorNameOrLabel());
		Fire();
	}
	
}

void AEnemyTower::HandleDestruction()
{
	Super::HandleDestruction();
	// 先停掉循环开火定时器，避免销毁后仍触发回调
	GetWorldTimerManager().ClearTimer(FireRateTimerHandle);
	Destroy();
}
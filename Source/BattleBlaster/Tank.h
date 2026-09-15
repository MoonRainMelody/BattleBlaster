// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"


#include "Tank.generated.h"

UCLASS()
class BATTLEBLASTER_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	// 设置默认值
	ATank();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* TankMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")

	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")

	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = "Input")

	UInputAction* FireAction;

	void HandleDestruction();

	void SetPlayerEnabledState(bool Enabled);

	/** 本机玩家控制器缓存：用 UPROPERTY 让 GC 追踪，避免悬空指针 */
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	bool bAlive = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// 弹簧臂组件必须在类的内部声明
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	// 摄像机组件必须在类的内部声明
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere)
	float TurnRate = 50.0f;
	
	void MoveInput(const FInputActionValue& Value);

	void TurnInput(const FInputActionValue& Value);


protected:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 0.5f; // 开火冷却时间（秒）

	bool bCanFire = true;  // 开火状态锁

	FTimerHandle FireRateTimerHandle;

	void CheckFireCondition(); // 替代原本直接绑定的 Fire 函数
	void ResetFire();          // 冷却结束后的重置函数
};
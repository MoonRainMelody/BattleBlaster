// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

// 构造函数，注意前面的 ATank::
ATank::ATank()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
	
		if(ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{	
			
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(TankMappingContext, 0);
			}
		}
	}

	SetPlayerEnabledState(false);
}


// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		FVector HitLocation = HitResult.ImpactPoint;

		RotateTurret(HitLocation);

		//DrawDebugSphere(GetWorld(), HitLocation, 25.0f, 12, FColor::Red, false, -1.0f);
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::MoveInput);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::TurnInput);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATank::CheckFireCondition);
	}
}

void ATank::MoveInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FVector DeltaLocation = FVector(0.0f, 0.0f, 0.0f);
	DeltaLocation.X = Speed * InputValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::TurnInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);
	DeltaRotation.Yaw = TurnRate * InputValue * GetWorld()->GetDeltaSeconds();
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetPlayerEnabledState(false);

	bAlive = false;
}

void ATank::SetPlayerEnabledState(bool Enabled)
{
	// PlayerController 可能尚未就绪（如 BeginPlay 早期、非玩家控制），必须先判空
	if (!PlayerController)
	{
		return;
	}

	if(Enabled)
	{
		EnableInput(PlayerController);
	}
	else
	{
		DisableInput(PlayerController);
	}	
	PlayerController->bShowMouseCursor = Enabled;
}

void ATank::CheckFireCondition()
{
	// 只有在没有被锁定时才能开火
	if (bCanFire)
	{
		Fire(); // 调用父类 BasePawn 的开火逻辑生成子弹
		bCanFire = false; // 立刻上锁，进入冷却状态

		// 启动冷却倒计时，FireRate 秒后自动调用 ResetFire 解锁
		GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATank::ResetFire, FireRate, false);
	}
}

void ATank::ResetFire()
{
	bCanFire = true; // 冷却时间到，重新允许开火
}

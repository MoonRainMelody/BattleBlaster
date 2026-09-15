// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));

	RootComponent = CapsuleComp;

	BaseMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(CapsuleComp);
	TurretMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector VectorToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.0f, VectorToTarget.Rotation().Yaw, 0.0f);

	FRotator InterpolatedRotation = FMath::RInterpTo(TurretMesh->GetComponentRotation(), 
		LookAtRotation, 
		GetWorld()->GetDeltaSeconds(), 
		TurretRotationSpeed);

	TurretMesh->SetWorldRotation(InterpolatedRotation);
}

void ABasePawn::Fire()
{
	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	//DrawDebugSphere(GetWorld(), SpawnLocation, 25.0f, 12, FColor::Red, false, 3.0f);

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,SpawnLocation, SpawnRotation);
	if (Projectile)
	{
		Projectile->SetOwner(this);
	}
}

void ABasePawn::HandleDestruction()
{
	if(DeathNiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			DeathNiagaraSystem,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
	if(DeathCameraShakeClass)
	{
		// 无本地玩家控制器时（专用服务器等）跳过，避免空指针解引用
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->ClientStartCameraShake(DeathCameraShakeClass);
		}
	}
}



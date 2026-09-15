// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

	TrailNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailNiagaraComponent"));
	TrailNiagaraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr) return; // 防御性编程，如果Owner丢失直接返回

	// 核心修复：检查 OtherActor 是不是另一个子弹
	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		// 尝试将撞到的物体转换为 AProjectile 类
		AProjectile* HitProjectile = Cast<AProjectile>(OtherActor);
		if (HitProjectile)
		{
			// 如果转换成功，说明撞到的是同行，直接退出函数，不造成伤害也不销毁自己
			return;
		}

		if (HitNiagaraSystem)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				HitNiagaraSystem,
				GetActorLocation(),
				GetActorRotation()
			);
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}
		if(HitCameraShakeClass)
		{	
			APlayerController* PlaerConr = UGameplayStatics::GetPlayerController(this, 0);
			if(PlaerConr)
			{
				PlaerConr->ClientStartCameraShake(HitCameraShakeClass);
			}
			
		}

		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, UDamageType::StaticClass());
	}

	// 只有撞到了非子弹、非主人的有效物体，才会执行到这里进行销毁
	Destroy();
}



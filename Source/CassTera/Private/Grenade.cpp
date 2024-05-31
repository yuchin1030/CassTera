// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>
#include <TestEnemyy.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "../CassTeraCharacter.h"

AGrenade::AGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(10, 10, 15));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(0.75));
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
	//Bomb();
}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bBeforeBomb == false)
	{
		// 터지기 직전 수류탄 위치 저장
		bombLoc = meshComp->GetComponentLocation();
	}
}

void AGrenade::BeforeBomb(ACassTeraCharacter* pc)
{
	meshComp->SetSimulatePhysics(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (pc != nullptr)
	{
		FVector throwDir = GetActorForwardVector() * 2 + GetActorUpVector();
		// from 에서 to 까지의 방향(플레이어에서 수류탄까지의 방향)
		//FVector newVel = UKismetMathLibrary::GetDirectionUnitVector(pc->GetActorLocation(), bombLoc);

		float speed = 500;
		meshComp->SetPhysicsLinearVelocity(throwDir * speed);

		Bomb();
	}

}

void AGrenade::Bomb()
{
	FTimerHandle bombHandler;
	GetWorld()->GetTimerManager().SetTimer(bombHandler, [&] () {

		bBeforeBomb = true;

		TArray<FOverlapResult> hitsArr;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		bool bHit = GetWorld()->OverlapMultiByObjectType(hitsArr, bombLoc, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(500), params);

		DrawDebugSphere(GetWorld(), bombLoc, 500, 16, FColor::Blue, false, 5, 0, 5);
		for (int i = 0; i < hitsArr.Num(); i++)
		{
			auto* enemy = Cast<ATestEnemyy>(hitsArr[i].GetActor());
			if (enemy)
			{
				enemy->OnDamaged(1);

			}
		}
			
		spawnedBombVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bombVFX, bombLoc);	//

	}, 2, false);

	FTimerHandle vfxDestroyHandler;
	GetWorldTimerManager().SetTimer(vfxDestroyHandler, [&]() {

		spawnedBombVFX->DestroyComponent();
		this->Destroy();

	}, 4, false);
}


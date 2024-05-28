// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>
#include <TestEnemyy.h>

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


}

void AGrenade::Bomb()
{
	FTimerHandle bombHandler;
	GetWorld()->GetTimerManager().SetTimer(bombHandler, [&] () {


		TArray<FOverlapResult> hitsArr;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		//
		bool bHit = GetWorld()->OverlapMultiByObjectType(hitsArr, GetActorLocation(), FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(500), params);

		for (int i = 0; i < hitsArr.Num(); i++)
		{
			auto* enemy = Cast<ATestEnemyy>(hitsArr[i].GetActor());
			if (enemy)
			{
				enemy->OnDamaged(1);

			}
		}
			
		spawnedBombVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bombVFX, GetActorTransform());	//

	}, 2, false);

	FTimerHandle vfxDestroyHandler;
	GetWorldTimerManager().SetTimer(vfxDestroyHandler, [&]() {

		spawnedBombVFX->DestroyComponent();
		this->Destroy();

	}, 4, false);
}


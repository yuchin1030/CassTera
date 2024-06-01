// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>
#include <TestEnemyy.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "CassTeraCharacter.h"
#include "HidePlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>

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
	
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		playerChar = Cast<ACassTeraCharacter>(*ActorItr);
		if(playerChar) {
			break;
		}
	}
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
		FVector newVel = UKismetMathLibrary::GetDirectionUnitVector(pc->GetActorLocation(), bombLoc);

		float speed = 1000;
		//meshComp->AddImpulse(throwDir);
		meshComp->SetPhysicsLinearVelocity(newVel * speed);

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
		
		/*if (bHit)
		{*/
			DrawDebugSphere(GetWorld(), bombLoc, 500, 16, FColor::Blue, false, 5, 0, 5);

			
//			ACassTeraCharacter* playerChar = nullptr;
//			for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
//			{
//				playerChar = Cast<ACassTeraCharacter>(*ActorItr);
//// 				enemy = Cast<AHidePlayer>(*ActorItr);
//			}

			for (int i = 0; i < hitsArr.Num(); i++)
			{
				auto* enemy = Cast<AHidePlayer>(hitsArr[i].GetActor());
								
				// playerChar = Cast<ACassTeraCharacter>(GetOwner());

				//playerChar = Cast<ACassTeraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

				// enemy 가 맞으면
				if (enemy)
				{
					enemy->OnTakeDamage();

					// 죽으면
					if (enemy->bDie)
					{
						if (playerChar)
						{
							playerChar->ShowKillUI();
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Can't find player (Showkillui)"));
						}
					}
				}
				else
				{
					if (playerChar)
					{
						playerChar->NotEnemyResult();
						UE_LOG(LogTemp, Warning, TEXT(""));

					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Can't find player(notenemyresult)"));
					}
					
				}
 			}
		//}
		
			
		spawnedBombVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bombVFX, bombLoc);	//

	}, 2, false);

	FTimerHandle vfxDestroyHandler;
	GetWorldTimerManager().SetTimer(vfxDestroyHandler, [&]() {

		spawnedBombVFX->DestroyComponent();
		this->Destroy();

	}, 4, false);
}


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
#include "CassteraGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

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
	
	gs = Cast<ACassteraGameState>(UGameplayStatics::GetGameState(GetWorld()));

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



void AGrenade::ServerRPC_BeforeBomb_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ServerRPC_BeforeBomb"));

	//MultiRPC_BeforeBomb();


	if (playerChar != nullptr)
	{
		//FVector throwDir = GetActorForwardVector() * 2 + GetActorUpVector();
	

		// from 에서 to 까지의 방향(플레이어에서 수류탄까지의 방향)
		newVel = UKismetMathLibrary::GetDirectionUnitVector(playerChar->GetActorLocation(), bombLoc);
		speed = 1000;

		/*meshComp->SetPhysicsLinearVelocity(newVel * speed);*/
		MultiRPC_BeforeBomb(newVel, speed);

		ServerRPC_Bomb();
	}

}

void AGrenade::MultiRPC_BeforeBomb_Implementation(FVector _newVel, float _speed)
{	// FVector _newVel, float _speed
	newVel = _newVel;
	speed = _speed;

	meshComp->SetSimulatePhysics(true);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	UE_LOG(LogTemp, Warning, TEXT("detach grenade"));

	meshComp->SetPhysicsLinearVelocity(newVel * speed);


}

void AGrenade::ServerRPC_Bomb_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ServerRPC_Bomb"));

	FTimerHandle bombHandler;
	GetWorld()->GetTimerManager().SetTimer(bombHandler, [&] () {
		if (explosionSound)
		{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), explosionSound, GetActorLocation());

		}
		bBeforeBomb = true;

		TArray<FOverlapResult> hitsArr;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		bool bHit = GetWorld()->OverlapMultiByObjectType(hitsArr, bombLoc, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(500), params);
					
		if (bHit)
		{

			for (int i = 0; i < hitsArr.Num(); i++)
			{
				auto* enemy = Cast<AHidePlayer>(hitsArr[i].GetActor());
								
				// playerChar = Cast<ACassTeraCharacter>(GetOwner());

				//playerChar = Cast<ACassTeraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

				// enemy 가 맞으면
				if (enemy)
				{
					enemy->ServerRPC_Damaged();

					// 죽으면
					if (enemy->currentHP == 0)
					{
// 						enemy->ServerRPC_Die();

						if (playerChar)
						{
							playerChar->ServerRPC_KillUI();
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Can't find player (Showkillui)"));
						}
					}
				}
		
 			}
		}
		else
		{
			if (playerChar)
			{
				playerChar->ServerRPC_WorngShot();

				if (gs)
				{
					gs->ServerRPC_DecreaseTime();
				}

			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Can't find player(notenemyresult)"));
			}

		}
			
		MultiRPC_Bomb();

	}, 2, false);


}

void AGrenade::MultiRPC_Bomb_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("MultiRPC_Bomb"));

	DrawDebugSphere(GetWorld(), bombLoc, 500, 16, FColor::Blue, false, 5, 0, 5);

	spawnedBombVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bombVFX, bombLoc);

	FTimerHandle vfxDestroyHandler;
	GetWorldTimerManager().SetTimer(vfxDestroyHandler, [&]() {

		spawnedBombVFX->DestroyComponent();
		this->Destroy();

	}, 4, false);
}
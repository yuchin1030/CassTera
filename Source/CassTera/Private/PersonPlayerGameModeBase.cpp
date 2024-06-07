// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonPlayerGameModeBase.h"
#include "PersonPlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "Objects.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "EngineUtils.h"
#include "CassteraGameState.h"
#include "Kismet/GameplayStatics.h"
#include "ResultWidget.h"

APersonPlayerGameModeBase::APersonPlayerGameModeBase()
{
	// 틱이 돌도록 설정해야 된다.
	PrimaryActorTick.bCanEverTick = true;

	resultWidget = Cast<UResultWidget>(CreateWidget(GetWorld(), wbp_resultWidget));
	gs = Cast<ACassteraGameState>(UGameplayStatics::GetGameState(GetWorld()));
	PlayerControllerClass = APersonPlayerController::StaticClass();

}

void APersonPlayerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//// 월드에 있는 모든 hidePlayer 찾아서 배열에 저장 
	//for (TActorIterator<AHidePlayer> it(GetWorld()); it; ++it)
	//{
	//	if (it)
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Rest HidePlayer Count : %d"), hidePlayerCount);
	//	}
	//}
	//hidePlayerCount = hidePlayers.Num();
}

void APersonPlayerGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	// 현재 술래 숫자가 최대 술래 숫자보다 작다면
	if (curSeaker < maxSeaker)
	{
		// 랜덤으로 역할을 배정한다
		if (playerRate < FMath::RandRange(0, 100))
		{
			DefaultPawnClass = SeakPlayerPawn;
			curSeaker++;


		}
		else
		{
			DefaultPawnClass = HidePlayerPawn;
		}
	}
	// 그렇지 않다면, 
	else
	{
		// 숨는 역할만 배정한다.
		DefaultPawnClass = HidePlayerPawn;
	}

	Super::PostLogin(NewPlayer);

	if (Objects_BP != nullptr)
	{
		Objects = Cast<AObjects>(Objects_BP);

		if (Objects != nullptr)
		{
			Objects->RandomSpawn();

		}
	}


}

void APersonPlayerGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//UE_LOG(LogTemp, Warning, TEXT("%d"), curSeaker);
}


void APersonPlayerGameModeBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APersonPlayerGameModeBase, Objects);
}

void APersonPlayerGameModeBase::DecreaseHidePlayerCount()
{
	hidePlayerCount = hidePlayerCount - 1;
	UE_LOG(LogTemp, Error, TEXT("Rest HidePlayer Count : %d"), hidePlayerCount);
	if (hidePlayerCount <= 0)
	{
		MultiRPC_DecreaseHidePlayerCount(hidePlayerCount);
	}
}

void APersonPlayerGameModeBase::MultiRPC_ShowResult_Implementation()
{
	
}

void APersonPlayerGameModeBase::MultiRPC_DecreaseHidePlayerCount_Implementation(int32 _hidePlayerCount)
{
	
}

//UClass* APersonPlayerGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
//{	
//	// 플레이어 컨트롤러에서 폰을 가져오기 위한 기능 재정의
//	APersonPlayerController* pc = Cast<APersonPlayerController>(InController);
//
//	/*UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Controller : %s \nClass : %s"), InController->GetActorNameOrLabel(), pc->GetPlayerPawnClass()->GetFName()));*/
//
//	if (pc != nullptr)
//	{
//		return pc->GetPlayerPawnClass();
//	}
//
//	// 컨트롤러가 없다면 기본 폰을 사용한다
//	return DefaultPawnClass;
//}

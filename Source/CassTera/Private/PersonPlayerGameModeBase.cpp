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
	// ƽ�� ������ �����ؾ� �ȴ�.
	PrimaryActorTick.bCanEverTick = true;

	resultWidget = Cast<UResultWidget>(CreateWidget(GetWorld(), wbp_resultWidget));
	gs = Cast<ACassteraGameState>(UGameplayStatics::GetGameState(GetWorld()));
	PlayerControllerClass = APersonPlayerController::StaticClass();

}

void APersonPlayerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//// ���忡 �ִ� ��� hidePlayer ã�Ƽ� �迭�� ���� 
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
	// ���� ���� ���ڰ� �ִ� ���� ���ں��� �۴ٸ�
	if (curSeaker < maxSeaker)
	{
		// �������� ������ �����Ѵ�
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
	// �׷��� �ʴٸ�, 
	else
	{
		// ���� ���Ҹ� �����Ѵ�.
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
//	// �÷��̾� ��Ʈ�ѷ����� ���� �������� ���� ��� ������
//	APersonPlayerController* pc = Cast<APersonPlayerController>(InController);
//
//	/*UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Controller : %s \nClass : %s"), InController->GetActorNameOrLabel(), pc->GetPlayerPawnClass()->GetFName()));*/
//
//	if (pc != nullptr)
//	{
//		return pc->GetPlayerPawnClass();
//	}
//
//	// ��Ʈ�ѷ��� ���ٸ� �⺻ ���� ����Ѵ�
//	return DefaultPawnClass;
//}

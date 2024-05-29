// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonPlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameModeBase.h>
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>

APersonPlayerController::APersonPlayerController(const FObjectInitializer& ObjectInitializer)
{
	// 값 초기화
	SeakPlayerPawn = nullptr;
	HidePlayerPawn = nullptr;

	// 폰 클래스가 복제되었는지 확인
	bReplicates = true;
}

void APersonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MultiRPC_DefinePawnClass();
}

void APersonPlayerController::MultiRPC_DefinePawnClass_Implementation()
{
	if (IsLocalController())
	{
		//텍스트 파일을 문자열 배열로 로드
		TArray<FString> TextStrings;
		const FString FilePath = FPaths::ProjectContentDir() + "Textfiles/PlayerSettings.txt";
		FFileHelper::LoadFileToStringArray(TextStrings, *FilePath);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FilePath);

		int32 number = FMath::RandRange(0, 100);
		if (rate < number)
		{
			if (TextStrings[0].Contains(TEXT("SeakPlayer")))
			{
				ServerRPC_SetPawn(SeakPlayerPawn);

				return;
			}
		}
		else
		{
			ServerRPC_SetPawn(HidePlayerPawn);

			return;
		}
	}
}

void APersonPlayerController::ServerRPC_SetPawn_Implementation(TSubclassOf<APawn> InPawnClass)
{
	MyPawnClass = InPawnClass;

	//시간 내에 서버에 PawnClass를 가져오지 못한 경우를 대비한다
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

bool APersonPlayerController::ServerRPC_SetPawn_Validate(TSubclassOf<APawn> InPawnClass)
{
	return true;
}

void  APersonPlayerController::GetLifetimeReplicatedProps(TArray < FLifetimeProperty >& OutLifetimeProps)  const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//s
	DOREPLIFETIME(APersonPlayerController, MyPawnClass);
}
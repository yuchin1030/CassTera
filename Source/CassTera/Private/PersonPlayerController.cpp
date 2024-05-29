// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonPlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameModeBase.h>
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>

APersonPlayerController::APersonPlayerController(const FObjectInitializer& ObjectInitializer)
{
	// �� �ʱ�ȭ
	SeakPlayerPawn = nullptr;
	HidePlayerPawn = nullptr;

	// �� Ŭ������ �����Ǿ����� Ȯ��
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
		//�ؽ�Ʈ ������ ���ڿ� �迭�� �ε�
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

	//�ð� ���� ������ PawnClass�� �������� ���� ��츦 ����Ѵ�
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
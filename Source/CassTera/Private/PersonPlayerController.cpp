// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonPlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameModeBase.h>
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>

APersonPlayerController::APersonPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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

	//ServerRPC_DefinePawnClass();
	int32 number = FMath::RandRange(0, 100);
	if (rate < number)
	{
		ServerRPC_SetPawn(SeakPlayerPawn);
	}
	else
	{
		ServerRPC_SetPawn(HidePlayerPawn);
	}
}

void APersonPlayerController::ServerRPC_DefinePawnClass_Implementation()
{
	//MultiRPC_DefinePawnClass();
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
			ServerRPC_SetPawn(SeakPlayerPawn);
		}
		else
		{
			ServerRPC_SetPawn(HidePlayerPawn);
		}
	}
}

void APersonPlayerController::ServerRPC_SetPawn_Implementation(TSubclassOf<APawn> InPawnClass)
{
	APawn* prevPawn = GetPawn();
	
	UnPossess();

	MyPawnClass = InPawnClass;

	//AActor* spt = GetWorld()->GetAuthGameMode()->FindPlayerStart(this);
	//FActorSpawnParameters params;
	//params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//
	//auto* newPawn = GetWorld()->SpawnActor<APawn>(MyPawnClass, spt->GetActorTransform(), params);
	//Possess(newPawn);
	//
		
	if (prevPawn)
		prevPawn->Destroy();

	//�ð� ���� ������ PawnClass�� �������� ���� ��츦 ����Ѵ�
	UE_LOG(LogTemp, Warning, TEXT("%s"), MyPawnClass);
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
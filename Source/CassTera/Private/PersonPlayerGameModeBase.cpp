// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonPlayerGameModeBase.h"
#include "PersonPlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>

APersonPlayerGameModeBase::APersonPlayerGameModeBase()
{
	// ƽ�� ������ �����ؾ� �ȴ�.
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = APersonPlayerController::StaticClass();
	
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

}

void APersonPlayerGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//UE_LOG(LogTemp, Warning, TEXT("%d"), curSeaker);
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

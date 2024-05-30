// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonPlayerGameModeBase.h"
#include "PersonPlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>

APersonPlayerGameModeBase::APersonPlayerGameModeBase()
{
	PlayerControllerClass = APersonPlayerController::StaticClass();
}


UClass* APersonPlayerGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{	
	// �÷��̾� ��Ʈ�ѷ����� ���� �������� ���� ��� ������
	APersonPlayerController* pc = Cast<APersonPlayerController>(InController);

	/*UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Controller : %s \nClass : %s"), InController->GetActorNameOrLabel(), pc->GetPlayerPawnClass()->GetFName()));*/

	if (pc != nullptr)
	{
		return pc->GetPlayerPawnClass();
	}

	// ��Ʈ�ѷ��� ���ٸ� �⺻ ���� ����Ѵ�
	return DefaultPawnClass;
}

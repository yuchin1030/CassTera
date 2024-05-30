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
	// 플레이어 컨트롤러에서 폰을 가져오기 위한 기능 재정의
	APersonPlayerController* pc = Cast<APersonPlayerController>(InController);

	/*UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Controller : %s \nClass : %s"), InController->GetActorNameOrLabel(), pc->GetPlayerPawnClass()->GetFName()));*/

	if (pc != nullptr)
	{
		return pc->GetPlayerPawnClass();
	}

	// 컨트롤러가 없다면 기본 폰을 사용한다
	return DefaultPawnClass;
}

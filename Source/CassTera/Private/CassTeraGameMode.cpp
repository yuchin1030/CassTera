// Copyright Epic Games, Inc. All Rights Reserved.

#include "CassTeraGameMode.h"
#include "CassTeraCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "LobbyWidget.h"

ACassTeraGameMode::ACassTeraGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACassTeraGameMode::StartPlay()
{
	// LobbyWidget_BP 가 있다면
	if (LobbyWidget_BP)
	{
		// LobbyWidget UI를 만든다
		auto lobbyUI = Cast<ULobbyWidget>(CreateWidget<ULobbyWidget>(GetWorld(), LobbyWidget_BP));

		if (lobbyUI)
		{	
			// lobbyUI를 뷰포트에 띄운다
			lobbyUI->AddToViewport();
		}
	}
}

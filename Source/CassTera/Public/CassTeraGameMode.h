// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CassTeraGameMode.generated.h"

UCLASS()
class ACassTeraGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACassTeraGameMode();

	virtual void StartPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULobbyWidget> LobbyWidget_BP;
};




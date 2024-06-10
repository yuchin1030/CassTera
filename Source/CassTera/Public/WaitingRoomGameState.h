// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "WaitingRoomGameState.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API AWaitingRoomGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;


	// 타이머 ui
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UWaitingRoomTimerWidget> WaitingTimerUI_BP;

	UPROPERTY()
	class UWaitingRoomTimerWidget* WaitingTimerUI;

	// 시작 타이머
	UPROPERTY()
	float StartTimer = 30.0f;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetTimer(float DeltaSeconds);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetTimer(float second);

	
};

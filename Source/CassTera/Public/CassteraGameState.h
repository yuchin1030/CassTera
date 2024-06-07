// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CassteraGameState.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API ACassteraGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_CalculateTime();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_CalculateTime(bool _bClearTimer, int32 _minute, int32 _seconds, float _pgPercent, float _totalSeconds);

	UFUNCTION(Server, Reliable)
	void ServerRPC_DecreaseTime();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_DecreaseTime(bool _bDecreasing, int32 _minute, int32 _seconds, int32 _minusSeconds, float _pgPercent, float _totalSeconds);

	UPROPERTY()
	FTimerHandle timerHandler;

	bool bClearTimer = false;

	UPROPERTY()
	bool bDecreasing;

	UPROPERTY()
	int32 seconds = 0;

	UPROPERTY()
	int32 minute = 3;


	UPROPERTY()
	int minusSeconds = 9;

	UPROPERTY()
	float totalSeconds = 180.0f;


	UPROPERTY()
	float pgPercent = 0;

	UPROPERTY()
	class UGameTimerWidget* timerWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGameTimerWidget> WBP_gameTimerWidget;

	UPROPERTY()
	int32 hidePlayerCount;

	UFUNCTION(Server, Reliable)
	void ServerRPC_HidePlayerCount(int32 newHidePlayerCount);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_HidePlayerCount(int32 newHidePlayerCount);

	UPROPERTY()
	class UResultWidget* resultWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UResultWidget> wbp_resultWidget;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ShowResult(bool bWin);

	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowResult(bool _bWin);
};

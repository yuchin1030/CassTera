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
	ACassteraGameState();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category = "Sound");
	class USoundBase* burningTime;

	UPROPERTY()
	bool bIsTraveld = false;

	UFUNCTION(Server, Reliable)
	void ServerRPC_CalculateTime();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_CalculateTime(bool _bClearTimer, int32 _minute, int32 _seconds, float _pgPercent, float _totalSeconds);

	UFUNCTION(Server, Reliable)
	void ServerRPC_DecreaseTime();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_DecreaseTime(bool _bDecreasing, int32 _minute, int32 _seconds, int32 _minusSeconds, float _pgPercent, float _totalSeconds);

	UFUNCTION(Server,Reliable)
	void ServerRPC_CountDown();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_CountDown(int32 _Count);

	UPROPERTY()
	int32 countDown = 10;

	UPROPERTY()
	bool bCount;

	UPROPERTY()
	bool bResult;

	UPROPERTY()
	FTimerHandle countHandle;


	UPROPERTY()
	class ACassTeraCharacter* cassTeraPlayer;
	UPROPERTY()
	class AHidePlayer* hidePlayer;

	UPROPERTY()
	class AHidePlayerCamera* hidePlayerCamera;


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
	void ServerRPC_HidePlayerCount();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_HidePlayerCount(int32 _hidePlayer);

	UPROPERTY()
	class UResultWidget* resultWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UResultWidget> wbp_resultWidget;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ShowResult();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ShowResult();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ShowResult2();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ShowResult2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_DecreaseHidePlayerCount();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_DecreaseHidePlayerCount(int32 _hidePlayer);

};

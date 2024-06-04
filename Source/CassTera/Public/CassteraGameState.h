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
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_CalculateTime();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_CalculateTime();

	UFUNCTION()
	void SetTimer();

	UPROPERTY()
	FTimerHandle timerHandler;

	bool bClearTimer = false;

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
};

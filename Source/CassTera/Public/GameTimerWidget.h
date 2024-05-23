// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API UGameTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	int minute = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	int seconds = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	int minusSeconds = 9;

	UFUNCTION()
	void DecreaseTime();
};

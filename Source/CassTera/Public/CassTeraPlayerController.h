// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CassTeraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API ACassTeraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UGameTimerWidget* gameTimerwidget;

	UPROPERTY()
	class UMainUI* mainUI;

};

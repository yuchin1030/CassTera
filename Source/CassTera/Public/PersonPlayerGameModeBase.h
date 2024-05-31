// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PersonPlayerGameModeBase.generated.h"

/**
 * 
 */
UCLASS(minimalapi)
class APersonPlayerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	APersonPlayerGameModeBase();
	//virtual UClass* GetDefaultPawnClassForController_Implementation(class AController* InController) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Tick( float DeltaSeconds ) override;

	// ����� ù ��° �� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="MySettings")
	TSubclassOf<APawn> SeakPlayerPawn;

	// ����� �� ��° �� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="MySettings")
	TSubclassOf<APawn> HidePlayerPawn;

	// ���� ���� ����
	UPROPERTY()
	int32 playerRate = 50;

	// �ִ� ���� ����
	int32 maxSeaker = 1;

	// ���� ���� ����
	int32 curSeaker = 0;
};


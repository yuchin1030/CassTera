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

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Tick( float DeltaSeconds ) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

	// 사용할 첫 번째 폰 유형
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="MySettings")
	TSubclassOf<APawn> SeakPlayerPawn;

	// 사용할 두 번째 폰 유형
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="MySettings")
	TSubclassOf<APawn> HidePlayerPawn;

	// 랜덤 역할 배정
	UPROPERTY()
	int32 playerRate = 50;

	// 최대 술래 숫자
	int32 maxSeaker = 1;

	// 현재 술래 숫자
	int32 curSeaker = 0;

	// 랜덤 메시 배치
	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	TSubclassOf<class AObjects> Objects_BP;
	UPROPERTY(EditDefaultsOnly, Replicated)
	class AObjects* Objects;

	UPROPERTY()
	int32 hidePlayerCount;
	
	UPROPERTY()
	TArray<class AHidePlayer*> hidePlayers;

	UPROPERTY()
	class AHidePlayer* hidePlayer;

	UFUNCTION()
	void DecreaseHidePlayerCount();
};


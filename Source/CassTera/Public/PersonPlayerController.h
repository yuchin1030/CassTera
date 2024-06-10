// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PersonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API APersonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// 생성자
	APersonPlayerController(const FObjectInitializer & ObjectInitializer);

	FORCEINLINE UClass * GetPlayerPawnClass() {return MyPawnClass;}

	UPROPERTY()
	class UGameTimerWidget* gameTimerwidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGameTimerWidget> WBP_gameTimerWidget;

	UPROPERTY()
	class UMainUI* mainUI;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UMainUI> WBP_MainUI;

	// 채팅
	UPROPERTY()
	class UChatWidget* chatUI;

// 	UFUNCTION(Server, Reliable)
// 	void ServerRPC_CreateTimerUI();
// 
// 	
// 	UFUNCTION(NetMulticast, Reliable)
// 	void MultiRPC_CreateTimerUI();
	

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* aPawn) override;
	
	virtual void Tick(float DeltaTime) override;



	UPROPERTY()
	class ACassteraGameState* gs;
	
	// 올바른 폰 클래스 클라이언트 측 반환
	UFUNCTION(Server, Reliable)
	void ServerRPC_DefinePawnClass();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_DefinePawnClass();

	// 이 컨트롤러에 대해 서버에 폰 클래스 설정
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_SetPawn(TSubclassOf<APawn> InPawnClass);

	// 사용하려는 실제 폰 클래스
	UPROPERTY(Replicated)
	TSubclassOf<APawn> MyPawnClass;

	// 사용할 첫 번째 폰 유형
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="MySettings")
	TSubclassOf<APawn> SeakPlayerPawn;

	// 사용할 두 번째 폰 유형
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="MySettings")
	TSubclassOf<APawn> HidePlayerPawn;

	// 랜덤으로 폰 설정
	UPROPERTY()
	int32 rate = 50;


public:
	//숨는 플레이어 관전모드로 바꾸기
	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeToSpectator(AHidePlayer* hidePlayer, bool isOldPawnDestroy = false);
	UFUNCTION(Client, Reliable)
	void ClientRPC_ChangeToSpectator(AHidePlayer* hidePlayer);

	//숨는 플레이어 관전모드->숨는 폰으로 바꾸기(서버)
	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeToPlayer();

	//바꾸기 실제함수
	UFUNCTION()
	void ChangeToPlayer();

	UPROPERTY()
	class AHidePlayerCamera* spectator;
	UPROPERTY()
	class ACassTeraCharacter* seekPlayer;
	UPROPERTY()
	class AHidePlayer* origin;

	UPROPERTY()
	class APersonPlayerGameModeBase* gm; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HidePlayer")
	TSubclassOf<class AHidePlayerCamera> watcingCam_bp;
	UPROPERTY()
	class AHidePlayerCamera* watchingCam;

	UPROPERTY()
	int32 _hidePlayerCount;

//	UPROPERTY()
//	class APawn* origin;

	UPROPERTY()
	bool bHidePlayerDie;

};

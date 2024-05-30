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

	UPROPERTY()
	class UMainUI* mainUI;

protected:
	virtual void BeginPlay() override;
	


	
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

	
};

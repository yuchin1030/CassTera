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
	// ������
	APersonPlayerController(const FObjectInitializer & ObjectInitializer);

	FORCEINLINE UClass * GetPlayerPawnClass() {return MyPawnClass;}


	UPROPERTY()
	class UGameTimerWidget* gameTimerwidget;

	UPROPERTY()
	class UMainUI* mainUI;

protected:
	virtual void BeginPlay() override;
	


	
	// �ùٸ� �� Ŭ���� Ŭ���̾�Ʈ �� ��ȯ
	UFUNCTION(Server, Reliable)
	void ServerRPC_DefinePawnClass();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_DefinePawnClass();

	// �� ��Ʈ�ѷ��� ���� ������ �� Ŭ���� ����
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_SetPawn(TSubclassOf<APawn> InPawnClass);

	// ����Ϸ��� ���� �� Ŭ����
	UPROPERTY(Replicated)
	TSubclassOf<APawn> MyPawnClass;

	// ����� ù ��° �� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="MySettings")
	TSubclassOf<APawn> SeakPlayerPawn;

	// ����� �� ��° �� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="MySettings")
	TSubclassOf<APawn> HidePlayerPawn;

	// �������� �� ����
	UPROPERTY()
	int32 rate = 50;


public:
	//���� �÷��̾� �������� �ٲٱ�
	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeToSpectator();
	//���� �÷��̾� �������->���� ������ �ٲٱ�(����)
	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeToPlayer();
	//�ٲٱ� �����Լ�
	UFUNCTION()
	void ChangeToPlayer();

	class AHidePlayerCamera* spectator;
	class AHidePlayer* originPlayer;

	UPROPERTY()
	class APersonPlayerGameModeBase* gm; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HidePlayer")
	TSubclassOf<class AHidePlayerCamera> watcingCam_bp;
	UPROPERTY()
	class AHidePlayerCamera* watchingCam;

	UPROPERTY()
	class APawn* origin;

};

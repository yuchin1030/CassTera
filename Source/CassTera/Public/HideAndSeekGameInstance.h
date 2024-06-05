// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include <OnlineSessionSettings.h>
#include "HideAndSeekGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString roomName;

	UPROPERTY(BlueprintReadOnly)
	FString hostName;

	UPROPERTY(BlueprintReadOnly)
	FString userName;

	UPROPERTY(BlueprintReadOnly)
	int32 maxPlayerCount;

	UPROPERTY(BlueprintReadOnly)
	int32 currentPlayerCount;

	UPROPERTY(BlueprintReadOnly)
	int32 pingMs;

	int32 index;

	FORCEINLINE void Set(const FOnlineSessionSearchResult& item) {
		//index = _index;
		// ������ �̸�
		userName = item.Session.OwningUserName;
		// �ִ� �÷��̾� ��
		maxPlayerCount = item.Session.SessionSettings.NumPublicConnections;
		// ���� �濡 ���� �÷��̾� ��
		pingMs = item.PingInMs;
	}

	FORCEINLINE FString ToString() {
		return FString::Printf(TEXT("%s, %s, %s, (%d/%d), %dms"), *roomName, *hostName, *userName, currentPlayerCount, maxPlayerCount, pingMs);
	}
};

// ��ã�� ��û �� ������ ���� �� ȣ��� ��������Ʈ
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessioinSearchDelegate, const FSessionInfo&, info)

UCLASS()
class CASSTERA_API UHideAndSeekGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	// ���� �������̽� �����
	IOnlineSessionPtr sessionInterface;

	//FSessioinSearchDelegate OnMySessionSearchCompleteDelegate;

	//�� �����
	UFUNCTION()
	void CreateMySession(FString roomName, int32 playerCount);

	// �� ����������� Ȯ��
	UFUNCTION()
	void OnCreateSessionCompleted(FName sessionName, bool bWasSuccessful);

	FString mySessionName = TEXT("CassTera");

	// ���� ã�� �ʹ�
	TSharedPtr<FOnlineSessionSearch> sessionInSearch;
	void FindOtherSessions();

	// �� ã�� �Ϸ�
	void OnFindSessionsComplete(bool bWasSuccessful);
};

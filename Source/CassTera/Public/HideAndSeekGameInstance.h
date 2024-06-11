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

	FORCEINLINE void Set(int _index, const FOnlineSessionSearchResult& item) {
		index = _index;
		// 방장의 이름
		userName = item.Session.OwningUserName;
		// 최대 플레이어 수
		maxPlayerCount = item.Session.SessionSettings.NumPublicConnections;
		// 현재 방에 들어온 플레이어 수
		currentPlayerCount = maxPlayerCount - item.Session.NumOpenPublicConnections;
		pingMs = item.PingInMs;
	}

	FORCEINLINE FString ToString() {
		return FString::Printf(TEXT("%s, %s, %s, (%d/%d), %dms"), *roomName, *hostName, *userName, currentPlayerCount, maxPlayerCount, pingMs);
	}
};

// 방찾기 요청 후 응답이 왔을 때 호출될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessioinSearchDelegate, const FSessionInfo&, info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessionSearchFinishedDelegate, bool, bSearching);


UCLASS()
class CASSTERA_API UHideAndSeekGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	void Shutdown() override;

	bool Tick(float DeltaSeconds);

	FTSTicker::FDelegateHandle TickDelegateHandle;

	UPROPERTY()
	class ACassteraGameState* cgs;


public:
	// 세션 인터페이스 만들기
	IOnlineSessionPtr sessionInterface;

	FSessioinSearchDelegate OnMySessionSearchCompleteDelegate;

	FSessionSearchFinishedDelegate OnMySessionSearchFinishedDelegate;

	//방 만들기	
	void CreateMySession(FString roomName, int32 playerCount);

	// 방 만들어졌는지 확인
	void OnCreateSessionCompleted(FName sessionName, bool bWasSuccessful);

	FString mySessionName = TEXT("CassTera");

	// 방을 찾고 싶다
	TSharedPtr<FOnlineSessionSearch> sessionInSearch;
	void FindOtherSessions();

	// 방 찾기 완료
	void OnFindSessionsComplete(bool bWasSuccessful);

	// Join 방에 합류
	void JoinMySession(int32 index);

	// Join 완료
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result);

	// 방이름을 한글로 하기 위함
	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);

	bool IsInRoom();
};

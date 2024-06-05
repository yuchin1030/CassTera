// Fill out your copyright notice in the Description page of Project Settings.


#include "HideAndSeekGameInstance.h"
#include <OnlineSubsystem.h>
#include <Interfaces/OnlineSessionInterface.h>
#include <Online/OnlineSessionNames.h>

void UHideAndSeekGameInstance::Init()
{
	Super::Init();

	// 서브시스템에서 세션 인터페이스 가져오기
	auto subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessionInterface = subsys->GetSessionInterface();

		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UHideAndSeekGameInstance::OnCreateSessionCompleted);


	}

	/*FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, [&]() {
		CreateMySession(TEXT("School"), 3);
		}, 3.0f, false);*/
}	

void UHideAndSeekGameInstance::CreateMySession(FString roomName, int32 playerCount)
{
	FOnlineSessionSettings set;

	// 1. 데디케이트 서버 접속인가? false
	set.bIsDedicated = false;

	// 2. 랜선으로 매칭할 것인가? subsys 이름이 다르면 false
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	set.bIsLANMatch = subsysName == "NULL";

	// 3. 매칭을 공개할 것인가?( <-> 친구 초대로만 할 것인가?)
	set.bShouldAdvertise = true;

	// 4. Presesce를 사용할 것인가?
	set.bUsesPresence = true;

	// 5. 게임 도중에 참여가능 여부
	set.bAllowJoinInProgress = true;
	set.bAllowJoinViaPresence = true;

	// 6. 참여할 공개 연결의 최대 갯수
	set.NumPublicConnections = playerCount;

	// 7. 커스텀 정보 : 방 이름, 호스트 이름
	set.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	set.Set(FName("HOST_NAME"), mySessionName, EOnlineDataAdvertisementType::ViaOnlineService);

	// 8. netID를 가져 오고 싶다
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	
	UE_LOG(LogTemp, Warning, TEXT("CreateMySession : %s"), *roomName);

	// 인터페이스로 방 만들기 요청
	sessionInterface->CreateSession(*netID,FName(*roomName), set);
}

void UHideAndSeekGameInstance::OnCreateSessionCompleted(FName sessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete -> sessionName : %s, bwasSuccessful : %d"), *sessionName.ToString(), bWasSuccessful);
		 
	// 방생성 성공을 했다면, 대기방으로 ServerTravel하고 싶다
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(TEXT("/Game/Yohan/Maps/WaitngMap?listen"));		
	}
}

void UHideAndSeekGameInstance::FindOtherSessions()
{
	// 세션 인터페이스를 이용해서 방을 찾고 싶다
	sessionInSearch = MakeShareable(new FOnlineSessionSearch);
	
	// 검색 조건을 설정하고 싶다
	sessionInSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// LAN 여부
	sessionInSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	// 최대로 검색할 수 있는 세션 갯수
	sessionInSearch->MaxSearchResults = 10;

	sessionInterface->FindSessions(0, sessionInSearch.ToSharedRef());
}

void UHideAndSeekGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	// 방 찾기에 성공했다면
	if (bWasSuccessful)
	{
		// sessionInSearch에서 정보를 가져오고 싶다. -> UI로 표현하고 싶다
		auto results = sessionInSearch->SearchResults;
		if (results.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete results.Num() == 0"));
		}
		for (int i = 0; i < results.Num(); i++)
		{
			auto item = results[i];
			if (false == item.IsValid())
			{
				continue;
			}
			
			FSessionInfo info;
			info.Set(item);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *info.ToString());

		}
	}
}

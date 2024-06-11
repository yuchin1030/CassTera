	// Fill out your copyright notice in the Description page of Project Settings.


#include "HideAndSeekGameInstance.h"
#include <OnlineSubsystem.h>
#include <Interfaces/OnlineSessionInterface.h>
#include <Online/OnlineSessionNames.h>
#include "CassteraGameState.h"
#include <../../../../../../../Source/Runtime/Core/Public/Containers/Ticker.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>



void UHideAndSeekGameInstance::Init()
{
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UHideAndSeekGameInstance::Tick));

	Super::Init();

	

	// 서브시스템에서 세션 인터페이스 가져오기
	auto subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessionInterface = subsys->GetSessionInterface();

		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UHideAndSeekGameInstance::OnCreateSessionCompleted);

		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UHideAndSeekGameInstance::OnFindSessionsComplete);

		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UHideAndSeekGameInstance::OnJoinSessionComplete);
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
	set.Set(FName("ROOM_NAME"), StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	set.Set(FName("HOST_NAME"), StringBase64Encode(mySessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

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

		// 대기 방 입장 후 30초 뒤에 교실 맵으로 ServerTravel하고 싶다
		FTimerHandle timer;
		GetWorld()->GetTimerManager().SetTimer(timer, [&]() {
			GetWorld()->ServerTravel(TEXT("/Game/Yohan/Maps/SchoolMap?listen"));
			UE_LOG(LogTemp, Error, TEXT("SchoolMap Open"));
			}, 30, false);

		
	}
}

bool UHideAndSeekGameInstance::IsInRoom()
{
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	return sessionInterface->IsPlayerInSession(FName(*mySessionName), *netID);
}

void UHideAndSeekGameInstance::FindOtherSessions()
{
	if (OnMySessionSearchFinishedDelegate.IsBound())
	{
		OnMySessionSearchFinishedDelegate.Broadcast(true);
	}

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
			info.Set(i, item);

			FString roomName;
			FString hostName;
			item.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
			item.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);

			info.roomName = StringBase64Decode(roomName);
			info.hostName = StringBase64Decode(hostName);

			OnMySessionSearchCompleteDelegate.Broadcast(info);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *info.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete bWasSuccessful is false"));
		OnMySessionSearchFinishedDelegate.Broadcast(false);
	}
}

void UHideAndSeekGameInstance::JoinMySession(int32 index)
{
	sessionInterface->JoinSession(0, FName(*mySessionName), sessionInSearch->SearchResults[index]);
}

void UHideAndSeekGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result)
{
	// 호스트의 IP, Port번호를 가져와서 ClientTravel 하고싶다.
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		auto* pc = GetWorld()->GetFirstPlayerController();
		FString url;
		sessionInterface->GetResolvedConnectString(SessionName, url);
		UE_LOG(LogTemp, Warning, TEXT("ClientTravel URL : %s"), *url);

		if (false == url.IsEmpty())
		{
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
	}
}

FString UHideAndSeekGameInstance::StringBase64Encode(const FString& str)
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UHideAndSeekGameInstance::StringBase64Decode(const FString& str)
{
	// Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}

bool UHideAndSeekGameInstance::Tick(float DeltaSeconds)
{
	cgs = Cast<ACassteraGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (cgs)
	{
		// 교실 스테이트에서 대기방으로 서버 트레블을 했다면
		if (cgs->bIsTraveld == true)
		{
			cgs->bIsTraveld = false;
			FTimerHandle startGame;
			GetWorld()->GetTimerManager().SetTimer(startGame, [&]() {
				GetWorld()->ServerTravel(TEXT("/Game/Yohan/Maps/SchoolMap?listen"));
				}, 30, false);
		}
	}
	//FDelegateHandle TickDelegateHandle;
	//auto* pc = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	//cgs = Cast<ACassteraGameState>(UGameplayStatics::GetGameState(GetWorld()));
	//if (cgs) 
	//{
	//	if (cgs->countDown <= 0)
	//	{
	//		cgs->countDown = 10;
	//		FTimerHandle goHandle;
	//		if (pc->IsLocalController())
	//		{
	//			GetWorld()->GetTimerManager().SetTimer(goHandle, [&]() {
	//				//자꾸터져서 주석처리
	//				cgs->bCount = true;
	//				GetWorld()->GetTimerManager().ClearTimer(cgs->countHandle);
	//				GetWorld()->GetTimerManager().ClearTimer(cgs->timerHandler);
	//				GetWorld()->ServerTravel(TEXT("/Game/Yohan/Maps/WaitngMap?listen"));

	//				}, 10.f, false);
	//		}
	//		FTimerHandle startGame;
	//		GetWorld()->GetTimerManager().SetTimer(startGame, [&]() {
	//			GetWorld()->ServerTravel(TEXT("/Game/Yohan/Maps/SchoolMap?listen"));
	//			}, 30, false);
	//	}
	//}
	
	return true;
}


void UHideAndSeekGameInstance::Shutdown()
{
	// Unregister ticker delegate
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);

	Super::Shutdown();
}

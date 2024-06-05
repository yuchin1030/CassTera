// Fill out your copyright notice in the Description page of Project Settings.


#include "HideAndSeekGameInstance.h"
#include <OnlineSubsystem.h>
#include <Interfaces/OnlineSessionInterface.h>
#include <Online/OnlineSessionNames.h>

void UHideAndSeekGameInstance::Init()
{
	Super::Init();

	// ����ý��ۿ��� ���� �������̽� ��������
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

	// 1. ��������Ʈ ���� �����ΰ�? false
	set.bIsDedicated = false;

	// 2. �������� ��Ī�� ���ΰ�? subsys �̸��� �ٸ��� false
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	set.bIsLANMatch = subsysName == "NULL";

	// 3. ��Ī�� ������ ���ΰ�?( <-> ģ�� �ʴ�θ� �� ���ΰ�?)
	set.bShouldAdvertise = true;

	// 4. Presesce�� ����� ���ΰ�?
	set.bUsesPresence = true;

	// 5. ���� ���߿� �������� ����
	set.bAllowJoinInProgress = true;
	set.bAllowJoinViaPresence = true;

	// 6. ������ ���� ������ �ִ� ����
	set.NumPublicConnections = playerCount;

	// 7. Ŀ���� ���� : �� �̸�, ȣ��Ʈ �̸�
	set.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	set.Set(FName("HOST_NAME"), mySessionName, EOnlineDataAdvertisementType::ViaOnlineService);

	// 8. netID�� ���� ���� �ʹ�
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	
	UE_LOG(LogTemp, Warning, TEXT("CreateMySession : %s"), *roomName);

	// �������̽��� �� ����� ��û
	sessionInterface->CreateSession(*netID,FName(*roomName), set);
}

void UHideAndSeekGameInstance::OnCreateSessionCompleted(FName sessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete -> sessionName : %s, bwasSuccessful : %d"), *sessionName.ToString(), bWasSuccessful);
		 
	// ����� ������ �ߴٸ�, �������� ServerTravel�ϰ� �ʹ�
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(TEXT("/Game/Yohan/Maps/WaitngMap?listen"));		
	}
}

void UHideAndSeekGameInstance::FindOtherSessions()
{
	// ���� �������̽��� �̿��ؼ� ���� ã�� �ʹ�
	sessionInSearch = MakeShareable(new FOnlineSessionSearch);
	
	// �˻� ������ �����ϰ� �ʹ�
	sessionInSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// LAN ����
	sessionInSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	// �ִ�� �˻��� �� �ִ� ���� ����
	sessionInSearch->MaxSearchResults = 10;

	sessionInterface->FindSessions(0, sessionInSearch.ToSharedRef());
}

void UHideAndSeekGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	// �� ã�⿡ �����ߴٸ�
	if (bWasSuccessful)
	{
		// sessionInSearch���� ������ �������� �ʹ�. -> UI�� ǥ���ϰ� �ʹ�
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

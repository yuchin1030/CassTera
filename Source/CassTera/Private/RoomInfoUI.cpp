// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomInfoUI.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "HideAndSeekGameInstance.h"

void URoomInfoUI::NativeConstruct()
{
	Button_Join->OnClicked.AddDynamic(this, &URoomInfoUI::OnMyClickJoinRoom);
}

void URoomInfoUI::OnMyClickJoinRoom()
{
	// gi의 joingRoom 함수 호출
	Cast<UHideAndSeekGameInstance>(GetWorld()->GetGameInstance())->JoinMySession(index);
}

void URoomInfoUI::Setup(const struct FSessionInfo& info)
{
	index = info.index;
	
	// 방정보를 이용해서 UI를 구성하고싶다.
	Text_RoomName->SetText(FText::FromString(info.roomName));
	Text_HostName->SetText(FText::FromString(info.hostName));
	Text_UserName->SetText(FText::GetEmpty());

	FString count = FString::Printf(TEXT("%d/%d"), info.currentPlayerCount, info.maxPlayerCount);
	Text_PlayerCount->SetText(FText::FromString(count));

	FString ping = FString::Printf(TEXT("%dms"), info.pingMs);
	Text_PingMS->SetText(FText::FromString(ping));
	
}

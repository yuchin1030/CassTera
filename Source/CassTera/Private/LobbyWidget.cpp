// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "HideAndSeekGameInstance.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Slider.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/EditableText.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetSwitcher.h>
#include "RoomInfoUI.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ScrollBox.h>

void ULobbyWidget::NativeConstruct()
{
	// 게임 인스턴스 채우기
	gi = Cast<UHideAndSeekGameInstance>(GetWorld()->GetGameInstance());

	// gi의 OnMySessionSearchCompleteDelegate에 AddRoomInfoUI를 연결하고싶다.
	gi->OnMySessionSearchCompleteDelegate.AddDynamic(this, &ULobbyWidget::AddRoomInfoUI);

	// gi의 OnMySessionSearchFinishedDelegate에 OnMySetActiveFindingText를 연결한다
	gi->OnMySessionSearchFinishedDelegate.AddDynamic(this, &ULobbyWidget::OnMySetActiveFindingText);

	// 버튼 연결
	Button_CreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickCreateRoom);
	Slider_PlayerCount->SetValue(FCString::Atof(*Text_PlayerCounts->GetText().ToString()));

	Button_GoCreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoCreateRoom);
	Button_GoFindRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoFindRoom);

	Button_Menu->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu);
	Button_OutMenu->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu);

	Button_Find->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickFindRoom);

	// 슬라이더 연결
	Slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULobbyWidget::OnMyValueChanged);

	// 최초 시작시 메뉴위젯 활성화
	SwitcherUI->SetActiveWidgetIndex(0);
}

void ULobbyWidget::OnMyClickCreateRoom()
{
	// 게임 인스턴스가 있다면
	if (gi)
	{
		// 플레이어의 숫자를 받아온다
		int32 count = Slider_PlayerCount->GetValue();
		// 받아온 플레이어의 숫자대로
		gi->CreateMySession(Edit_RoomName->GetText().ToString(), count);

	}
}

void ULobbyWidget::OnMyValueChanged(float value)
{
	Text_PlayerCounts->SetText(FText::AsNumber(value));
}

void ULobbyWidget::OnMyClickGoMenu()
{
	SwitcherUI->SetActiveWidgetIndex(0);
}
void ULobbyWidget::OnMyClickGoCreateRoom()
{	
	//닉네임 설정
	SetSessionName();
		
	SwitcherUI->SetActiveWidgetIndex(1);
}
void ULobbyWidget::OnMyClickGoFindRoom()
{
	// 닉네임 설정
	SetSessionName();

	SwitcherUI->SetActiveWidgetIndex(2);

	// 방 찾기 메뉴 진입시 방 찾기 시도
	if (gi)
	{
		gi->FindOtherSessions();
	}
}

void ULobbyWidget::OnMyClickFindRoom()
{
	// 기존 목록 삭제
	ScrollBox_RoomList->ClearChildren();
	
	// gi의 FindOtherSession 호출
	gi->FindOtherSessions();
}

void ULobbyWidget::AddRoomInfoUI(const struct FSessionInfo& info)
{
	// RoomInfoUI_BP를 이용해 위젯 만들기
	auto ui = CreateWidget<URoomInfoUI>(this, roomInfoUI_BP);
	// info를 Setup함수를 통해 전달
	ui->Setup(info);
	// 생성한 위젯을 scrollBox_RoomList에 붙이기
	ScrollBox_RoomList->AddChild(ui);
}

void ULobbyWidget::OnMySetActiveFindingText(bool bSearching)
{
	Text_Finding->SetVisibility(bSearching ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	Button_Find->SetIsEnabled(bSearching);
}

void ULobbyWidget::SetSessionName()
{
	if (gi)
	{
		// 인스턴스에서 MySessionName(닉네임)을 가져와서 Edit_SessionName에 설정한다.
		gi->mySessionName = Edit_SessionName->GetText().ToString();
	}
}

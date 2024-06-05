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
	// ���� �ν��Ͻ� ä���
	gi = Cast<UHideAndSeekGameInstance>(GetWorld()->GetGameInstance());

	// ��ư ����
	Button_CreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickCreateRoom);
	Slider_PlayerCount->SetValue(FCString::Atof(*Text_PlayerCounts->GetText().ToString()));

	Button_GoCreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoCreateRoom);
	Button_GoFindRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoFindRoom);

	Button_Menu->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu);
	Button_OutMenu->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu);

	Button_Find->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickFindRoom);

	// �����̴� ����
	Slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULobbyWidget::OnMyValueChanged);

	// ���� ���۽� �޴����� Ȱ��ȭ
	SwitcherUI->SetActiveWidgetIndex(0);
}

void ULobbyWidget::OnMyClickCreateRoom()
{
	// ���� �ν��Ͻ��� �ִٸ�
	if (gi)
	{
		// �÷��̾��� ���ڸ� �޾ƿ´�
		int32 count = Slider_PlayerCount->GetValue();
		// �޾ƿ� �÷��̾��� ���ڴ��
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
	SwitcherUI->SetActiveWidgetIndex(1);
}

void ULobbyWidget::OnMyClickGoFindRoom()
{
	SwitcherUI->SetActiveWidgetIndex(2);
}

void ULobbyWidget::OnMyClickFindRoom()
{

}

void ULobbyWidget::AddRoomInfoUI(const struct FSessionInfo& info)
{
	// RoomInfoUI_BP�� �̿��� ���� �����
	auto ui = CreateWidget<URoomInfoUI>(this, roomInfoUI_BP);
	// info�� Setup�Լ��� ���� ����
	ui->Setup(info);
	// ������ ������ scrollBox_RoomList�� ���̱�
	ScrollBox_RoomList->AddChild(ui);
}

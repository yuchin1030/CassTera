// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/EditableText.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ScrollBox.h>
#include "PersonPlayerController.h"
#include "CassTeraCharacter.h"
#include "ChatListWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "HidePlayer.h"

void UChatWidget::NativeConstruct()
{
	//ButtonSendMsg->OnClicked.AddDynamic(this, &UChatWidget::OnMyClickSendMsg);
}

void UChatWidget::OnMyClickSendMsg()
{
	FString msg = EditMessage->GetText().ToString();
	if (msg.IsEmpty())
	{
		return;
	}
	EditMessage->SetText(FText::GetEmpty());
	
	auto* pc = Cast<APersonPlayerController>(GetWorld()->GetFirstPlayerController());
	if (pc)
	{
		auto* seaker = Cast<ACassTeraCharacter>(pc->GetPawn());
		if (seaker)
		{
			seaker->ServerRPC_SendMsg(msg);
		}
		else
		{
			auto* hider = Cast<AHidePlayer>(pc->GetPawn());
			if (hider)
			{
				hider->ServerRPC_SendMsg(msg);
			}
		}
	}
}

void UChatWidget::AddMsg(const FString& msg)
{
	// chatListUI를 채팅 리스트에 올리는 작업
	auto* chatList = CreateWidget<UChatListWidget>(this, chatListUI_BP);
	chatList->Text_Message->SetText(FText::FromString(msg));
	ScrollMessageList->AddChild(chatList);
	ScrollMessageList->ScrollToEnd();
}

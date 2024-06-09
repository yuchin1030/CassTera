// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomTimerWidget.h"
#include "WatinGameMode.h"
#include "Components/TextBlock.h"

void UWaitingRoomTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateCanTick();

	gm = Cast<AWatinGameMode>(GetWorld()->GetAuthGameMode());
}

void UWaitingRoomTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (gm)
	{
		// 게임 모드의 시작 타이머를 받아와서, 초를 세팅한다
		FString secString = FString::FromInt((int)(gm->StartTimer) % 60);
		FText sec = FText::FromString(secString);
		Text_Seconds->SetText(sec);
	}
}

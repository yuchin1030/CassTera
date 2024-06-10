// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomTimerWidget.h"
#include "WatinGameMode.h"
#include "Components/TextBlock.h"
#include "WaitingRoomGameState.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void UWaitingRoomTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateCanTick();

	gm = Cast<AWatinGameMode>(GetWorld()->GetAuthGameMode());
	gs = Cast<AWaitingRoomGameState>(UGameplayStatics::GetGameState(GetWorld()));
}

void UWaitingRoomTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

void UWaitingRoomTimerWidget::TimerSet()
{
	if (gs)
	{
		// 게임 스테이트의 시작 타이머를 받아와서, 초를 세팅한다
		FString secString = FString::FromInt((int)(gs->StartTimer) % 60);
		FText sec = FText::FromString(secString);
		Text_Seconds->SetText(sec);
	}
}

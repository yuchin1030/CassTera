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
		// ���� ����� ���� Ÿ�̸Ӹ� �޾ƿͼ�, �ʸ� �����Ѵ�
		FString secString = FString::FromInt((int)(gm->StartTimer) % 60);
		FText sec = FText::FromString(secString);
		Text_Seconds->SetText(sec);
	}
}

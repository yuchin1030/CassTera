// Fill out your copyright notice in the Description page of Project Settings.


#include "WatinGameMode.h"
#include "WaitingRoomTimerWidget.h"

AWatinGameMode::AWatinGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWatinGameMode::BeginPlay()
{
	Super::BeginPlay();

	//// 타이머 ui를 생성하고
	//if (WaitingTimerUI_BP)
	//{
	//	WaitingTimerUI = Cast< UWaitingRoomTimerWidget>(CreateWidget(GetWorld(), WaitingTimerUI_BP));

	//	// 뷰포트에 붙힌다
	//	WaitingTimerUI->AddToViewport();
	//}
	
}

void AWatinGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 게임 시작 타이머를 돌린다
	/*StartTimer -= DeltaSeconds;*/
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomGameState.h"
#include "WaitingRoomTimerWidget.h"
#include "ChatWidget.h"
#include "Net/UnrealNetwork.h"

void AWaitingRoomGameState::BeginPlay()
{
	Super::BeginPlay();

	// 타이머 ui를 생성하고
	if (WaitingTimerUI_BP)
	{
		WaitingTimerUI = Cast< UWaitingRoomTimerWidget>(CreateWidget(GetWorld(), WaitingTimerUI_BP));

		// 뷰포트에 붙힌다
		WaitingTimerUI->AddToViewport();
	}
	
}	


void AWaitingRoomGameState::ServerRPC_SetTimer_Implementation(float DeltaSeconds)
{
	// 게임시작 타이머 돌리기
	float sec = StartTimer - DeltaSeconds;
	sec = FMath::Clamp(sec, 0, StartTimer);

	// 클라이언트와 시간 동기화
	MultiRPC_SetTimer(sec);
}

void AWaitingRoomGameState::MultiRPC_SetTimer_Implementation(float second)
{	
	StartTimer = second;
	if (WaitingTimerUI)
	{
		WaitingTimerUI->TimerSet();
		// 게임 스테이트에서 타이머 ui 생성 및 뷰포트 추가
		// 게임 스테이트에서 서버rpc로 시간 감소 기능 만들기
		// 시간 감소 서버RPC를 컨트롤러의 Tick에서 호출
		// 서버RPC에 멀티RPC를 후출
		// 멀티RPC에서 시간 동기화 및 타이머 UI의 숫자 반영
	}
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "CassteraGameState.h"
#include "GameTimerWidget.h"

void ACassteraGameState::BeginPlay()
{
	Super::BeginPlay();

	timerWidget = Cast<UGameTimerWidget>(CreateWidget(GetWorld(), WBP_gameTimerWidget));
	timerWidget->AddToViewport();
	UE_LOG(LogTemp, Warning, TEXT("start"));
}

void ACassteraGameState::ServerRPC_DecreaseTime_Implementation()
{
	if (!(minute == 0 && seconds <= 30))
	{
		pgPercent += (1.0f / totalSeconds * minusSeconds);
		
	}

	if (minute > 0 && seconds < 10)
	{
		minute -= 1;
		seconds += 51;
	}
	else if (minute == 0 && seconds < 10)
	{
		seconds = 0;
	}
	else if (!(minute == 0 && seconds <= 30))	// 버닝타임 제외하고는 모두 10초씩 차감
	{
		seconds -= minusSeconds;
	}

	ClientRPC_DecreaseTime(minute, seconds, minusSeconds, pgPercent, totalSeconds);
}

void ACassteraGameState::ClientRPC_DecreaseTime_Implementation(int32 _minute, int32 _seconds, int32 _minusSeconds, float _pgPercent, float _totalSeconds)
{
	minute = _minute;
	seconds = _seconds;
	minusSeconds = _minusSeconds;
	pgPercent = _pgPercent;
	totalSeconds = _totalSeconds;

	timerWidget->Timer();
}

void ACassteraGameState::ServerRPC_DecreaseHidePlayerCount_Implementation()
{
	
}

void ACassteraGameState::MultiRPC_DecreaseHidePlayerCount_Implementation(int32 _hidePlayerCount)
{
	
}

void ACassteraGameState::ServerRPC_CalculateTime_Implementation()
{
	if (bClearTimer == false)
	{
		bClearTimer = true;

		GetWorld()->GetTimerManager().SetTimer(timerHandler, [&]() {
			UE_LOG(LogTemp, Warning, TEXT("%d, %d"), minute, seconds);

			// 프로그래스바 타이머
			pgPercent += (1.0f / totalSeconds);
			// pg_Timer->SetPercent(pgPercent);

			// 숫자 타이머
			if (minute >= 0 && seconds > 0)
			{
				seconds -= 1;
			}
			else if (minute > 0 && seconds == 0)
			{
				minute -= 1;
				seconds = 59;
			}

			MultiRPC_CalculateTime(bClearTimer, minute, seconds, pgPercent, totalSeconds);
		}, 1.0f, true);
	}



	
}

void ACassteraGameState::MultiRPC_CalculateTime_Implementation(bool _bClearTimer, int32 _minute, int32 _seconds, float _pgPercent, float _totalSeconds)
{
	bClearTimer = _bClearTimer;
	minute = _minute;
	seconds = _seconds;
	pgPercent = _pgPercent;
	totalSeconds = _totalSeconds;

	if (timerWidget) {
	
		timerWidget->Timer();
		UE_LOG(LogTemp, Warning, TEXT("timer"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("timerNone"));
	}

	bClearTimer = false;
}

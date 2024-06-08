

#include "CassteraGameState.h"
#include "GameTimerWidget.h"
#include "ResultWidget.h"

void ACassteraGameState::BeginPlay()
{
	Super::BeginPlay();

	resultWidget = Cast<UResultWidget>(CreateWidget(GetWorld(), wbp_resultWidget));

	timerWidget = Cast<UGameTimerWidget>(CreateWidget(GetWorld(), WBP_gameTimerWidget));
	timerWidget->AddToViewport();
	UE_LOG(LogTemp, Error, TEXT("start"));
}

void ACassteraGameState::ServerRPC_HidePlayerCount_Implementation()
{
// 	hidePlayerCount = newHidePlayerCount;
	MultiRPC_HidePlayerCount(hidePlayerCount);
}

void ACassteraGameState::MultiRPC_HidePlayerCount_Implementation(int32 _hidePlayer)
{
	hidePlayerCount = _hidePlayer;
	if (timerWidget)
	{
		timerWidget->SetHidePlayerCount();
	}
}

void ACassteraGameState::ServerRPC_ShowResult_Implementation(bool bWin)
{	

	ClientRPC_ShowResult(bWin);
}

void ACassteraGameState::ClientRPC_ShowResult_Implementation(bool _bWin)
{
	resultWidget->ShowResult(_bWin);
}

void ACassteraGameState::ServerRPC_DecreaseHidePlayerCount_Implementation()
{
	hidePlayerCount -= 1;
	MultiRPC_DecreaseHidePlayerCount(hidePlayerCount);
	if (hidePlayerCount <= 0)
	{
		
	}
}

void ACassteraGameState::MultiRPC_DecreaseHidePlayerCount_Implementation(int32 _hidePlayer)
{
	hidePlayerCount = _hidePlayer;
	if (timerWidget)
	{
		timerWidget->SetHidePlayerCount();
	}

}

void ACassteraGameState::ServerRPC_DecreaseTime_Implementation()
{
	bDecreasing = true;
	UE_LOG(LogTemp, Warning, TEXT("%d"), bDecreasing);

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

	MultiRPC_DecreaseTime(bDecreasing, minute, seconds, minusSeconds, pgPercent, totalSeconds);
}

void ACassteraGameState::MultiRPC_DecreaseTime_Implementation(bool _bDecreasing, int32 _minute, int32 _seconds, int32 _minusSeconds, float _pgPercent, float _totalSeconds)
{
	bDecreasing = _bDecreasing;
	minute = _minute;
	seconds = _seconds;
	minusSeconds = _minusSeconds;
	pgPercent = _pgPercent;
	totalSeconds = _totalSeconds;

	timerWidget->Timer();

	bDecreasing = false;
	UE_LOG(LogTemp, Warning, TEXT("%d"), bDecreasing);

}

void ACassteraGameState::ServerRPC_CalculateTime_Implementation()
{
	if (bClearTimer == false)
	{
		bClearTimer = true;

		GetWorld()->GetTimerManager().SetTimer(timerHandler, [&]() {
			//UE_LOG(LogTemp, Warning, TEXT("%d, %d"), minute, seconds);

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
		//UE_LOG(LogTemp, Warning, TEXT("timer"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("timerNone"));
	}

	bClearTimer = false;
}



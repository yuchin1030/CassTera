// Fill out your copyright notice in the Description page of Project Settings.


#include "CassteraGameState.h"
#include "GameTimerWidget.h"

void ACassteraGameState::BeginPlay()
{
	Super::BeginPlay();

	timerWidget = Cast<UGameTimerWidget>(CreateWidget(GetWorld(), WBP_gameTimerWidget));
	timerWidget->AddToViewport();
	UE_LOG(LogTemp, Warning, TEXT("start"));

	//timerWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ACassteraGameState::Tick(float DeltaTime)
{
	//SetTimer();
}

void ACassteraGameState::SetTimer()
{
	//UE_LOG(LogTemp, Warning, TEXT("%d"), bClearTimer);

	if (bClearTimer == false)
	{
		bClearTimer = true;
		//UE_LOG(LogTemp, Warning, TEXT("%d"), bClearTimer);


		GetWorld()->GetTimerManager().SetTimer(timerHandler, this, &ACassteraGameState::ServerRPC_CalculateTime, 1.0f, true);
	}
}

void ACassteraGameState::ServerRPC_CalculateTime_Implementation()
{
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

	MultiRPC_CalculateTime();
	//txt_Minute->SetText(UKismetTextLibrary::Conv_IntToText(minute, false, true, 2, 2));
	//txt_Second->SetText(UKismetTextLibrary::Conv_IntToText(seconds, false, true, 2, 2));
	
}

void ACassteraGameState::MultiRPC_CalculateTime_Implementation()
{
	if (timerWidget)
	{
		//timerWidget->SetVisibility(ESlateVisibility::Visible);

		timerWidget->Timer();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("timerNone"));
	}

	bClearTimer = false;
}

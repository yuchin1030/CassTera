// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetTextLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "CassTeraCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Components/ProgressBar.h"
#include "Net/UnrealNetwork.h"

void UGameTimerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UGameTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetTimer();
}

void UGameTimerWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGameTimerWidget, seconds);
	DOREPLIFETIME(UGameTimerWidget, minute);
	DOREPLIFETIME(UGameTimerWidget, minusSeconds);
	DOREPLIFETIME(UGameTimerWidget, totalSeconds);
	DOREPLIFETIME(UGameTimerWidget, pgPercent);
}


void UGameTimerWidget::DecreaseTime()
{
	if (!(minute == 0 && seconds <= 30))
	{
		pgPercent += (1.0f / totalSeconds * minusSeconds);
		pg_Timer->SetPercent(pgPercent);
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
}

void UGameTimerWidget::Timer()
{
	// 프로그래스바 타이머
	pgPercent += (1.0f / totalSeconds);
	pg_Timer->SetPercent(pgPercent);

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

	txt_Minute->SetText(UKismetTextLibrary::Conv_IntToText(minute, false, true, 2, 2));
	txt_Second->SetText(UKismetTextLibrary::Conv_IntToText(seconds, false, true, 2, 2));

	//totalSeconds -= 1;

	//FString minute = FString::FromInt((int)(totalSeconds / 60));
	//FString seconds = FString::FromInt((int)(totalSeconds % 60));
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *minute);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *seconds);

	//txt_Minute->SetText(FText::FromString(minute));
	//txt_Second->SetText(FText::FromString(seconds));

	//GetWorld()->GetTimerManager().ClearTimer(timerHandler);

	bClearTimer = false;
}

void UGameTimerWidget::SetTimer()
{
	if (bClearTimer == false)
	{
		bClearTimer = true;
		GetWorld()->GetTimerManager().SetTimer(timerHandler, this, &UGameTimerWidget::ServerRPC_Timer, 1.0f, false);
	}
}

void UGameTimerWidget::ServerRPC_Timer_Implementation()
{
	MultiRPC_Timer();
}

void UGameTimerWidget::MultiRPC_Timer_Implementation()
{
	Timer();
}

void UGameTimerWidget::ServerRPC_DecreaseTime_Implementation()
{
	ClientRPC_DecreaseTime();
}

void UGameTimerWidget::ClientRPC_DecreaseTime_Implementation()
{
	DecreaseTime();
}

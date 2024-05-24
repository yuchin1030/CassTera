// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetTextLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "../CassTeraCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void UGameTimerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UGameTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bClearTimer == false)
	{
		bClearTimer = true;

		GetWorld()->GetTimerManager().SetTimer(timerHandler, this, &UGameTimerWidget::Timer, 1.0f, false);
	}
	
}

void UGameTimerWidget::DecreaseTime()
{
	if (minute > 0 && seconds < 10)
	{
		minute -= 1;
		seconds += 50;
	}
	else if (minute == 0 && seconds < 10)
	{
		seconds = 0;
	}
	else if (minute > 0 && seconds > 30)
	{
		seconds -= minusSeconds;
		//seconds = FMath::Clamp(seconds - minusSeconds, 0, 59);
	}
}

void UGameTimerWidget::Timer()
{
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

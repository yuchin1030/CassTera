// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimerWidget.h"

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
	else
	{
		seconds -= minusSeconds; 
	}
}

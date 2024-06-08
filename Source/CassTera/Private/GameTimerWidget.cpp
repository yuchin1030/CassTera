// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetTextLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "CassTeraCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Components/ProgressBar.h"
#include "Net/UnrealNetwork.h"
#include "CassteraGameState.h"
#include "PersonPlayerGameModeBase.h"
#include "PersonPlayerController.h"

void UGameTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gs = Cast<ACassteraGameState>(UGameplayStatics::GetGameState(GetWorld()));
	gm = Cast<APersonPlayerGameModeBase>(GetWorld()->GetAuthGameMode());
}

// void UGameTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
// {
// 
// }

//void UGameTimerWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(UGameTimerWidget, seconds);
//	DOREPLIFETIME(UGameTimerWidget, minute);
//	DOREPLIFETIME(UGameTimerWidget, minusSeconds);
//	DOREPLIFETIME(UGameTimerWidget, totalSeconds);
//	DOREPLIFETIME(UGameTimerWidget, pgPercent);
//}


void UGameTimerWidget::DecreaseTime()
{
	pg_Timer->SetPercent(pgPercent);
	//if (!(minute == 0 && seconds <= 30))
	//{
	//	pgPercent += (1.0f / totalSeconds * minusSeconds);
	//	pg_Timer->SetPercent(pgPercent);
	//}

	//if (minute > 0 && seconds < 10)
	//{
	//	minute -= 1;
	//	seconds += 51;
	//}
	//else if (minute == 0 && seconds < 10)
	//{
	//	seconds = 0;
	//}
	//else if (!(minute == 0 && seconds <= 30))	// 버닝타임 제외하고는 모두 10초씩 차감
	//{
	//	seconds -= minusSeconds;
	//}
}

void UGameTimerWidget::Timer()
{
	if (pg_Timer)
		pg_Timer->SetPercent(gs->pgPercent);

	if (txt_Minute)
	{
		txt_Minute->SetText(UKismetTextLibrary::Conv_IntToText(gs->minute, false, true, 2, 2));
	}
	if (txt_Second)
		txt_Second->SetText(UKismetTextLibrary::Conv_IntToText(gs->seconds, false, true, 2, 2));


}

void UGameTimerWidget::SetHidePlayerCount()
{
	if (txt_hidePlayerCount && gs)
	{
		txt_hidePlayerCount->SetText(UKismetTextLibrary::Conv_IntToText(gs->hidePlayerCount, false, true, 2, 2));
	}

}

//UE_LOG(LogTemp, Warning, TEXT("%d %d"), gs->minute, gs->seconds);

// 	if (txt_hidePlayerCount)
// 	{
// 		txt_hidePlayerCount->SetText(UKismetTextLibrary::Conv_IntToText(gm->hidePlayerCount, false, true, 2, 2));
// 	}

//void UGameTimerWidget::ServerRPC_DecreaseTime_Implementation()
//{
//	ClientRPC_DecreaseTime();
//}
//
//void UGameTimerWidget::ClientRPC_DecreaseTime_Implementation()
//{
//	DecreaseTime();
//}

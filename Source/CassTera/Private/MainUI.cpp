// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CassTeraCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetTextLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>

void UMainUI::NativeConstruct()
{
	img_RedCH->SetVisibility(ESlateVisibility::Hidden);
	img_Kill->SetVisibility(ESlateVisibility::Hidden);
	txt_Kill->SetVisibility(ESlateVisibility::Hidden);

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		playerChar = Cast<ACassTeraCharacter>(*ActorItr);
		if (playerChar) {
			break;
		}
	}
}

void UMainUI::ShowKillContent()
{
	img_Kill->SetVisibility(ESlateVisibility::Visible);
	txt_Kill->SetVisibility(ESlateVisibility::Visible);

	FTimerHandle visibleKillHandler;
	GetWorld()->GetTimerManager().SetTimer(visibleKillHandler, [&]() {

		img_Kill->SetVisibility(ESlateVisibility::Hidden);
		txt_Kill->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(visibleKillHandler);

	}, 1.0f, false);
}

void UMainUI::ShowGrenadeCount()
{
	txt_GrenadeCount->SetText(UKismetTextLibrary::Conv_IntToText(playerChar->grenadeCount, false, true));

}

void UMainUI::ShowStartUI()
{
	img_ShowFirst->SetVisibility(ESlateVisibility::Visible);
	txt_ShowFirst->SetVisibility(ESlateVisibility::Visible);
}

void UMainUI::HideStartUI()
{
	img_ShowFirst->SetVisibility(ESlateVisibility::Hidden);
	txt_ShowFirst->SetVisibility(ESlateVisibility::Hidden);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UMainUI::NativeConstruct()
{
	img_RedCH->SetVisibility(ESlateVisibility::Hidden);
	img_Kill->SetVisibility(ESlateVisibility::Hidden);
	txt_Kill->SetVisibility(ESlateVisibility::Hidden);

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

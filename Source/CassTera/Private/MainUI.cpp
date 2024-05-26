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

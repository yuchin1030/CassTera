// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings, meta = (BindWidget))
	class UImage* img_RedCH;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings, meta = (BindWidget))
	class UImage* img_Kill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings, meta = (BindWidget))
	class UTextBlock* txt_Kill;

	UFUNCTION()
	void ShowKillContent();

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API UGameTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings, meta =(BindWidget))
	class UTextBlock* txt_Minute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings, meta = (BindWidget))
	class UTextBlock* txt_Second;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings, meta = (BindWidget))
	class UProgressBar* pg_Timer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	int32 seconds = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	int32 minute = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	int minusSeconds = 9;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	float totalSeconds = 180.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	float pgPercent = 0;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	FTimerHandle timerHandler;

	bool bClearTimer = false;

	UFUNCTION()
	void DecreaseTime();

	UFUNCTION()
	void Timer();
};

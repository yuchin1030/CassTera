
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	virtual void NativeConstruct() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	class UTextBlock* text_Win;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	class UTextBlock* text_Lose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	class UTextBlock* text_CountDown;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	class UButton* btn_Quit;

	UPROPERTY()
	class ACassteraGameState* gs;

	UFUNCTION()
	void OnClickQuit();

	UFUNCTION()
	void ShowResult(bool bWin);

	UFUNCTION()
	void SetTimer();


};

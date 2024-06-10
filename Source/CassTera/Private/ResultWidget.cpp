

#include "ResultWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CassteraGameState.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/GameplayStatics.h"

void UResultWidget::NativeConstruct()
{

	gs = Cast<ACassteraGameState>(UGameplayStatics::GetGameState(GetWorld()));

	text_Win->SetVisibility(ESlateVisibility::Hidden);
	text_Lose->SetVisibility(ESlateVisibility::Hidden);

	btn_Quit->OnClicked.AddDynamic(this, &UResultWidget::OnClickQuit);
}

void UResultWidget::OnClickQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}


void UResultWidget::ShowResult(bool bWin)
{
	if (bWin)
	{
		text_Win->SetVisibility(ESlateVisibility::Visible);
		text_Lose->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		text_Win->SetVisibility(ESlateVisibility::Hidden);
		text_Lose->SetVisibility(ESlateVisibility::Visible);
	}
}

void UResultWidget::SetTimer()
{
	if (text_CountDown)
	{
		gs = Cast<ACassteraGameState>(UGameplayStatics::GetGameState(GetWorld()));

		text_CountDown->SetText(UKismetTextLibrary::Conv_IntToText(gs->countDown, false, true, 2, 2));
	}
}

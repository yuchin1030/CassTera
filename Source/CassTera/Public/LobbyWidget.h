// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UEditableText* Edit_RoomName;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class USlider* Slider_PlayerCount;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Text_PlayerCounts;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_CreateRoom;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UWidgetSwitcher* SwitcherUI;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Button_GoCreateRoom;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Button_GoFindRoom;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Button_Find;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Button_Menu;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Button_OutMenu;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UScrollBox* ScrollBox_RoomList;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Text_Finding;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UEditableText* Edit_SessionName;

	UPROPERTY()
	class UHideAndSeekGameInstance* gi;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URoomInfoUI> roomInfoUI_BP;	

	UFUNCTION()
	void OnMyClickCreateRoom();

	UFUNCTION()
	void OnMyValueChanged(float value);

	UFUNCTION()
	void OnMyClickGoCreateRoom();

	UFUNCTION()
	void OnMyClickGoFindRoom();

	UFUNCTION()
	void OnMyClickGoMenu();

	UFUNCTION()
	void OnMyClickFindRoom();

	UFUNCTION()
	void AddRoomInfoUI(const struct FSessionInfo& info);

	UFUNCTION()
	void OnMySetActiveFindingText(bool bSearching);

	UFUNCTION()
	void SetSessionName();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UScrollBox* ScrollMessageList;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UEditableText* EditMessage;

// 	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
// 	class UButton* ButtonSendMsg;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UChatListWidget> chatListUI_BP;

	// 채팅 보내기
	UFUNCTION()
	void OnMyClickSendMsg();

	// 채팅 받기
	void AddMsg(const FString& msg);
};

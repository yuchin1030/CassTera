// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CassTeraGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API UCassTeraGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_Timer();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Timer();


};

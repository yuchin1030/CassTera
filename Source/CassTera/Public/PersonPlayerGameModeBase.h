// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "PersonPlayerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API APersonPlayerGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	
};

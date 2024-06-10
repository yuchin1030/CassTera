// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomController.h"
#include "WaitingRoomGameState.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void AWaitingRoomController::BeginPlay()
{
	Super::BeginPlay();

	gs = Cast<AWaitingRoomGameState>(UGameplayStatics::GetGameState(GetWorld()));
}

void AWaitingRoomController::Tick(float DeltaTime)
{
	if (gs)
	{
		if (HasAuthority() && IsLocalController())
		{
			gs->ServerRPC_SetTimer(DeltaTime);
		}
	}
}

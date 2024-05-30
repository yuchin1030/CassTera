

#include "HidePlayerController.h"
#include "HidePlayer.h"
#include "HidePlayerCamera.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>

void AHidePlayerController::BeginPlay()
{
	
}

void AHidePlayerController::ServerRPC_ChangeToSpectator_Implementation(APawn* cam)
{
	spectator = Cast<AHidePlayerCamera>(cam);
	if (spectator != nullptr)
	{
		Possess(cam);
	}
}

void AHidePlayerController::ServerRPC_ChangeToPlayer_Implementation()
{
	for (TActorIterator<AHidePlayer> iter(GetWorld()); iter; ++iter)
	{
		auto* hideplayer = *iter;
		if (hideplayer->Controller == nullptr)
		{
			originPlayer = hideplayer;
			Possess(originPlayer);
			
		}
	}
}

void AHidePlayerController::ChangeToPlayer()
{
	ServerRPC_ChangeToPlayer();
}

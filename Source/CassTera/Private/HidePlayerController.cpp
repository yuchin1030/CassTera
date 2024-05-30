

#include "HidePlayerController.h"
#include "HidePlayer.h"
#include "HidePlayerCamera.h"

void AHidePlayerController::BeginPlay()
{
	
}

void AHidePlayerController::ServerRPC_ChangeToSpectator_Implementation(APawn* cam)
{
	AHidePlayerCamera* spectator = Cast<AHidePlayerCamera>(cam);
	if (spectator != nullptr)
	{
		Possess(cam);
	}
}

void AHidePlayerController::ServerRPC_ChangeToPlayer_Implementation()
{

}

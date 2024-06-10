
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HidePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API AHidePlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class APersonPlayerGameModeBase* gm;

public:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeToSpectator(APawn* cam);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeToPlayer();

	UFUNCTION()
	void ChangeToPlayer();

	class AHidePlayerCamera* spectator;
	class AHidePlayer* originPlayer;
};

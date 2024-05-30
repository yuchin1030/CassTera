
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HidePlayerCamera.generated.h"

UCLASS()
class CASSTERA_API AHidePlayerCamera : public APawn
{
	GENERATED_BODY()

public:
	AHidePlayerCamera();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UCameraComponent* camera;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UBoxComponent* rootComp;


	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Inputs")
	class UInputMappingContext* imc_hidingPlayer;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Inputs")
	class UInputAction* ia_move;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Inputs")
	class UInputAction* ia_look;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Inputs")
	class UInputAction* ia_changeCam;

	FVector MovementVector;
	FRotator deltaRotation;

	UFUNCTION()
	void OnIAMove(const FInputActionValue& value);
	UFUNCTION()
	void OnIALook(const FInputActionValue& value);
	UFUNCTION()
	void OnIAChangeCamera(const FInputActionValue& value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HidePlayer")
	TSubclassOf<class AHidePlayer> player_bp;
	class AHidePlayer* player;

	UPROPERTY()
	bool bDie = false;

	class AHidePlayerController* PlayerController;
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeCamera();
};

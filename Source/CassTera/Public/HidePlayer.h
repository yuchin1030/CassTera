
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HidePlayer.generated.h"

UCLASS()
class CASSTERA_API AHidePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AHidePlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UBoxComponent* rootComp;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UCameraComponent* camera;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class USpringArmComponent* cameraBoom;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UInputMappingContext* imc_hidingPlayer;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UInputAction* ia_move;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UInputAction* ia_look;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UInputAction* ia_jump;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UInputAction* ia_sounding;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UInputAction* ia_lockLocation;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UInputAction* ia_changeCam;

	FVector MovementVector;
	FRotator deltaRotation;

	UFUNCTION()
	void OnIAMove(const FInputActionValue& value);
	UFUNCTION()
	void OnIALook(const FInputActionValue& value);
	UFUNCTION()
	void OnIAJump(const FInputActionValue& value);
	UFUNCTION()
	void OnIASounding(const FInputActionValue& value);
	UFUNCTION()
	void OnIALockLocation(const FInputActionValue& value);
	UFUNCTION()
	void OnIAChangeCamera(const FInputActionValue& value);


};
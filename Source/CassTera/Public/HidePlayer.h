
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
	class UCameraComponent* camera;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class USpringArmComponent* cameraBoom;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Inputs")
	class UInputMappingContext* imc_hidingPlayer;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Inputs")
	class UInputAction* ia_move;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Inputs")
	class UInputAction* ia_look;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Inputs")
	class UInputAction* ia_jump;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Inputs")
	class UInputAction* ia_sounding;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Inputs")
	class UInputAction* ia_lockLocation;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Inputs")
	class UInputAction* ia_changeCam;

	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Sounds")
	class USoundBase* boop1;

	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|Sounds")
	class USoundBase* boop2;
		
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	int32 maxHP = 2;

	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer")
	int32 currentHP = maxHP;

	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|VFX")
	class UNiagaraSystem* hitVFX;
	UPROPERTY(EditDefaultsOnly, Category = "HidePlayer|VFX")
	class UNiagaraSystem* dieVFX;


	bool bDie;

	FVector MovementVector;
	FRotator deltaRotation;

	bool bLockLocation = false;
	FVector lockLoc;
	FRotator lockRot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HidePlayer|Meshs")
	TArray<UStaticMesh*> meshOptions;

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
	void UnLockLocation();
	UFUNCTION()
	void LockLocation();

	UFUNCTION()
	void OnIAChangeCamera(const FInputActionValue& value);
	UFUNCTION()
	void OnChangeCamera();
	UFUNCTION()
	void OnResetCamera();
	bool bChangeCam = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HidePlayer")
	TSubclassOf<class AHidePlayerCamera> watcingCam_bp;

	UPROPERTY()
	class AHidePlayerCamera* watchingCam;

	class APlayerController* PlayerController;


	UFUNCTION()
	void OnTakeDamage();

	UFUNCTION()
	void Die();

};
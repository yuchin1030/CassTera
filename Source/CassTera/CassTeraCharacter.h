// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CassTeraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACassTeraCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:

	ACassTeraCharacter();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	class UStaticMeshComponent* gun;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ia_fire;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ia_throw;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	UParticleSystem* fireVFX;

	UPROPERTY()
	class ACassTeraPlayerController* MyController;
	
	UPROPERTY()
	class UGameTimerWidget* gameTimerwidget;

	UPROPERTY()
	class UMainUI* mainUI;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	TSubclassOf<class UUserWidget> WBP_gameTimerWidget;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	TSubclassOf<class UUserWidget> WBP_mainUI;

	UFUNCTION()
	void AddMainUI();




	void Fire(const FInputActionValue& Value);
	void FireFin(const FInputActionValue& Value);

	void Throw(const FInputActionValue& Value);
	void ThrowFinish(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	TSubclassOf<class AGrenade> grenade_bp;
	
	UPROPERTY()
	class AGrenade* grenade;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UAnimMontage* throwMontage;

	UFUNCTION()
	void ShowKillUI();

	UFUNCTION()
	void NotEnemyResult();
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	bool bIsNotEnemy = false;

	bool bDecreasing;

	bool bMoving;

	bool bFiring;

	bool bThrowing;



	// ==========================================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_Fire();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Fire(FHitResult HitInfo, bool bFire);



	// ==========================================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_IMC();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_IMC();



	// ==========================================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_Throw();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Throw(bool _bThrowing);



	// ==========================================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_ThrowFin();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ThrowFin(bool _bThrowing);



protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void MoveFin(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick( float DeltaSeconds ) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};


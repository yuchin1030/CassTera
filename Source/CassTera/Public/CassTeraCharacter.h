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

	UCharacterMovementComponent* characterMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	class UStaticMeshComponent* gun;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ia_fire;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ia_throw;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ia_chat;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ia_chatEnter;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* gunShot;


	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* GameStartSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* winSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* loseSound;


	bool bChatEnabled;

	UFUNCTION()
	void OnIAChatEnter(const FInputActionValue& value);

	UFUNCTION()
	void OnIAChat(const FInputActionValue& value);

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

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	TSubclassOf<class AStartLocation> BP_startLocation;

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

	UPROPERTY()
	int32 grenadeCount = 4;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UAnimMontage* throwMontage;

	UFUNCTION()
	void ShowKillUI();

	UFUNCTION()
	void NotEnemyResult();

	UFUNCTION()
	void ChangePersonPlayerMovement();

	UPROPERTY()
	class AHidePlayer* enemyPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	bool bIsNotEnemy = false;

	//bool bDecreasing;

	bool bMoving;

	bool bFiring;

	bool bThrowing;

	bool bGameStart;

	UPROPERTY()
	class ACassteraGameState* gs;

	UPROPERTY(EditDefaultsOnly, Category = HidePlayer)
	TSubclassOf<class UResultWidget> wbp_resultWidget;

	UPROPERTY()
	class UResultWidget* resultWidget;


	UPROPERTY()
	bool bWin;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Lose();

	UFUNCTION(Client, Reliable)
	void ClientRPC_Lost();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Win();

	UFUNCTION(Client, Reliable)
	void ClientRPC_Win();


	UFUNCTION(Server, Reliable)
	void ServerRPC_DisableOutLiner();

	UFUNCTION(Client, Reliable)
	void ClientRPC_DisableOutLiner();

	UPROPERTY()
	AHidePlayer* hidePlayer;
	// ==========================================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_WorngShot();

	UFUNCTION(Client, Reliable)
	void ClientRPC_WorngShot();

	UFUNCTION(Server, Reliable)
	void ServerRPC_KillUI();

	UFUNCTION(Client, Reliable)
	void ClientRPC_KillUI();
	// ==========================================================================
	//UFUNCTION(Server, Reliable)
	//void ServerRPC_AddTimerUI();

	//UFUNCTION(Client, Reliable)
	//void ClientRPC_AddTimerUI();

	//UFUNCTION(Server, Reliable)
	//void ServerRPC_SetTimer();

	//UFUNCTION(NetMulticast, Reliable)
	//void MultiRPC_SetTimer();

	//==============================================================================


	UFUNCTION(Server,Reliable)
	void ServerRPC_AddMainUI();

	UFUNCTION(Client, Reliable)
	void ClientRPC_AddMainUI();



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
	void MultiRPC_Throw(bool _bThrowing, int32 _grenadeCount);



	// ==========================================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_ThrowFin();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ThrowFin(bool _bThrowing);

	// ==========================================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeMovement();

	UFUNCTION(NetMulticast, Reliable)
	void ClientRPC_ChangeMovement(bool _bGameStart);

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

	// Ã¤ÆÃ
	UPROPERTY(EditDefaultsOnly, Category = "MySetting")
	TSubclassOf<class UChatWidget> Chat_BP;

	UPROPERTY()
	class UChatWidget* chatUI;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SendMsg(const FString& msg);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SendMsg(const FString& msg);

};


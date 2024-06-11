
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
	
	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	bool ChangeCameraLock = false;


	UPROPERTY(ReplicatedUsing=OnRep_SetMesh)
	class UStaticMesh* newMesh;

	UFUNCTION()
	void OnRep_SetMesh();

	UPROPERTY(Replicated)
	FVector lockLoc;

	UPROPERTY(Replicated)
	FRotator lockRot;

	UPROPERTY()
	class ACassTeraCharacter* seekPlayer;

	//UFUNCTION(Server, Reliable)
	//void ServerRPC_WrongShot();

	//UFUNCTION(NetMulticast, Reliable)
	//void MultiRPC_WrongShot();

	UPROPERTY()
	bool bDie;

	FVector MovementVector;

	FVector localMoveDir;

	FRotator deltaRotation;

	bool bLockLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HidePlayer|Meshs")
	TArray<UStaticMesh*> meshOptions;

	UPROPERTY(Replicated)
	FVector meshLoc;

	UPROPERTY(Replicated)
	FRotator meshRot;

	UPROPERTY(Replicated)
	FVector MeshScale;

	UPROPERTY()
	class UGameTimerWidget* playerGameTimerwidget;

	UPROPERTY(EditDefaultsOnly, Category = HidePlayer)
	TSubclassOf<class UUserWidget> WBP_PlayergameTimerWidget;

	UPROPERTY(EditDefaultsOnly, Category = HidePlayer)
	TSubclassOf<class UResultWidget> wbp_resultWidget;

	UPROPERTY()
	class UResultWidget* resultWidget;



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

	class APersonPlayerController* PlayerController;

	UFUNCTION()
	void OnTakeDamage();

	UFUNCTION()
	void Die();

	UFUNCTION()
	void RandomMesh();

	UFUNCTION(Server, Reliable)
	void ServerRPC_LockLocation();
	UFUNCTION(Server, Reliable)
	void ServerRPC_UnLockLocation();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_LockLocation();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_UnLockLocation();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeCamera();
	UFUNCTION(NetMulticast, Reliable)
	void MulitRPC_ChangeCamera();
	UFUNCTION(Server, Reliable)
	void ServerRPC_ResetCamera();

	UFUNCTION(Server, Reliable)
	void ServerRPC_MakeIMC();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_MakeIMC();


	UFUNCTION(Server, Reliable)
	void ServerRPC_Die();

	UFUNCTION(Client, Reliable)
	void ClientRPC_Die();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Die();

	UPROPERTY()
	bool bWin;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Win();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Win();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Lose();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Lost();

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* winSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound");
	class USoundBase* loseSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* gameStartSound;

	 



	//UFUNCTION(Server, Reliable)
	//void ServerRPC_AttachUI();

	//UFUNCTION(Client, Reliable)
	//void ClientRPC_AttachUI();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Damaged();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Damaged(int32 _currentHP);
	UFUNCTION(Client, Reliable)
	void ClientRPC_Damaged(int32 _currentHP);

	/*UFUNCTION(Server, Reliable)
	void ServerRPC_SetTimer();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetTimer();*/


	// 채팅
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ia_chat;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ia_chatEnter;

	bool bChatEnabled;

	UFUNCTION()
	void OnIAChatEnter(const FInputActionValue& value);

	UFUNCTION()
	void OnIAChat(const FInputActionValue& value);

	// 채팅
	UPROPERTY(EditDefaultsOnly, Category = "MySetting")
	TSubclassOf<class UChatWidget> Chat_BP;

	UPROPERTY()
	class UChatWidget* chatUI;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SendMsg(const FString& msg);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SendMsg(const FString& msg);

	UFUNCTION(Server, Reliable)
	void ServerRPC_AddChatUI();

	UFUNCTION(Client, Reliable)
	void ClientRPC_AddChatUI();

	UFUNCTION()
	void AddChatUI();
};
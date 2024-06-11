

#include "HidePlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "HidePlayerCamera.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "PersonPlayerController.h"
#include "GameTimerWidget.h"
#include "CassTeraCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Materials/MaterialInstance.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Materials/Material.h>
#include "CassteraGameState.h"
#include "ResultWidget.h"
#include "Components/TextBlock.h"
#include "ChatWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"


AHidePlayer::AHidePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;


	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	GetCharacterMovement()->MaxAcceleration = 2050.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2050.f;
	GetCharacterMovement()->GroundFriction = 8.0f;
	JumpMaxCount = 2;

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->TargetArmLength = 400.f;
	cameraBoom->bUsePawnControlRotation = true;
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(cameraBoom);
	camera->bUsePawnControlRotation = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	bReplicates = true;

	// 랜덤 매시로 로드
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh0(TEXT("/Script/Engine.StaticMesh'/Game/Bohyun/Meshs/BlackBoard/BlackBoard.BlackBoard'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh1(TEXT("/Script/Engine.StaticMesh'/Game/Bohyun/Meshs/Chair.Chair'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh2(TEXT("/Script/Engine.StaticMesh'/Game/Bohyun/Meshs/Chalk.Chalk'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh3(TEXT("/Script/Engine.StaticMesh'/Game/Bohyun/Meshs/Desk.Desk'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh4(TEXT("/Script/Engine.StaticMesh'/Game/bohyun/Meshs/Bag.Bag'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh5(TEXT("/Script/Engine.StaticMesh'/Game/bohyun/Meshs/Trash.Trash'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh6(TEXT("/Script/Engine.StaticMesh'/Game/bohyun/Meshs/Cleaner.Cleaner'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh7(TEXT("/Script/Engine.StaticMesh'/Game/bohyun/Meshs/Paper.Paper'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh8(TEXT("/Script/Engine.StaticMesh'/Game/bohyun/Meshs/Pen.Pen'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh9(TEXT("/Script/Engine.StaticMesh'/Game/bohyun/Meshs/shibainu.shibainu'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh10(TEXT("/Script/Engine.StaticMesh'/Game/bohyun/Meshs/TableShelp.TableShelp'"));


	meshOptions.Add(mesh0.Object);
	meshOptions.Add(mesh1.Object);
	meshOptions.Add(mesh2.Object);
	meshOptions.Add(mesh3.Object);
	meshOptions.Add(mesh4.Object);
	meshOptions.Add(mesh5.Object);
	meshOptions.Add(mesh6.Object);
	meshOptions.Add(mesh7.Object);
	meshOptions.Add(mesh8.Object);
	meshOptions.Add(mesh9.Object);
	meshOptions.Add(mesh10.Object);


}

void AHidePlayer::BeginPlay()
{
	Super::BeginPlay();

	if (gameStartSound)
	{

	UGameplayStatics::PlaySound2D(GetWorld(), gameStartSound);
	}
	// 채팅 UI 추가
	if (IsLocallyControlled())
	{
		ServerRPC_AddChatUI();
	}

	if(HasAuthority())
	{ 
		RandomMesh();
	}
	FString myname = GetName();

	PlayerController = Cast<APersonPlayerController>(Controller);
	

	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// clearMappingContext를 쓰면 기존에 스폰되어 있던 캐릭터의 imc까지 날라가는듯..
			// RemoveMappingContext(내가 만든 imc) 를 쓰면 원하는 거 하나만 지워줌..
			// 혹시 내가 가지고 있는 imc가 남아있을 수 있으니, 안전하게 같은 이름을 가진 친구가 있다면 지우고, 새로 imc 추가한다.
			Subsystem->RemoveMappingContext(imc_hidingPlayer);
			Subsystem->AddMappingContext(imc_hidingPlayer, 0);
		}
	}	
}

void AHidePlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//ServerRPC_MakeIMC();
	
}

void AHidePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	localMoveDir = GetTransform().TransformVector(MovementVector);
	if (bLockLocation == true)
	{
		SetActorLocation(lockLoc, true);
		SetActorRotation(lockRot);
	}
	else
	{
		AddMovementInput(localMoveDir * 600, true);
		AddControllerYawInput(deltaRotation.Yaw);
		AddControllerPitchInput(deltaRotation.Pitch);
	}

}

void AHidePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ia_move, ETriggerEvent::Triggered, this, &AHidePlayer::OnIAMove);
		EnhancedInputComponent->BindAction(ia_move, ETriggerEvent::Completed, this, &AHidePlayer::OnIAMove);
		EnhancedInputComponent->BindAction(ia_look, ETriggerEvent::Triggered, this, &AHidePlayer::OnIALook);
		EnhancedInputComponent->BindAction(ia_look, ETriggerEvent::Completed , this, &AHidePlayer::OnIALook);
		EnhancedInputComponent->BindAction(ia_jump, ETriggerEvent::Started, this, &AHidePlayer::OnIAJump);
		EnhancedInputComponent->BindAction(ia_lockLocation, ETriggerEvent::Started, this, &AHidePlayer::OnIALockLocation);
		EnhancedInputComponent->BindAction(ia_sounding, ETriggerEvent::Started, this, &AHidePlayer::OnIASounding);
		EnhancedInputComponent->BindAction(ia_changeCam, ETriggerEvent::Started, this, &AHidePlayer::OnIAChangeCamera);
		
		// 채팅
		EnhancedInputComponent->BindAction(ia_chat, ETriggerEvent::Started, this, &AHidePlayer::OnIAChat);
		EnhancedInputComponent->BindAction(ia_chatEnter, ETriggerEvent::Started, this, &AHidePlayer::OnIAChatEnter);
	}
}

void AHidePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHidePlayer, newMesh);
	DOREPLIFETIME(AHidePlayer, meshLoc);
	DOREPLIFETIME(AHidePlayer, meshRot);
	DOREPLIFETIME(AHidePlayer, MeshScale);
	DOREPLIFETIME(AHidePlayer, lockLoc);
	DOREPLIFETIME(AHidePlayer, lockRot);
	//DOREPLIFETIME(AHidePlayer, bDie);

}

void AHidePlayer::OnRep_SetMesh()
{	
	meshComp->SetStaticMesh(newMesh);
	meshComp->SetRelativeLocationAndRotation(meshLoc,meshRot);
	meshComp->SetRelativeScale3D(MeshScale);
}

//void AHidePlayer::ServerRPC_WrongShot_Implementation()
//{
//	MultiRPC_WrongShot();
//}

//void AHidePlayer::MultiRPC_WrongShot_Implementation()
//{
//	//if (playerGameTimerwidget)
//	//{
//
//	//playerGameTimerwidget->ServerRPC_DecreaseTime();
//	//}
//}

void AHidePlayer::OnIAMove(const FInputActionValue& value)
{
	FVector2D moveDir = value.Get<FVector2D>();
	MovementVector = FVector(moveDir.Y, moveDir.X, 0);
}

void AHidePlayer::OnIALook(const FInputActionValue& value)
{
	FVector2D LookAxisVector = value.Get<FVector2D>();
	deltaRotation = FRotator(0,LookAxisVector.X, 0);

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHidePlayer::OnIAJump(const FInputActionValue& value)
{
	if (bLockLocation == false)
	{
		Jump();
	}
} 

void AHidePlayer::OnIASounding(const FInputActionValue& value)
{
	int32 random = FMath::RandRange(0, 1);
	if (random == 0)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), boop1, GetActorLocation());
	}
	if (random == 1)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), boop2, GetActorLocation());
	}
}

void AHidePlayer::OnIALockLocation(const FInputActionValue& value)
{
	if (bLockLocation)
	{
		ServerRPC_UnLockLocation();
	}
	else
	{
		ServerRPC_LockLocation();
	}
}

void AHidePlayer::UnLockLocation()
{
	if (!bLockLocation)
	{
		return;
	}
	bLockLocation = false;
}

void AHidePlayer::LockLocation()
{
	if (bLockLocation)
	{
		return;
	}

	bLockLocation = true;
	lockLoc = GetActorLocation();
	lockRot = GetActorRotation();
}

void AHidePlayer::OnIAChangeCamera(const FInputActionValue& value)
{
	if (ChangeCameraLock)
	{
		return;
	}
	ChangeCameraLock = true;

	FTimerHandle h1;
	GetWorldTimerManager().SetTimer(h1, [&]() {
		ChangeCameraLock = false;
	}, 0.2f, false);

	if (!bLockLocation)
	{
		return;
	}

	if (bChangeCam)
	{
		PlayerController->ServerRPC_ChangeToPlayer();
		bChangeCam = false;
	}
	else
	{
		PlayerController->ServerRPC_ChangeToSpectator(this, false);
		bChangeCam = true;
	}
	
}

void AHidePlayer::OnChangeCamera()
{
}

void AHidePlayer::OnResetCamera()
{
	if (!bChangeCam)
	{
		return;
	}
	bChangeCam = false;
}

void AHidePlayer::OnTakeDamage()
{
// 	currentHP = currentHP-1;

	//UE_LOG(LogTemp, Warning, TEXT("damage : %d"), currentHP);

	if (hitVFX != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitVFX, GetActorLocation());
	}
	if (currentHP <= 0)
	{
 	//	Die();
	}
}

void AHidePlayer::Die()
{
	//UE_LOG(LogTemp, Warning, TEXT("die : %d"), currentHP);
// 	PlayerController = Cast<APersonPlayerController>(Controller);
	ServerRPC_Die();
	/*if (dieVFX != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), dieVFX, GetActorLocation());
	}*/
	if (PlayerController)
	{
		bChangeCam = true;
		PlayerController->ServerRPC_ChangeToSpectator(this, true);
	}
}

void AHidePlayer::RandomMesh()
{
	//랜덤 매시 로딩 
	int32 random = FMath::RandRange(0, meshOptions.Num() - 1);
	if (random == 0)
	{
		meshComp->SetStaticMesh(meshOptions[0]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 0, -10), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(0.7f, 0.7f, 1));
		meshLoc = meshComp->GetRelativeLocation();
		meshRot = meshComp->GetRelativeRotation();
		MeshScale = meshComp->GetRelativeScale3D();
		newMesh = meshOptions[0];
	}
	if (random == 1)
	{
		meshComp->SetStaticMesh(meshOptions[1]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 0, -80), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(1.5f));
		meshLoc = meshComp->GetRelativeLocation();
		meshRot = meshComp->GetRelativeRotation();
		MeshScale = meshComp->GetRelativeScale3D();
		newMesh = meshOptions[1];
	}
	if (random == 2)
	{
		meshComp->SetStaticMesh(meshOptions[2]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 0, -80), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(5));
		meshLoc = meshComp->GetRelativeLocation();
		meshRot = meshComp->GetRelativeRotation();
		MeshScale = meshComp->GetRelativeScale3D();
		newMesh = meshOptions[2];
	}
	if (random == 3)
	{
		meshComp->SetStaticMesh(meshOptions[3]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 0, -80), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(1.5f));
		meshLoc = meshComp->GetRelativeLocation();
		meshRot = meshComp->GetRelativeRotation();
		MeshScale = meshComp->GetRelativeScale3D();
		newMesh = meshOptions[3];
	}
	if (random == 4)
	{
		meshComp->SetStaticMesh(meshOptions[4]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, -320, -80), FRotator(0, 180, 0));
		meshComp->SetRelativeScale3D(FVector(80));
		meshLoc = meshComp->GetRelativeLocation();
		meshRot = meshComp->GetRelativeRotation();
		MeshScale = meshComp->GetRelativeScale3D();
		newMesh = meshOptions[4];
	}
	if (random == 5)
	{
		meshComp->SetStaticMesh(meshOptions[5]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 0, -30), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(0.55f));
		meshLoc = meshComp->GetRelativeLocation();
		meshRot = meshComp->GetRelativeRotation();
		MeshScale = meshComp->GetRelativeScale3D();
		newMesh = meshOptions[5];
	}
	if (random == 6)
	{
		meshComp->SetStaticMesh(meshOptions[6]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 10, 20), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(1));
		meshLoc = meshComp->GetRelativeLocation();
		meshRot = meshComp->GetRelativeRotation();
		MeshScale = meshComp->GetRelativeScale3D();
		newMesh = meshOptions[6];
	}
	if (random == 7)
	{
		meshComp->SetStaticMesh(meshOptions[7]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 10, -80), FRotator(0, 200, 0));
		meshComp->SetRelativeScale3D(FVector(2));
		meshLoc = meshComp->GetRelativeLocation();
		meshRot = meshComp->GetRelativeRotation();
		MeshScale = meshComp->GetRelativeScale3D();
		newMesh = meshOptions[7];
	}
	if (random == 8)
	{
		meshComp->SetStaticMesh(meshOptions[8]);
		meshComp->SetRelativeLocationAndRotation(FVector(-30, -20, -88), FRotator(0, 180, 0));
		meshComp->SetRelativeScale3D(FVector(3));
		meshLoc = meshComp->GetRelativeLocation();
		meshRot = meshComp->GetRelativeRotation();
		MeshScale = meshComp->GetRelativeScale3D();
		newMesh = meshOptions[8];
	}
	if (random == 9)
	{
		meshComp->SetStaticMesh(meshOptions[9]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 0, -38), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(3));
		meshLoc = meshComp->GetRelativeLocation();
		meshRot = meshComp->GetRelativeRotation();
		MeshScale = meshComp->GetRelativeScale3D();
		newMesh = meshOptions[9];
	}
	if (random == 10)
	{
		meshComp->SetStaticMesh(meshOptions[10]);
		meshComp->SetRelativeLocationAndRotation(FVector(-80, -40, -368), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(2));
		meshLoc = meshComp->GetRelativeLocation();
		meshRot = meshComp->GetRelativeRotation();
		MeshScale = meshComp->GetRelativeScale3D();
		newMesh = meshOptions[10];
	}
}

void AHidePlayer::ServerRPC_LockLocation_Implementation()
{
	MultiRPC_LockLocation();
}

void AHidePlayer::ServerRPC_UnLockLocation_Implementation()
{
	MultiRPC_UnLockLocation();
}

void AHidePlayer::MultiRPC_LockLocation_Implementation()
{
	LockLocation();

}

void AHidePlayer::MultiRPC_UnLockLocation_Implementation()
{
	UnLockLocation();

}

void AHidePlayer::ServerRPC_ChangeCamera_Implementation()
{
	MulitRPC_ChangeCamera();
}

void AHidePlayer::MulitRPC_ChangeCamera_Implementation()
{
	OnChangeCamera();
}

void AHidePlayer::ServerRPC_ResetCamera_Implementation()
{
	OnResetCamera();
}

void AHidePlayer::ServerRPC_MakeIMC_Implementation()
{
	MultiRPC_MakeIMC();
}

void AHidePlayer::MultiRPC_MakeIMC_Implementation()
{
	if (IsLocallyControlled())
	{
		if (PlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(imc_hidingPlayer, 0);
			}
		}
	}
}

// void AHidePlayer::ServerRPC_AttachUI_Implementation()
// {
// // 	ClientRPC_AttachUI();
// }
// 
// void AHidePlayer::ClientRPC_AttachUI_Implementation()
// {
// // 	if (IsLocallyControlled())
// // {
// // 		auto* pc = Cast<APersonPlayerController>(Controller);
// // 	//	playerGameTimerwidget = Cast<UGameTimerWidget>(CreateWidget(GetWorld(), WBP_PlayergameTimerWidget));
// // 		if (pc->gameTimerwidget)
// // 		{
// // 			playerGameTimerwidget = pc->gameTimerwidget;
// // 			playerGameTimerwidget->AddToViewport();
// // 			//ServerRPC_SetTimer();
// // 		}
// // 	}
// }


void AHidePlayer::ServerRPC_Damaged_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("SERVER_DMG"));

	currentHP = currentHP - 1;

	if (currentHP <= 0)
	{
		bDie = true;
		ServerRPC_Die();
	}

	/*ClientRPC_Damaged(currentHP);*/
	MultiRPC_Damaged(currentHP);
}

void AHidePlayer::MultiRPC_Damaged_Implementation(int32 _currentHP)
{
// 	UE_LOG(LogTemp, Warning, TEXT("MULTI_DMG"));
// 
// 	//OnTakeDamage();
// 	UE_LOG(LogTemp, Warning, TEXT("Client_DMG"));

	currentHP = _currentHP;

	if (hitVFX != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitVFX, GetActorLocation());
	}

	if (currentHP <= 0)
	{
		bDie = true;
		if (dieVFX != nullptr)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), dieVFX, GetActorLocation());
		}
		//Die();
		// 클라이언트는 죽었을 때 카메라액터 스폰이랑 Destroy 자체를 안하고 있어서 켜둠
		Destroy();
	}
}

void AHidePlayer::ClientRPC_Damaged_Implementation(int32 _currentHP)
{

}

void AHidePlayer::ServerRPC_Die_Implementation()
{
	ACassteraGameState* gs = Cast<ACassteraGameState>(GetWorld()->GetGameState());
	if (gs)
	{
		gs->ServerRPC_DecreaseHidePlayerCount();
	}

	PlayerController = Cast<APersonPlayerController>(Controller);

	if (PlayerController)
	{
		bChangeCam = true;
		PlayerController->ServerRPC_ChangeToSpectator(this);
	}
}

void AHidePlayer::ClientRPC_Die_Implementation()
{


}

void AHidePlayer::MultiRPC_Die_Implementation()
{
	if (dieVFX != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), dieVFX, GetActorLocation());
	}
	//Die();
	// 클라이언트는 죽었을 때 카메라액터 스폰이랑 Destroy 자체를 안하고 있어서 켜둠
	Destroy();
}

void AHidePlayer::ServerRPC_Win_Implementation()
{

	MultiRPC_Win();
	
}

void AHidePlayer::MultiRPC_Win_Implementation()
{
	if (IsLocallyControlled())
	{
		if (winSound)
		{
		UGameplayStatics::PlaySound2D(GetWorld(), winSound);

		}
	ACassteraGameState* gs = Cast<ACassteraGameState>(GetWorld()->GetGameState());
	if (gs)
	{
		resultWidget = Cast<UResultWidget>(CreateWidget(GetWorld(), wbp_resultWidget));
		resultWidget = gs->resultWidget;
		resultWidget->AddToViewport();
		resultWidget->text_Win->SetVisibility(ESlateVisibility::Visible);
		resultWidget->text_Lose->SetVisibility(ESlateVisibility::Hidden);
		bWin = true;
		PlayerController = Cast<APersonPlayerController>(Controller);
		if (PlayerController)
		{
			PlayerController->SetShowMouseCursor(true);
		}
// 		ServerRPC_CountDown();
//		gs->ServerRPC_ShowResult(bWin);
	}
	}
}

void AHidePlayer::ServerRPC_Lose_Implementation()
{
	MultiRPC_Lost();
}

void AHidePlayer::MultiRPC_Lost_Implementation()
{
	if (IsLocallyControlled())
	{
		if (loseSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), loseSound);

		}
		ACassteraGameState* gs = Cast<ACassteraGameState>(GetWorld()->GetGameState());
		if (gs)
		{
			resultWidget = Cast<UResultWidget>(CreateWidget(GetWorld(), wbp_resultWidget));
			resultWidget = gs->resultWidget;
			resultWidget->AddToViewport();
			resultWidget->text_Win->SetVisibility(ESlateVisibility::Hidden);
			resultWidget->text_Lose->SetVisibility(ESlateVisibility::Visible);
			bWin = false;
			PlayerController = Cast<APersonPlayerController>(Controller);
			if (PlayerController)
			{
				PlayerController->SetShowMouseCursor(true);
			}
			// 			ServerRPC_CountDown();
				//		gs->ServerRPC_ShowResult(bWin);

		}
	}
}

//void AHidePlayer::ServerRPC_SetTimer_Implementation()
//{
//	MultiRPC_SetTimer();
//}
//
//void AHidePlayer::MultiRPC_SetTimer_Implementation()
//{
//
//	currentHP = maxHP;
//	if (playerGameTimerwidget)
//	{
//		playerGameTimerwidget->SetTimer();
//	}
//}
// 채팅
void AHidePlayer::ServerRPC_SendMsg_Implementation(const FString& msg)
{
	MultiRPC_SendMsg(msg);
}

void AHidePlayer::MultiRPC_SendMsg_Implementation(const FString& msg)
{
	// 로컬 플레이어 컨트롤러를 가져와서 chatUI의 addMsg호출
	auto* pc = Cast<APersonPlayerController>(GetWorld()->GetFirstPlayerController());
	if (pc)
	{
		pc->chatUI->AddMsg(msg);
	}
}

void AHidePlayer::ServerRPC_AddChatUI_Implementation()
{
	ClientRPC_AddChatUI();
}



void AHidePlayer::ClientRPC_AddChatUI_Implementation()
{
	AddChatUI();
}

void AHidePlayer::AddChatUI()
{
	// 내가 조작하는 주인공만 UI를 생성하고싶다.
	//MyController = Cast<ACassTeraPlayerController>(Controller);
	auto* pc = Cast<APersonPlayerController>(Controller);
	
	// 채팅 추가
	if (nullptr == pc->chatUI)
	{
		pc->chatUI = Cast<UChatWidget>(CreateWidget(GetWorld(), Chat_BP));
		pc->chatUI->AddToViewport();
	}
	chatUI = pc->chatUI;
}

void AHidePlayer::OnIAChatEnter(const FInputActionValue& value)
{
	auto* pc = Cast<APersonPlayerController>(GetWorld()->GetFirstPlayerController());
	//if(pc)
	//pc->chatUI->OnMyClickSendMsg();
}

// 채팅 IA 키 눌렸을 때 실행될 기능
void AHidePlayer::OnIAChat(const FInputActionValue& value)
{
	auto* pc = Cast<APersonPlayerController>(GetWorld()->GetFirstPlayerController());
	if (nullptr == pc)
	{
		return;
	}

	bChatEnabled = !bChatEnabled;

	if (bChatEnabled)
	{
		pc->SetInputMode(FInputModeGameAndUI());
		pc->SetShowMouseCursor(true);
	}
	else
	{
		pc->chatUI->OnMyClickSendMsg();
		pc->SetInputMode(FInputModeGameOnly());
		pc->SetShowMouseCursor(false);
	}
}
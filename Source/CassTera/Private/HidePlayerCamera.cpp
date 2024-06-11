

#include "HidePlayerCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "HidePlayer.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"
#include "PersonPlayerController.h"
#include "GameTimerWidget.h"
#include "CassteraGameState.h"
#include "ResultWidget.h"
#include "Components/TextBlock.h"

AHidePlayerCamera::AHidePlayerCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);
	rootComp -> SetCollisionEnabled(ECollisionEnabled::NoCollision);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera -> SetupAttachment(rootComp);
	camera->bUsePawnControlRotation = true;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

}

void AHidePlayerCamera::BeginPlay()
{
	Super::BeginPlay();


}

void AHidePlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector localMoveDir = GetTransform().TransformVector(MovementVector);
	SetActorLocation(GetActorLocation() + localMoveDir * 600 * DeltaTime, false);

	AddControllerYawInput(deltaRotation.Yaw);
	AddControllerPitchInput(deltaRotation.Pitch);
	AddControllerRollInput(deltaRotation.Roll);

}

void AHidePlayerCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{ 
		EnhancedInputComponent->BindAction(ia_move, ETriggerEvent::Triggered, this, &AHidePlayerCamera::OnIAMove);
		EnhancedInputComponent->BindAction(ia_move, ETriggerEvent::Completed, this, &AHidePlayerCamera::OnIAMove);
		EnhancedInputComponent->BindAction(ia_look, ETriggerEvent::Triggered, this, &AHidePlayerCamera::OnIALook);
		EnhancedInputComponent->BindAction(ia_look, ETriggerEvent::Completed, this, &AHidePlayerCamera::OnIALook);
		EnhancedInputComponent->BindAction(ia_changeCam, ETriggerEvent::Started, this, &AHidePlayerCamera::OnIAChangeCamera);

	}
}

void AHidePlayerCamera::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ServerRPC_MakeIMC();
}

void AHidePlayerCamera::OnIAMove(const FInputActionValue& value)
{
	FVector2D moveDir = value.Get<FVector2D>();
	MovementVector = FVector(moveDir.Y, moveDir.X, 0);
}

void AHidePlayerCamera::OnIALook(const FInputActionValue& value)
{
	FVector2D LookAxisVector = value.Get<FVector2D>();
	deltaRotation = FRotator(0, LookAxisVector.X, 0);

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHidePlayerCamera::OnIAChangeCamera(const FInputActionValue& value)
{
	if (bDie)
	{
		return;
	}
	ServerRPC_ChangeCamera();

}

void AHidePlayerCamera::ServerRPC_ChangeCamera_Implementation()
{
	PlayerController = Cast<APersonPlayerController>(Controller);
	if (PlayerController->bHidePlayerDie == true)
	{
		return;
	}
	PlayerController->ChangeToPlayer();
	Destroy();
}

void AHidePlayerCamera::ServerRPC_MakeIMC_Implementation()
{
	MultiRPC_MakeIMC();

}

void AHidePlayerCamera::MultiRPC_MakeIMC_Implementation()
{
	if (IsLocallyControlled())
	{
		//PlayerController = Cast<AHidePlayerController>(Controller);
		auto pc = Cast<APlayerController>(Controller);
		if (pc)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(imc_hidingPlayer, 0);
			}
		}
	}
}


void AHidePlayerCamera::ServerRPC_Lose_Implementation()
{
	MultiRPC_Lost();
}

void AHidePlayerCamera::MultiRPC_Lost_Implementation()
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

void AHidePlayerCamera::ServerRPC_Win_Implementation()
{
	MultiRPC_Win();
}

void AHidePlayerCamera::MultiRPC_Win_Implementation()
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


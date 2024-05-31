// Copyright Epic Games, Inc. All Rights Reserved.

#include "CassTeraCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TestEnemyy.h"
#include "GameTimerWidget.h"
#include "MainUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/StaticMeshComponent.h"
#include "../../../../../../../Program Files/Epic Games/UE_5.3/Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include <Grenade.h>
#include <Kismet/KismetMathLibrary.h>
#include "CassTeraPlayerController.h"
#include "PersonPlayerController.h"
#include "HidePlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACassTeraCharacter

ACassTeraCharacter::ACassTeraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	//GetMesh()->SetupAttachment(FollowCamera);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunComp"));
	gun->SetupAttachment(GetMesh());
	gun->SetRelativeLocation(FVector(6.974221, 17.777489, 3.074630));
	gun->SetRelativeRotation(FRotator(-1.104704, 63.238364, 1.899696));
}

void ACassTeraCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (false == HasAuthority())
	{
		AddMainUI();
	}

	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
		{
			// clearMappingContext를 쓰면 기존에 스폰되어 있던 캐릭터의 imc까지 날라가는듯..
			// RemoveMappingContext(내가 만든 imc) 를 쓰면 원하는 거 하나만 지워줌..
			// 혹시 내가 가지고 있는 imc가 남아있을 수 있으니, 안전하게 같은 이름을 가진 친구가 있다면 지우고, 새로 imc 추가한다.
			Subsystem->RemoveMappingContext(DefaultMappingContext);
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACassTeraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//if (IsLocallyControlled())
	//{
	//	AddMainUI();
	//}
	AddMainUI();

	//Add Input Mapping Context
	//ServerRPC_IMC();

	//if (IsLocallyControlled())
	//{
	//	gameTimerwidget = CreateWidget<UGameTimerWidget>(GetWorld(), WBP_gameTimerWidget);
	//	mainUI = CreateWidget<UMainUI>(GetWorld(), WBP_mainUI);

	//	if (gameTimerwidget != nullptr)
	//	{
	//		gameTimerwidget->AddToViewport();
	//	}

	//	if (mainUI != nullptr)
	//	{
	//		mainUI->AddToViewport();
	//	}
	//}



}

void ACassTeraCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACassTeraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACassTeraCharacter::Move);
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ACassTeraCharacter::MoveFin);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACassTeraCharacter::Look);

		// Fire
		EnhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Started, this, &ACassTeraCharacter::Fire);
		EnhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Completed, this, &ACassTeraCharacter::FireFin);

		// Throw
		EnhancedInputComponent->BindAction(ia_throw, ETriggerEvent::Started, this, &ACassTeraCharacter::Throw);
		EnhancedInputComponent->BindAction(ia_throw, ETriggerEvent::Completed, this, &ACassTeraCharacter::ThrowFinish);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACassTeraCharacter::Move(const FInputActionValue& Value)
{
	//bMoving = true;

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACassTeraCharacter::MoveFin(const FInputActionValue& Value)
{
	//bMoving = false;
}

void ACassTeraCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACassTeraCharacter::AddMainUI()
{
	// 내가 조작하는 주인공만 UI를 생성하고싶다.
	//MyController = Cast<ACassTeraPlayerController>(Controller);
	auto* pc = Cast<APersonPlayerController>(Controller);

	//FString myname = GetName();

	if (IsLocallyControlled() && (pc && nullptr == pc->gameTimerwidget))
	{
		pc->gameTimerwidget = CreateWidget<UGameTimerWidget>(GetWorld(), WBP_gameTimerWidget);
		pc->mainUI = CreateWidget<UMainUI>(GetWorld(), WBP_mainUI);

		if (pc->gameTimerwidget != nullptr)
		{
			pc->gameTimerwidget->AddToViewport();
		}

		if (pc->mainUI != nullptr)
		{
			pc->mainUI->AddToViewport();
		}
	}
	if (pc)
	{
		gameTimerwidget = pc->gameTimerwidget;
		mainUI = pc->mainUI;
	}
}

void ACassTeraCharacter::Fire(const FInputActionValue& Value)
{
	if (bThrowing || bFiring)
		return;

	bFiring = true;

	ServerRPC_Fire();
}

void ACassTeraCharacter::FireFin(const FInputActionValue& Value)
{
	bFiring = false;
}

void ACassTeraCharacter::ServerRPC_Fire_Implementation()
{
	// 총 쏘면서 수류탄 동시에 하지 못하게 하는 변수

	FHitResult HitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	FVector start = FollowCamera->GetComponentLocation();
	FVector end = start + FollowCamera->GetComponentRotation().Vector() * 1000.0f;

	bool bFire = GetWorld()->LineTraceSingleByChannel(HitInfo, start, end, ECC_Visibility, params);

	MultiRPC_Fire(HitInfo, bFire);
}

void ACassTeraCharacter::MultiRPC_Fire_Implementation(FHitResult HitInfo, bool bFire)
{
	//gameTimerwidget = pc->gameTimerwidget;
	//mainUI = pc->mainUI;

	PlayAnimMontage(FireMontage);

	if (bFire)
	{
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, 0, 2);
		//DrawDebugSphere(GetWorld(), HitInfo.Location, 10, 10, FColor::Green, 0, 2);

		// enemy 가 맞으면
		if (HitInfo.GetActor()->IsA<AHidePlayer>())
		{
			AHidePlayer* enemy = Cast<AHidePlayer>(HitInfo.GetActor());

			// 에너미 데미지 -1
			enemy->OnTakeDamage();

			// 죽으면
			if (enemy->bDie)
			{
				// 킬 이미지, 텍스트 UI 띄우기
				if (mainUI)
					mainUI->ShowKillContent();
			}
		}
		else
		{
			if (gameTimerwidget != nullptr)
			{
				// 더블 클릭해서 시간 감소 버그 방지용 변수
				if (bDecreasing)
					return;

				bDecreasing = true;

				gameTimerwidget->DecreaseTime();
				mainUI->img_RedCH->SetVisibility(ESlateVisibility::Visible);

				FTimerHandle visibleHandler;
				GetWorld()->GetTimerManager().SetTimer(visibleHandler, [&]() {

					mainUI->img_RedCH->SetVisibility(ESlateVisibility::Hidden);
					GetWorld()->GetTimerManager().ClearTimer(visibleHandler);
					bDecreasing = false;
					}, 0.5f, false);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Fail222222"));
			}
		}
	}
}


void ACassTeraCharacter::ServerRPC_IMC_Implementation()
{
	MultiRPC_IMC();
}

void ACassTeraCharacter::MultiRPC_IMC_Implementation()
{
	if (IsLocallyControlled())
	{
		/*if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}*/
		auto pc = Cast<APlayerController>(Controller);
		if (pc)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(DefaultMappingContext);
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ACassTeraCharacter::Throw(const FInputActionValue& Value)
{
	if (bFiring || bThrowing)
		return;

	bThrowing = true;

	gun->SetVisibility(false);

	FActorSpawnParameters params;
	grenade = GetWorld()->SpawnActor<AGrenade>(grenade_bp, gun->GetSocketTransform("Weapon_L"), params);
	grenade->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Weapon_L");
}

void ACassTeraCharacter::ThrowFinish(const FInputActionValue& Value)
{
	/*if (bMoving == true)
		return;*/

	if (grenade != nullptr && bThrowing)
	{
		PlayAnimMontage(throwMontage);

		gun->SetVisibility(true);

		grenade->BeforeBomb(this);

	}

	bThrowing = false;
	/*FTimerHandle animDelayHandler;
	GetWorldTimerManager().SetTimer(animDelayHandler, [&]() {
		
	}, 2.2f, false);*/
	
}


//void ACassTeraCharacter::ServerRPC_Throw_Implementation()
//{
//
//}
//
//void ACassTeraCharacter::MultiRPC_Throw_Implementation(FHitResult HitInfo, bool bFire)
//{
//
//}




#include "HidePlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

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

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->TargetArmLength = 400.f;
	cameraBoom->bUsePawnControlRotation = true;
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(cameraBoom);
	camera->bUsePawnControlRotation = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(RootComponent);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

}

void AHidePlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(imc_hidingPlayer, 0);
		}
	}
}

void AHidePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector localMoveDir = GetTransform().TransformVector(MovementVector);

	SetActorLocation(GetActorLocation() + localMoveDir * 600 * DeltaTime);
	AddControllerYawInput(deltaRotation.Yaw);
	AddControllerPitchInput(deltaRotation.Pitch);
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
	}
}

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
	Jump();
} 

void AHidePlayer::OnIASounding(const FInputActionValue& value)
{
}

void AHidePlayer::OnIALockLocation(const FInputActionValue& value)
{
}

void AHidePlayer::OnIAChangeCamera(const FInputActionValue& value)
{
}



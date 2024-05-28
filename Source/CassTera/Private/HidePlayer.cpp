

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

	// 랜덤 매시로 로드
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh0(TEXT("/Script/Engine.StaticMesh'/Game/Bohyun/Meshs/BlackBoard.BlackBoard'"));
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



	//랜덤 매시 로딩 
	int32 random = FMath::RandRange(0, meshOptions.Num() - 1);
	if (random == 0)
	{
		meshComp->SetStaticMesh(meshOptions[0]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 0, -10), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(0.7f,0.7f,1));

	}
	if (random == 1)
	{
		meshComp->SetStaticMesh(meshOptions[1]);
		meshComp->SetRelativeLocationAndRotation(FVector(0,0,-80), FRotator(0,90,0));
		meshComp->SetRelativeScale3D(FVector(1.5f));
	}
	if (random == 2)
	{
		meshComp->SetStaticMesh(meshOptions[2]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 0, -80), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(5));
	}
	if (random == 3)
	{
		meshComp->SetStaticMesh(meshOptions[3]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 0, -80), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(1.5f));
	}
	if (random == 4)
	{
		meshComp->SetStaticMesh(meshOptions[4]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, -320, -80), FRotator(0, 180, 0));
		meshComp->SetRelativeScale3D(FVector(80));
	}
	if (random == 5)
	{
		meshComp->SetStaticMesh(meshOptions[5]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 0, -30), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(0.55f));
	}
	if (random == 6)
	{
		meshComp->SetStaticMesh(meshOptions[6]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 10, 20), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(1));
	}
	if (random == 7)
	{
		meshComp->SetStaticMesh(meshOptions[7]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 10, -80), FRotator(0, 200, 0));
		meshComp->SetRelativeScale3D(FVector(2));
	}
	if (random == 8)
	{
		meshComp->SetStaticMesh(meshOptions[8]);
		meshComp->SetRelativeLocationAndRotation(FVector(-30, -20, -88), FRotator(0, 180, 0));
		meshComp->SetRelativeScale3D(FVector(3));
	}
	if (random == 9)
	{
		meshComp->SetStaticMesh(meshOptions[9]);
		meshComp->SetRelativeLocationAndRotation(FVector(0, 0, -38), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(3));
	}
	if (random == 10)
	{
		meshComp->SetStaticMesh(meshOptions[10]);
		meshComp->SetRelativeLocationAndRotation(FVector(-80, -40, -368), FRotator(0, 90, 0));
		meshComp->SetRelativeScale3D(FVector(2));
	}

	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(imc_hidingPlayer, 0);
		}
	}
	
	currentHP = maxHP;
}

void AHidePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector localMoveDir = GetTransform().TransformVector(MovementVector);
	if (bLockLocation == true)
	{
		SetActorLocation(lockLoc, true);
		SetActorRotation(lockRot);
	}
	else 
	{
		SetActorLocation(GetActorLocation() + localMoveDir * 600 * DeltaTime, true);
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
		UnLockLocation();
	}
	else
	{
		LockLocation();
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
	if (!bLockLocation)
	{
		return;
	}

	if (bChangeCam)
	{
		OnResetCamera();
	}
	else
	{
		OnChangeCamera();
	}

}

void AHidePlayer::OnChangeCamera()
{
	if (bChangeCam)
	{
		return;
	}
	bChangeCam = true;

	FVector loc = GetActorLocation() + FVector(0, 50, 50);
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	watchingCam = GetWorld()->SpawnActor<AHidePlayerCamera>(watcingCam_bp, loc, FRotator::ZeroRotator, params);
	if (watchingCam)
	{
		PlayerController->Possess(watchingCam);
	}
	
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
	currentHP = currentHP-1;
	if (hitVFX != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitVFX, GetActorLocation());
	}
	if (currentHP <= 0)
	{
		bDie = true;
		Die();
	}
}

void AHidePlayer::Die()
{
	if (dieVFX != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), dieVFX, GetActorLocation());
	}
}



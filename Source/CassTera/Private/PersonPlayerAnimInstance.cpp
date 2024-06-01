// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonPlayerAnimInstance.h"
#include "CassTeraCharacter.h"

void UPersonPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	player = Cast<ACassTeraCharacter>(TryGetPawnOwner());
}

void UPersonPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (nullptr == player)
		return;

	auto vel = player->GetVelocity();

	horizontal = FVector::DotProduct(vel, player->GetActorRightVector());

	vertical = FVector::DotProduct(vel, player->GetActorForwardVector());

	PitchAngle = player->GetBaseAimRotation().GetNormalized().Pitch;

	PitchAngle = FMath::Clamp(-PitchAngle, -60, 60);


}

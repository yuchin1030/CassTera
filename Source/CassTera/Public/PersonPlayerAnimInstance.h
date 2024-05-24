// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PersonPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CASSTERA_API UPersonPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY()
	class ACassTeraCharacter* player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	float horizontal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	float vertical;
	 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	float PitchAngle;


};

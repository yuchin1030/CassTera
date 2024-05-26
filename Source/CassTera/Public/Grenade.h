// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class CASSTERA_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrenade();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UParticleSystem* bombVFX;

	void Bomb();
};

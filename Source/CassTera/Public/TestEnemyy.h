// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestEnemyy.generated.h"

UCLASS()
class CASSTERA_API ATestEnemyy : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestEnemyy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	float enemyHP = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	UParticleSystem* correctVFX;

	UFUNCTION()
	void OnDamaged(float dmg);

};

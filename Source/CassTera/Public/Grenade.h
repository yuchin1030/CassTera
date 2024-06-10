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

	class UParticleSystemComponent* spawnedBombVFX;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	FVector bombLoc;

	bool bBeforeBomb;

	UPROPERTY()
	FVector newVel;

	UPROPERTY()
	float speed;


	UFUNCTION(Server, Reliable)
	void ServerRPC_BeforeBomb();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_BeforeBomb(FVector _newVel, float _speed);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Bomb();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Bomb();

	UPROPERTY()
	class ACassTeraCharacter* playerChar;

	UPROPERTY()
	class ACassteraGameState* gs;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* explosionSound;
};

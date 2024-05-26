// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEnemyy.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../../../../../Program Files/Epic Games/UE_5.3/Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

// Sets default values
ATestEnemyy::ATestEnemyy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestEnemyy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestEnemyy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestEnemyy::OnDamaged(float dmg)
{
	enemyHP = FMath::Clamp(enemyHP -= dmg, 0.0f , 5.0f);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitVFX, GetActorLocation());

	UE_LOG(LogTemp, Warning, TEXT("%f"), enemyHP);

	if (enemyHP <= 0)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), dieVFX, GetActorLocation());

		Destroy();
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects.h"

// Sets default values
AObjects::AObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	objectComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Component"));
	objectComp->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AObjects::BeginPlay()
{
	Super::BeginPlay();

	// 피직스 끈다
	objectComp->SetSimulatePhysics(false);
	//RandomSpawn();
}

// Called every frame
void AObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjects::RandomSpawn()
{
	// 랜덤으로 visible, 콜리전, 피직스를 끄고 시작한다
	int32 number = FMath::RandRange(0, 100);

	if (number < rate)
	{
		objectComp->SetVisibility(false);
		objectComp->SetSimulatePhysics(false);
		objectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

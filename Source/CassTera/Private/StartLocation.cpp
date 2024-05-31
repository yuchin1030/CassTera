// Fill out your copyright notice in the Description page of Project Settings.


#include "StartLocation.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>

// Sets default values
AStartLocation::AStartLocation()
{
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	locComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Start Location Component"));
	locComp -> SetupAttachment(RootComponent);
}

void AStartLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStartLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


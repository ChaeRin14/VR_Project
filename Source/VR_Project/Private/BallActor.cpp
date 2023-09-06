// Fill out your copyright notice in the Description page of Project Settings.


#include "BallActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

ABallActor::ABallActor()
{
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(rootComp);


}

void ABallActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


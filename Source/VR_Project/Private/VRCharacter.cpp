// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"


AVRCharacter::AVRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


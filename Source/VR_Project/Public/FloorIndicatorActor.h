// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorIndicatorActor.generated.h"

UCLASS()
class VR_PROJECT_API AFloorIndicatorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFloorIndicatorActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallActor.generated.h"

UCLASS()
class VR_PROJECT_API ABallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABallActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class USceneComponent* rootComp;

	UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class UStaticMeshComponent* meshComp;

};

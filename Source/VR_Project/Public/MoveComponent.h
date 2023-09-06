// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR_PROJECT_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMoveComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions);

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Movement")
	float interval = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Movement")
	int32 segment = 50;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Movement")
	FVector throwDirection = FVector(1, 0, 1);

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Movement")
	float throwPower = 500;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Movement")
	float myMass = 1;

	

private:
	class AVRCharacter* player;
	bool bIsShowLine = false;
	TArray<FVector> linePositions;


	void Move(const struct FInputActionValue& value);
	void Rotate(const struct FInputActionValue& value);
	void LeftTriggerDown();
	void LeftTriggerUp();

	void DrawTrajectory(FVector dir, float power, float mass);
	
};

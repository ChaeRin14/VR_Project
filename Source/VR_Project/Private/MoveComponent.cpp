// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Components/TextRenderComponent.h"
#include "VRCharacter.h"
#include "MotionControllerComponent.h"
#include "DrawDebugHelpers.h"
#include "BallActor.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"


UMoveComponent::UMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	player = GetOwner<AVRCharacter>();

	//FTimerHandle massTimer;
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]() {
		if (player->ball != nullptr)
		{
			player->ball->meshComp->SetSimulatePhysics(true);
			myMass = player->ball->meshComp->GetMass();
			player->ball->meshComp->SetSimulatePhysics(false);
		}
		}));

}


void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 텔레포트 라인 그리기
	if (bIsShowLine)
	{
		DrawTrajectory(throwDirection, throwPower, myMass);
		//DrawBezierCurve();
	}
}

void UMoveComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
	enhancedInputComponent->BindAction(inputActions[0], ETriggerEvent::Triggered, this, &UMoveComponent::Move);
	enhancedInputComponent->BindAction(inputActions[0], ETriggerEvent::Completed, this, &UMoveComponent::Move);
	enhancedInputComponent->BindAction(inputActions[1], ETriggerEvent::Triggered, this, &UMoveComponent::Rotate);
	enhancedInputComponent->BindAction(inputActions[1], ETriggerEvent::Completed, this, &UMoveComponent::Rotate);
	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Started, this, &UMoveComponent::LeftTriggerDown);
	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Completed, this, &UMoveComponent::LeftTriggerUp);
}

void UMoveComponent::Move(const FInputActionValue& value)
{
	FVector2D controllerInput = value.Get<FVector2D>();
	player->leftLog->SetText(FText::FromString(FString::Printf(TEXT("x: %.2f\r\ny: %.2f"), controllerInput.X, controllerInput.Y)));

	FVector forwardVec = FRotationMatrix(player->pc->GetControlRotation()).GetUnitAxis(EAxis::X);
	FVector rightVec = FRotationMatrix(player->pc->GetControlRotation()).GetUnitAxis(EAxis::Y);

	player->AddMovementInput(forwardVec, controllerInput.X);
	player->AddMovementInput(rightVec, controllerInput.Y);
}

void UMoveComponent::Rotate(const FInputActionValue& value)
{
	FVector2D rightConInput = value.Get<FVector2D>();

	player->rightLog->SetText(FText::FromString(FString::Printf(TEXT("%.2f\r\n%.2f"), rightConInput.X, rightConInput.Y)));

	if (player->pc != nullptr)
	{
		player->pc->AddYawInput(rightConInput.X);
		player->pc->AddPitchInput(rightConInput.Y);
	}
}

void UMoveComponent::LeftTriggerDown()
{
	bIsShowLine = true;
}

void UMoveComponent::LeftTriggerUp()
{
	//bIsShowLine = false;

	if (player->ball != nullptr)
	{
		player->ball->meshComp->SetSimulatePhysics(true);

		FVector releativeDir = player->leftMotionController->GetComponentTransform().TransformVector(throwDirection);
		player->ball->meshComp->AddImpulse(releativeDir * throwPower);
	}

	float duration = 1.0f;
	player->pc->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, duration, FLinearColor::Black, true);

	FTimerHandle moveHandle;
	GetWorld()->GetTimerManager().SetTimer(moveHandle, FTimerDelegate::CreateLambda([&]() {
		// 나를 이동시킨다.
		FVector targetLoc = linePositions[linePositions.Num() - 1];
		targetLoc.Z += player->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		player->SetActorLocation(targetLoc, true);
		}), duration, false);

}

void UMoveComponent::DrawTrajectory(FVector dir, float power, float mass)
{
	linePositions.Empty();

	UWorld* world = GetWorld();
	FVector startLoc = player->leftMotionController->GetComponentLocation();
	FVector direction = player->leftMotionController->GetComponentTransform().TransformVector(dir);

	for (int32 i = 0; i < segment; i++)
	{
		float elapsedTime = interval * i;
		float gravity = world->GetGravityZ();
		FVector downForce = FVector(0, 0, 0.5f * gravity * elapsedTime * elapsedTime * mass * mass);
		FVector endLoc = startLoc + direction * power * elapsedTime + downForce;

		FHitResult hitInfo;

		if (i > 0 && world->LineTraceSingleByChannel(hitInfo, linePositions[i - 1], endLoc, ECC_Visibility))
		{
			endLoc = hitInfo.ImpactPoint;
			linePositions.Add(endLoc);
			break;
		}

		linePositions.Add(endLoc);
	}

	/*for (int i = 0; i < linePositions.Num() - 1; i++)
	{
		DrawDebugLine(world, linePositions[i], linePositions[i + 1], FColor::Red, false, 0, 0, 2);
	}*/

	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(player->lineFx, FName("PointArray"), linePositions);
}

void UMoveComponent::DrawBezierCurve()
{
	FVector startLoc = player->leftMotionController->GetComponentLocation();
	FVector relativeDir = player->leftMotionController->GetComponentTransform().TransformVector(throwDirection);
	FHitResult hitInfo;

	if (GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, startLoc + relativeDir * throwPower, ECC_Visibility))
	{
		float heightDist = FMath::Abs(hitInfo.ImpactPoint.Z - player->leftMotionController->GetComponentLocation().Z);

		FVector point1 = startLoc;
		FVector point2 = hitInfo.ImpactPoint + FVector::UpVector * heightDist * 1.5f;
		FVector point3 = hitInfo.ImpactPoint;
		float alpha = 0;
		linePositions.Empty();

		for (int32 i = 0; i < 11; i++)
		{
			FVector lineStart = FMath::Lerp<FVector>(point1, point2, alpha);
			FVector lineEnd = FMath::Lerp<FVector>(point2, point3, alpha);
			FVector	linePoint = FMath::Lerp<FVector>(lineStart, lineEnd, alpha);
			alpha += 0.1f;

			linePositions.Add(linePoint);
		}

		for (int32 i = 0; i < linePositions.Num() - 1; i++)
		{
			DrawDebugLine(GetWorld(), linePositions[i], linePositions[i+1], FColor::Green, false, 0, 0, 2);
		}
	}
	else
	{

	}
}


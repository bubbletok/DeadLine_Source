// Fill out your copyright notice in the Description page of Project Settings.


#include "Noise/BaseNoiseEmitterComponent.h"
#include "Noise/EnvironmentManager.h"

#include "Perception/AISense_Hearing.h"

#include <Kismet/KismetSystemLibrary.h>

#include "Math/UnrealMathUtility.h"
#include "Noise/TemporalNoiseEmitterPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBaseNoiseEmitterComponent::UBaseNoiseEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBaseNoiseEmitterComponent::BeginPlay()
{
	Super::BeginPlay();

	InstigatorPawn = Cast<APawn>(GetOwner());

	if (!InstigatorPawn.IsValid())
	{
		InstigatorPawn = Cast<APawn>(GetOwner()->GetInstigator());
	}
}

// Called every frame
//void UBaseNoiseEmitterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

/*
 * Make a Noise Event
 * AI_Hearing���� ������ �� �ִ� Event�� �����Ѵ�.
 * MakeNoise �Լ��� �ݵ�� NoiseInstigator�� NoiseEmitterComponent�� ���� Pawn�� �����ؾ��Ѵ�.
 *
 * @param Loudness Loudness�� �Ҹ��� �����, Tag�� ���� ���޵Ǵ� FName ���̴�.
 * @param NoiseInstigator NoiseInstigator�� �Ҹ��� �߻���Ű�� Pawn����, NoiseInstigator�� nullptr�� ��� Component�� ������ InstigatorPawn�� ����Ѵ�.
 * @param NoiseLocation NoiseLocation�� �Ҹ��� �߻��� ��ġ�̴�.
 * @param Range Range�� �Ҹ��� ���޵Ǵ� �����̴�.
 */
void UBaseNoiseEmitterComponent::MakeEnvironmentNoise_Implementation(APawn* NoiseInstigator, float Loudness,
                                                                     const FVector& NoiseLocation, float Range)
{
	APawn* TempPawn = nullptr;

	if (NoiseInstigator)
	{
		TempPawn = NoiseInstigator;
	}
	else
	{
		//Spawn a Temporal Noise Emitter Pawn
		auto tempPawn = GetWorld()->SpawnActor<ATemporalNoiseEmitterPawn>(ATemporalNoiseEmitterPawn::StaticClass(), NoiseLocation, FRotator::ZeroRotator);
		tempPawn->SetLifeSpan(1.0f);
		TempPawn = tempPawn;
	}
	check(TempPawn);

	//debug log
	FString b = TempPawn->GetName();

	// UE_LOG(LogTemp, Display, TEXT("Instigator Name : %s"), *b);

	int LoudnessInt = FMath::Floor(Loudness);
	FString LoudnessString = FString::FromInt(LoudnessInt);
	FName LoudnessName = FName(*LoudnessString);

	// ���� ȯ�濡 ���� Hearing Range �� ����
	// EnvironmentManager�� ���� ���� ȯ���� �����´�.
	UEnvironmentManager* EnvironmentManager = GetWorld()->GetSubsystem<UEnvironmentManager>();
	float EnvironmentNoiseRangeMultiplier = EnvironmentManager->GetEnvironmentNoiseRangeMultiplier(TempPawn->GetActorLocation());

	// Range ���� ���
	Range = Range * EnvironmentNoiseRangeMultiplier;

	//Using ReportNoiseEvent to make noise
	UAISense_Hearing::ReportNoiseEvent(this, NoiseLocation, 1, TempPawn, Range, LoudnessName);

	//Spawn Noise Particle 
	if (NoiseParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NoiseParticleSystem, NoiseLocation);
	}
}

/*
 * �ֱ������� ������ �߻���Ű�� �Լ�
 * ?? ���� �ֱ��?
 */
void UBaseNoiseEmitterComponent::MakeContinuousEnvironmentNoise_Implementation(
	APawn* NoiseInstigator, float Loudness, const FVector& NoiseLocation, float Range, float Duration)
{
	// Timer�� ���� �ֱ������� ������ �߻���Ŵ
	FTimerHandle noiseTimerHandle;

	FTimerDelegate continuousEvent;
	continuousEvent.BindUFunction(this, FName("MakeEnvironmentNoise_Implementation"), NoiseInstigator, Loudness, NoiseLocation, Range);
	GetWorld()->GetTimerManager().SetTimer(noiseTimerHandle, continuousEvent, 1.0f, true, 0.0f);

	// Duration��ŭ�� �ð��� ������ ���� �߻��� ������.
	FTimerHandle DurationTimerHandle;
	bool isTimerEnd = false;
	// Timer�� ������ TimerHandle�� Clear�Ѵ�.
	auto durationTimer = [&noiseTimerHandle, this]()
	{
		GetWorld()->GetTimerManager().ClearTimer(noiseTimerHandle);
	};
	GetWorld()->GetTimerManager().SetTimer(DurationTimerHandle, durationTimer, Duration, false);

	return;
}

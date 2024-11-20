// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Particles/ParticleSystem.h"
#include "Perception/AISense_Hearing.h"
#include "NoiseEmitterInterface.h"

#include "BaseNoiseEmitterComponent.generated.h"

// TODO : UPawnNoiseEmitterComponent ������� �����������
// https://docs.unrealengine.com/4.26/en-US/BlueprintAPI/AI/MakeNoise/ ����
// Make Noise �Լ��� �ݵ�� UPawnNoiseEmitterComponent�� ���� Pawn������ ��밡��
UCLASS(Blueprintable, ClassGroup=(ZombieProjectNoiseEmitter), meta=(BlueprintSpawnableComponent) )
class ZOMBIEPROJECT_API UBaseNoiseEmitterComponent : public UActorComponent, public INoiseEmitterInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseNoiseEmitterComponent();

	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//void MakeNoise(float Loudness, FVector NoiseLocation, float Range);
	void MakeEnvironmentNoise_Implementation(APawn* NoiseInstigator, float Loudness, const FVector& NoiseLocation, float Range) override;

	//void MakeContinuousNoise(float Loudness, FVector NoiseLocation, float Range, float Duration);
	void MakeContinuousEnvironmentNoise_Implementation(APawn* NoiseInstigator, float Loudness, const FVector& NoiseLocation, float Range, float Duration) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ZombieProjectNoiseEmitter)
	TWeakObjectPtr<APawn> InstigatorPawn;

	//Particle System to play when making noise
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ZombieProjectNoiseEmitter)
	UParticleSystem* NoiseParticleSystem;
};

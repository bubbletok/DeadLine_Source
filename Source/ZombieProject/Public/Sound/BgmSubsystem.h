// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BgmSubsystem.generated.h"

/**
 * BGM�� �����ϱ� ���� ���� �ý���, ������ ������� BGM�� ����� �� �ֽ��ϴ�.
 * ������ �̵��� ������ ���� ��� ���� ������ �����˴ϴ�.
 */
UCLASS()
class ZOMBIEPROJECT_API UBgmSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UBgmSubsystem();
	
	// USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End of USubsystem Interface

	UFUNCTION(BlueprintCallable, Category = "BGM")
	void PlayBgm(int BgmId);
	

private:
	// ������
	
	UDataTable* BgmDataTable;

	TSoftObjectPtr<USoundWave> CurrentBgm;

	void OnBgmLoaded(const int BgmId) const;
};

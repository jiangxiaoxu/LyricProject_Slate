// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "LTypes.h"
#include "LrcHUD.generated.h"

/**
 * 
 */

class SStandardWidget;

UCLASS()
class LYRICSPROJECT_API ALrcHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ALrcHUD();

	TSharedPtr<class SWidget> MyWidget;

	virtual void BeginPlay()override;
	
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	UAudioComponent*  AudioComp;

	float StartTime = -1;

	float SoundDuration = -1;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	ULyricAsset*   LyricAsset;

	UFUNCTION( BlueprintCallable, Category = "Audio")
	void PlaySound();

	float GetPlayedSeconds()const;
};

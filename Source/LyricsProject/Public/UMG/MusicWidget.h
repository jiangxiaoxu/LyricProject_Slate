// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "MusicWidget.generated.h"

/**
 * 
 */
UCLASS()
class LYRICSPROJECT_API UMusicWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION( BlueprintCallable,Category="lrc")
	 class ULyricAsset* LoadLyricAsset();
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "MusicWidget.h"
#include "LTypes.h"




class ULyricAsset* UMusicWidget::LoadLyricAsset()
{
	FString LrcString;
	FString Path = FPaths::GameDir();
	Path += TEXT("高桥瞳 - 青空のナミダ.lrc");

	FFileHelper::LoadFileToString(LrcString, *Path);

	ULyricAsset* NewAsset = NewObject<ULyricAsset>(this);
	NewAsset->LoadFromString(LrcString);
	return  NewAsset;
}


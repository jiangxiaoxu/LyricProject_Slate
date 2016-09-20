// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SimpleAssetEditor.h"

/**
 *
 */
class LYRICSPROJECT_API FLyricAssetEditor :public FSimpleAssetEditor
{
public:

	static TSharedRef<FLyricAssetEditor> CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UObject* ObjectToEdit, FGetDetailsViewObjects GetDetailsViewObjects = FGetDetailsViewObjects());

	//static TSharedRef<FLyricAssetEditor> CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, const TArray<UObject*>& ObjectsToEdit, FGetDetailsViewObjects GetDetailsViewObjects = FGetDetailsViewObjects());

};

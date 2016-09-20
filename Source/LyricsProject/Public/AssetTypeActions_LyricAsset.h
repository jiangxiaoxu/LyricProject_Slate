// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealEd.h"
#include "AssetTypeActions_Base.h"

/**
 * 
 */
class LYRICSPROJECT_API FAssetTypeActions_LyricAsset:public FAssetTypeActions_Base
{
public:
	virtual bool IsImportedAsset() const override { return true; }

	virtual FText GetName() const override{ return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_LyricAsset", "LyricAsset"); }

	virtual UClass* GetSupportedClass() const override;

	virtual FColor GetTypeColor() const override { return FColor::White; }

	virtual uint32 GetCategories() override{ return EAssetTypeCategories::Misc; }

	virtual void GetResolvedSourceFilePaths(const TArray<UObject *>& TypeAssets, TArray<FString>& OutSourceFilePaths)const override;

	// slate
	virtual bool HasActions(const TArray<UObject *>& InObjects) const override { return true; }
	
	virtual void GetActions(const TArray<UObject *>& InObjects, FMenuBuilder& MenuBuilder) override;

	void Execute_ZeroOffset( TArray<TWeakObjectPtr<class ULyricAsset>>  Objects);

	virtual void OpenAssetEditor(const TArray<UObject *>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /* = TSharedPtr<IToolkitHost>() */)override;


	//////////////////////////////////////////////////////////////////////////
	

	static void Initialize();

	static void ShutDown();

private:
	static	TSharedPtr<FAssetTypeActions_LyricAsset>   SingletonPtr;
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "AssetTypeActions_LyricAsset.h"
#include "LTypes.h"
#include "LyricAssetEditor.h"

#define LOCTEXT_NAMESPACE "Your Namespace" 

UClass* FAssetTypeActions_LyricAsset::GetSupportedClass() const
{
	return ULyricAsset::StaticClass();
}

void FAssetTypeActions_LyricAsset::GetResolvedSourceFilePaths(const TArray<UObject *>& TypeAssets, TArray<FString>& OutSourceFilePaths) const
{

	for (auto Asset : TypeAssets)
	{
		auto LrcAsset = CastChecked<ULyricAsset>(Asset);

		if (LrcAsset->AssetImportData)
		{
			OutSourceFilePaths.Add(LrcAsset->AssetImportData->GetFirstFilename());
		}
	}
}

void FAssetTypeActions_LyricAsset::GetActions(const TArray<UObject *>& InObjects, FMenuBuilder& MenuBuilder)
{
	TArray<TWeakObjectPtr<class ULyricAsset>> MyAssetImports = GetTypedWeakObjectPtrs<ULyricAsset>(InObjects);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("Lyric_Asset", "Zero Time Offset"),
			LOCTEXT("Lyric_AssetTooltip", "Zero Time Offset"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateSP(this, &FAssetTypeActions_LyricAsset::Execute_ZeroOffset, MyAssetImports), FCanExecuteAction())
		);

}

void FAssetTypeActions_LyricAsset::Execute_ZeroOffset(TArray<TWeakObjectPtr<class ULyricAsset>> Objects)
{

	for (TWeakObjectPtr<ULyricAsset> ptr : Objects)
	{
		if (ptr.IsValid())
		{
			ptr->TimeOffset = 0.0f;
			ptr->MarkPackageDirty();
		}
	}
}


void FAssetTypeActions_LyricAsset::OpenAssetEditor(const TArray<UObject *>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /* = TSharedPtr<IToolkitHost>() */)
{
	for (UObject* Obj:InObjects)
	{
		FLyricAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, Obj);
	}

}

TSharedPtr<FAssetTypeActions_LyricAsset> FAssetTypeActions_LyricAsset::SingletonPtr=nullptr;

void FAssetTypeActions_LyricAsset::Initialize()
{
	 if (!SingletonPtr.IsValid())
	 {
		 SingletonPtr=MakeShareable(new FAssetTypeActions_LyricAsset) ;
	 }

	 FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get().RegisterAssetTypeActions(SingletonPtr.ToSharedRef());

}

void FAssetTypeActions_LyricAsset::ShutDown()
{
	if (SingletonPtr.IsValid())
	{
		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get().UnregisterAssetTypeActions(SingletonPtr.ToSharedRef());
		}
		SingletonPtr.Reset();
	}
}

#undef 	LOCTEXT_NAMESPACE

// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "AssetTypeActions_LyricAsset.h"
#include "LyricProjectWidgetStyles.h"

//IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, LyricsProject, "LyricsProject" );


class FLyricsModule :public FDefaultGameModuleImpl
{
	virtual void StartupModule() override;

	virtual void ShutdownModule()override;

};

void FLyricsModule::StartupModule()
{
	FAssetTypeActions_LyricAsset::Initialize();

	FSlateStyleRegistry::UnRegisterSlateStyle(FLyricProjectWidgetStyles::GetStyleSetName());
	FLyricProjectWidgetStyles::Initialize();

}

void FLyricsModule::ShutdownModule()
{
	FAssetTypeActions_LyricAsset::ShutDown();

	FLyricProjectWidgetStyles::Shutdown();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FLyricsModule, LyricsProject, "LyricsProject");
// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LyricProjectWidgetStyles.h"
#include "SlateGameResources.h" 

void FLyricProjectWidgetStyles::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FLyricProjectWidgetStyles::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

const class ISlateStyle& FLyricProjectWidgetStyles::Get()
{
	 return *StyleInstance;
}

FName FLyricProjectWidgetStyles::GetStyleSetName()
{
	static FName StyleName(TEXT("LyricWidgetStyle"));
	return StyleName;
}

TSharedRef<class FSlateStyleSet> FLyricProjectWidgetStyles::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(FLyricProjectWidgetStyles::GetStyleSetName(), "/Game/UI/Styles", "/Game/UI/Styles");
	return StyleRef;
}

TSharedPtr<class FSlateStyleSet> FLyricProjectWidgetStyles::StyleInstance=nullptr;


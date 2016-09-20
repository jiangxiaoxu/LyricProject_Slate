// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LyricWidgetStyle.h"

void FLyricWidgetStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{

}

const FName FLyricWidgetStyle::TypeName = TEXT("FGlobalStyle");

const FName FLyricWidgetStyle::GetTypeName() const
{
	static const FName TheTypeName = TEXT("FGlobalStyle");
	return TheTypeName;
}

const FLyricWidgetStyle& FLyricWidgetStyle::GetDefault()
{
		static FLyricWidgetStyle 	TheGlobalStyle;
		return 	  TheGlobalStyle;
}

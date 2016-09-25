// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LyricWidgetStyle.h"

void FLyricWidgetStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	OutBrushes.Add(&BackgroundBrush);
}

const FName FLyricWidgetStyle::TypeName = TEXT("FGlobalStyle");

const FName FLyricWidgetStyle::GetTypeName() const
{
	return TypeName;
}

const FLyricWidgetStyle& FLyricWidgetStyle::GetDefault()
{
		static FLyricWidgetStyle 	TheGlobalStyle;
		return 	  TheGlobalStyle;
}

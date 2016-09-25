// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SlateBasics.h"
/**
 * 
 */


class LYRICSPROJECT_API SLrcMenuWidget :public SCompoundWidget
{

	SLATE_BEGIN_ARGS(SLrcMenuWidget) {}

	SLATE_ARGUMENT(TAutoWeakObjectPtr<class ALrcHUD>, OwnerHUD)

	SLATE_ATTRIBUTE(float, PlayedSeconds)

	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

	

private:
	const struct FLyricWidgetStyle* MyStyle;

	void	BuildLyricLine();

	EActiveTimerReturnType UpdateScrollTo(double InCurrentTime, float InDeltaTime);

	TAutoWeakObjectPtr<class ALrcHUD> OwnerHUD;

	TSharedPtr<SScrollBox>  ContainBox;

	TArray<TSharedRef<STextBlock>>	 Lines;

	TAttribute<float> PlayedSeconds;
};

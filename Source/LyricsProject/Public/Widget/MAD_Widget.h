// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SlateBasics.h"
/**
 * 
 */
class LYRICSPROJECT_API SMAD_Widget :public SCompoundWidget
{

	SLATE_BEGIN_ARGS(SMAD_Widget) {}

	SLATE_ARGUMENT(TAutoWeakObjectPtr<class ALrcHUD>, OwnerHUD)

//		SLATE_ATTRIBUTE(float, PlayedSeconds)

		SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);


private:
		const struct FLyricWidgetStyle* MyStyle;

	TAutoWeakObjectPtr<class ALrcHUD> OwnerHUD;

	FText GetLyricText()const;

};

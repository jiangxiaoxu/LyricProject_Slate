// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
/**
 * 
 */
class LYRICSPROJECT_API SStandardWidget :public SCompoundWidget
{


	SLATE_BEGIN_ARGS(SStandardWidget) {}

	SLATE_ARGUMENT(TAutoWeakObjectPtr<class ALrcHUD>, OwnerHUD)

		SLATE_END_ARGS()
public:

	void Construct(const FArguments& InArgs);



private:
	TAutoWeakObjectPtr<class ALrcHUD>  OwnerHUD;
};

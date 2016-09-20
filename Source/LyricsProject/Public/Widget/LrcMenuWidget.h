// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SlateBasics.h"
/**
 * 
 */

class LYRICSPROJECT_API SLrcMenuWidget :public SCompoundWidget
{

	SLATE_BEGIN_ARGS(SLrcMenuWidget){ }

		SLATE_ARGUMENT(TAutoWeakObjectPtr<class ALrcHUD>,OwnerHUD)

		SLATE_ATTRIBUTE(FText,Intext)

		SLATE_END_ARGS()

public:
	 void Construct(const FArguments& InArgs);

	 TAutoWeakObjectPtr<class ALrcHUD> OwnerHUD;

};

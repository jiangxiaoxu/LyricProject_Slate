// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "LrcHUD.generated.h"

/**
 * 
 */

class SStandardWidget;

UCLASS()
class LYRICSPROJECT_API ALrcHUD : public AHUD
{
	GENERATED_BODY()
	
	
public:
	TSharedPtr<class SWidget>	 MyWidget;

	virtual void PostInitializeComponents()override;

	virtual void BeginPlay()override;
	

	virtual void Tick( float DeltaSeconds )override;

	float TimeCalcu=0;
	



};

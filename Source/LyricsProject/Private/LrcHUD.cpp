// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LrcHUD.h"

#include "StandardWidget.h"
#include "LrcMenuWidget.h"



void ALrcHUD::PostInitializeComponents()
{
	  Super::PostInitializeComponents();
	  
	  GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(
		  SAssignNew(MyWidget, SLrcMenuWidget)
		  .OwnerHUD(this)
		  .Intext_Lambda([this]{return FText::AsNumber(static_cast<int32>(TimeCalcu));})
	  ));
}

void ALrcHUD::BeginPlay()
{
	 Super::BeginPlay();
	 
	 GetOwningPlayerController()->bShowMouseCursor = true;
}

void ALrcHUD::Tick(float DeltaSeconds)
{
	   Super::Tick(DeltaSeconds);
	   
	   TimeCalcu+= DeltaSeconds;
}

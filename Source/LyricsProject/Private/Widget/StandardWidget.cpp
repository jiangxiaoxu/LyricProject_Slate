// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "StandardWidget.h"
#include "LrcHUD.h"

#define LOCTEXT_NAMESPACE "WidgetnameSpace"

void SStandardWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
			[
			   SNew(STextBlock)
			   .ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(1,1))
				.Font(FSlateFontInfo("Veranda",40))
				.Text(LOCTEXT("mytext","That is text block"))
			]
		];
}



#undef LOCTEXT_NAMESPACE
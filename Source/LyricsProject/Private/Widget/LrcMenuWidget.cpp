// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LrcMenuWidget.h"
#include "LrcHUD.h"
#include "LyricProjectWidgetStyles.h"
#include "LyricWidgetStyle.h"

void SLrcMenuWidget::Construct(const FArguments& InArgs)
{
	bCanTick = false;

	OwnerHUD = InArgs._OwnerHUD;

	auto TheStyle = &FLyricProjectWidgetStyles::Get().GetWidgetStyle<FLyricWidgetStyle>("Style1");

	ChildSlot
		[
		  SNew(SOverlay) 
		  +SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor::White)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ShadowOffset(FIntPoint(-1,1))
				.Font(TheStyle->FontInfo )
				.Text(InArgs._Intext)
			]
			 +SOverlay::Slot()
				.HAlign(HAlign_Right)
				 .VAlign(VAlign_Bottom)
				  [
					SNew(SBox)
					.WidthOverride(400)
					.HeightOverride(400)
					[
					  SNew(SVerticalBox)
					  + SVerticalBox::Slot()
						[
							SNew(SButton)
						//	.Text(FText::FromString("Button 1"))
							.OnClicked_Lambda([ this ] { GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("Button 1 clicked ")); return FReply::Handled(); })
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
							  SNew(STextBlock)
							  .Text(FText::FromString("trwwww 1"))
							  .Font(FSlateFontInfo("Arial",50)		)
							]
						]
					 + SVerticalBox::Slot()
					  [
						 SNew(SButton)
						 .Text(FText::FromString("Button 2"))
						 .OnClicked_Lambda([ this ] { GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("Button 2 clicked ")); return FReply::Handled(); })
						 .ToolTipText(FText::FromString("Sample text"))
					  ]
					]
				  ]
		];
}



// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "MAD_Widget.h"
#include "LrcHUD.h"
#include "LyricProjectWidgetStyles.h"
#include "LyricWidgetStyle.h"
#include "SScaleBox.h"

void SMAD_Widget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyStyle = &FLyricProjectWidgetStyles::Get().GetWidgetStyle<FLyricWidgetStyle>("Style1");

	const FTextBlockStyle  *const usedtextBlockStyle=&FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("LyricStyleAsset")  ;

	ChildSlot
		  [
            SNew(SScaleBox)
			.Stretch(EStretch::ScaleToFit)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
			[
			  SNew(SBox)
			  .WidthOverride(MyStyle->BackgroundBrush.ImageSize.X)
			  .HeightOverride(MyStyle->BackgroundBrush.ImageSize.Y)
			  [
				SNew(SBorder)
				.BorderImage(&MyStyle->BackgroundBrush)
                .BorderBackgroundColor(FLinearColor(1, 1, 1, 0.5f))
				[
				  SNew(SOverlay)
				  +SOverlay::Slot()
				  .HAlign(HAlign_Center)
				  .VAlign(VAlign_Bottom)
				  .Padding(FMargin(0 ,0,0,50))
				  [
					SNew(STextBlock)
				//	.ColorAndOpacity(FLinearColor::White)
				//	.ShadowColorAndOpacity(FLinearColor::Black)
				//	.ShadowOffset(FIntPoint(-1, 1))
					.TextStyle(usedtextBlockStyle)
					.Font(FSlateFontInfo("Microsoft YaHei UI Light", 20))
					.Text_Raw(this,&SMAD_Widget::GetLyricText)
				  ]
				]
			  ]
			]
		];

}

FText SMAD_Widget::GetLyricText() const
{
	if (OwnerHUD&&OwnerHUD->LyricAsset)
	{
		int32  Index = OwnerHUD->LyricAsset->LineArray.FindLastByPredicate([ this ](const FLyricLine&  TestLine) {return TestLine.StartTime <= OwnerHUD->GetPlayedSeconds(); });
		if (OwnerHUD->LyricAsset->LineArray.IsValidIndex(Index))
		{
			return FText::FromString(OwnerHUD->LyricAsset->LineArray[Index].StringBody);
		}
	}
	return FText::FromString("No Lyric");
}

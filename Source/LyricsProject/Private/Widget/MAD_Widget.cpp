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
						  
	UsedtextBlockStyle=&FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("LyricStyleAsset")  ;

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
					.TextStyle(UsedtextBlockStyle)
					.Font(FSlateFontInfo("Microsoft YaHei UI Light", 20))
					.Text_Raw(this,&SMAD_Widget::GetLyricText)
				  ]
				  +SOverlay::Slot()
				  .HAlign(HAlign_Center)
				  .VAlign(VAlign_Top)
				  .Padding(FMargin(0,50,0,0))
				  [
					SNew(STextBlock)
					.ShadowOffset(UsedtextBlockStyle->ShadowOffset)
					.ColorAndOpacity_Raw(this,&SMAD_Widget::GetTitleColor)
					.Font(FSlateFontInfo("Microsoft YaHei UI Light", 40))
					.Text(InArgs._TitleText)
				  ]
				]
			  ]
			]
		];
	SetupAnimation();
}

void SMAD_Widget::PlayStartAnimation()
{
	BeginPlayAnimation.Play(this->AsShared());
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

void SMAD_Widget::SetupAnimation()
{
	TitleColorCurve= BeginPlayAnimation.AddCurve(0,5,ECurveEaseFunction::CubicInOut)	;
}

FSlateColor SMAD_Widget::GetTitleColor() const
{
	const float LerpValue = TitleColorCurve.GetLerp();
	FLinearColor UsedColor = UsedtextBlockStyle->ColorAndOpacity.GetSpecifiedColor();
	FLinearColor AlphaColor(UsedColor.R, UsedColor.G, UsedColor.B, 0);

	float FadeIn=0.1f;

	float FadeOut=0.9f;

	if (LerpValue <FadeIn)
	{
		return FMath::Lerp(AlphaColor, UsedColor, LerpValue/ FadeIn);
	}

	if (LerpValue < FadeOut)
	{
		return	  UsedtextBlockStyle->ColorAndOpacity;
	}

	return FMath::Lerp(UsedColor, AlphaColor,(LerpValue- FadeOut)/(1-FadeOut));
}

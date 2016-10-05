// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LrcMenuWidget.h"
#include "LrcHUD.h"
#include "LyricProjectWidgetStyles.h"
#include "LyricWidgetStyle.h"
#include "SScaleBox.h"

void SLrcMenuWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	PlayedSeconds=InArgs._PlayedSeconds;
	MyStyle = &FLyricProjectWidgetStyles::Get().GetWidgetStyle<FLyricWidgetStyle>("Style1");

	ChildSlot
	[
		SNew(SScaleBox)
		.Stretch(EStretch::ScaleToFit)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
		//	.WidthOverride(MyStyle->BackgroundBrush.ImageSize.X)
		//	.HeightOverride(MyStyle->BackgroundBrush.ImageSize.Y)
			[
				SNew(SBorder)
			//	.BorderImage(&MyStyle->BackgroundBrush)
				.BorderBackgroundColor(FLinearColor(1, 1, 1, 0.5f))
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Center)
				    .AutoHeight()
					.Padding(FMargin(10, 30))
					[
						SNew(STextBlock)
						.ColorAndOpacity(FLinearColor::White)
						.ShadowColorAndOpacity(FLinearColor::Black)
						.ShadowOffset(FIntPoint(-1, 1))
						.Font(FSlateFontInfo("Arial", 60))
						.Text_Lambda([ this ] {
						float CachedSeconds = PlayedSeconds.Get();
						int32 Min = FMath::TruncToInt(CachedSeconds / 60);
						int32 Second = FMath::TruncToInt(CachedSeconds) % 60;
						int32 MillSecond = FMath::TruncToInt((CachedSeconds - FMath::TruncToInt(CachedSeconds)) * 100);
						return FText::FromString(FString::Printf(TEXT("[%02i:%02i.%02i]"), Min, Second, MillSecond)); })
					]
				   + SVerticalBox::Slot()
					[
						SAssignNew(ContainBox, SScrollBox)
					]
				]
			]
		]
	];

	BuildLyricLine();
}

EActiveTimerReturnType SLrcMenuWidget::UpdateScrollTo(double InCurrentTime, float InDeltaTime)
{
	int32  Index = OwnerHUD->LyricAsset->LineArray.FindLastByPredicate([ this ](const FLyricLine&  TestLine) {return TestLine.StartTime <= PlayedSeconds.Get(); });
	if (Index == INDEX_NONE)
	{
		return EActiveTimerReturnType::Stop;
	}

	check(Lines.IsValidIndex(Index));

	ContainBox->ScrollDescendantIntoView(Lines.IsValidIndex(Index + 3) ? Lines[Index + 3] : Lines.Last());
	
	if (Lines.Num() - 1 == Index)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, TEXT("Now to the End"));
		return EActiveTimerReturnType::Stop;
	}
	else
	{
		return	EActiveTimerReturnType::Continue;
	}
}

void SLrcMenuWidget::BuildLyricLine()
{
   if (ensure(ContainBox.IsValid()) && OwnerHUD&&OwnerHUD->LyricAsset)
   {
	   for (const FLyricLine& OneLine : OwnerHUD->LyricAsset->LineArray)
	   {
		   TSharedPtr<STextBlock>	 OneLineWidget;

		   FVector2D TimeRange(OneLine.StartTime, OneLine.EndTime);

		   ContainBox->AddSlot()
			   .HAlign(HAlign_Center)
			   .VAlign(VAlign_Center)
			   [
				   SAssignNew(OneLineWidget, STextBlock)
				   .Text(FText::FromString(OneLine.StringBody))
				   .Font(FSlateFontInfo("Microsoft YaHei UI Light", 40))
		//		   .Font(MyStyle->FontInfo)
				   .ColorAndOpacity_Lambda([ this, TimeRange ] {return  PlayedSeconds.Get() >= TimeRange.X&&PlayedSeconds.Get() <= TimeRange.Y ? FLinearColor(1, 1, 1, 1) : FLinearColor(1, 1, 1, 0.5); })
			   ];
		   Lines.Add(OneLineWidget.ToSharedRef());
	   }

	   RegisterActiveTimer(0, FWidgetActiveTimerDelegate::CreateSP(this, &SLrcMenuWidget::UpdateScrollTo));
   }
   else
   {
	   UE_LOG(LogTemp, Warning, TEXT("Need valid OwnerHUD and LyricAsset "));
   }
}


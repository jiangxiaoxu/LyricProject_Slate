// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LrcMenuWidget.h"
#include "LrcHUD.h"
#include "LyricProjectWidgetStyles.h"
#include "LyricWidgetStyle.h"

void SLrcMenuWidget::Construct(const FArguments& InArgs)
{
	//bCanTick = false;

	OwnerHUD = InArgs._OwnerHUD;
	PlayedSeconds=InArgs._PlayedSeconds;
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
				.Font(FSlateFontInfo("Arial", 60))
				.Text_Lambda([ this ] { 
					float CachedSeconds = PlayedSeconds.Get();
					int32 Min = FMath::TruncToInt(CachedSeconds / 60);
					int32 Second = FMath::TruncToInt(CachedSeconds) % 60;
					int32 MillSecond = FMath::TruncToInt((CachedSeconds - FMath::TruncToInt(CachedSeconds)) * 100);
					return FText::FromString(FString::Printf(TEXT("[%02i:%02i.%02i]"), Min, Second, MillSecond)); })
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
				[
				   SNew(SBox)
				   .HeightOverride(650)
					[
						SAssignNew(ContainBox, SScrollBox)
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
			   .Font(FSlateFontInfo("Arial", 50))
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


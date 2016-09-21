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

	auto TheStyle = &FLyricProjectWidgetStyles::Get().GetWidgetStyle<FLyricWidgetStyle>("Style1");

	TSharedPtr<SScrollBox>  ContainBox;

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
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
				[
				   SNew(SBox)
				//   .WidthOverride(400)
				   .HeightOverride(200)
					[
						SAssignNew(ContainBox, SScrollBox)
				    ]
				]
		];

	BuildLyricLine(ContainBox.ToSharedRef());

}

void SLrcMenuWidget::BuildLyricLine(TSharedRef<SScrollBox> InPanel)
{
   
   TSharedPtr<SWidget> LastTextWidget;

	for (int32 i = 0; i < 20; i++)
	{
		auto LineWidget = SNew(STextBlock)
			.Text(FText::FromString(FString::Printf(TEXT(" that is  %i line "), i)))
			.Font(FSlateFontInfo("Arial", 50));

		InPanel->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			LineWidget
		];

		if (i==19)
		{
			LastTextWidget= LineWidget;
		}
   }
	

	 RegisterActiveTimer(3,
	  FWidgetActiveTimerDelegate::CreateLambda([ = ](double InCurrentTime, float InDeltaTime)
		{
		InPanel->ScrollDescendantIntoView(LastTextWidget);
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow,TEXT("RegisterActiveTimer"))	  ;
		return EActiveTimerReturnType::Stop;
		}
	)
	);
}


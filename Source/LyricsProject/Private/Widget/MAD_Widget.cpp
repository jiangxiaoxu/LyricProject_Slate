// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "MAD_Widget.h"
#include "LrcHUD.h"
#include "LyricProjectWidgetStyles.h"
#include "LyricWidgetStyle.h"
#include "SScaleBox.h"
#include "SConstraintCanvas.h"
#include "Kismet/KismetMaterialLibrary.h"

void SMAD_Widget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;

	MyStyle = &FLyricProjectWidgetStyles::Get().GetWidgetStyle<FLyricWidgetStyle>("Style1");
//	UsedtextBlockStyle = &FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("LyricStyleAsset");

	DynamicMaterial=UKismetMaterialLibrary::CreateDynamicMaterialInstance(OwnerHUD.Get(),MyStyle->BackGroundMat) ;
	check(DynamicMaterial.IsValid());

	UTexture* UsedTexture = nullptr;
	check(DynamicMaterial->GetTextureParameterValue("UsedTexture", UsedTexture));

	DynamicBrush.ImageSize=FVector2D(UsedTexture->GetSurfaceWidth(),UsedTexture->GetSurfaceHeight()) ;
	DynamicBrush.ImageType= ESlateBrushImageType::FullColor;
	DynamicBrush.SetResourceObject(DynamicMaterial.Get());


	ChildSlot
		  [
           	 SNew(SConstraintCanvas)
			 + SConstraintCanvas::Slot()
			 .AutoSize(true)
			 .Anchors(FAnchors(0,0,1,1))
			[
				SNew(SScaleBox)
				.Stretch(MyStyle->BackGroundStretchType)
				.StretchDirection(MyStyle->BackGroundStretchDirection)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SNew(SImage)
					.Image(&DynamicBrush)
					.ColorAndOpacity(FLinearColor(1, 1, 1, 0.5f))
				]
			]
			+ SConstraintCanvas::Slot()
			.AutoSize(true)
			.Anchors(FAnchors(0, 1, 1, 1))
			.Alignment(FVector2D(0,1))
			[
				
				SNew(SBox)
				.HeightOverride(258)
				.RenderTransform_Raw(this,&SMAD_Widget::GetPanelTransform)
				.RenderTransformPivot(FVector2D(0,0.5f))
				[
					SNew(SConstraintCanvas)
					+SConstraintCanvas::Slot()
					.Anchors(FAnchors(0, 0, 1, 1))
					[
					   SNew(SImage)
					   .ColorAndOpacity(FSlateColor(FLinearColor(0,0,0,0.5f)) )
					]
					+SConstraintCanvas::Slot()
					.Anchors(FAnchors(1, 0, 1,0))
					.Alignment(FVector2D(1,0))
					.Offset(FMargin(-30,20,0,0))
					.AutoSize(true)
					[
						SNew(STextBlock)
						.ColorAndOpacity(FLinearColor::White)
						.ShadowColorAndOpacity(FLinearColor::Black)
						.ShadowOffset(FIntPoint(-1, 1))
						.Font(FSlateFontInfo("Arial", 60))
						.RenderTransform_Raw(this, &SMAD_Widget::GetTimeTransform)
						.RenderTransformPivot(FVector2D(1, 0.5f))
						.Text_Lambda([ this ] {
						float CachedSeconds = OwnerHUD->GetPlayedSeconds();
						int32 Min = FMath::TruncToInt(CachedSeconds / 60);
						int32 Second = FMath::TruncToInt(CachedSeconds) % 60;
					//	int32 MillSecond = FMath::TruncToInt((CachedSeconds - FMath::TruncToInt(CachedSeconds)) * 100);
						return FText::FromString(FString::Printf(TEXT("[%02i:%02i]"), Min, Second)); })
					]
					+SConstraintCanvas::Slot()
					.Anchors(FAnchors(1, 1, 1, 1))
					.Alignment(FVector2D(1, 1))
					.Offset(FMargin(-10, -10, 0, 0))
					.AutoSize(true)
					[
						SNew(STextBlock)
						.ColorAndOpacity(FLinearColor::White)
						.ShadowColorAndOpacity(FLinearColor::Black)
						.ShadowOffset(FIntPoint(-1, 1))
						.Font(FSlateFontInfo("Microsoft YaHei UI Light", 30))
						.RenderTransform_Raw(this, &SMAD_Widget::GetTitleTransform)
						.RenderTransformPivot(FVector2D(1, 0.5f))
						.Text(InArgs._TitleText)
					]
					+SConstraintCanvas::Slot()
					.Anchors(FAnchors(0, 0.5f, 1, 0.5f))
					.Alignment(FVector2D(0, 0))
					.Offset(FMargin(100, 0, 0, 0))
					.AutoSize(true)
					[
						SNew(STextBlock)
						.ColorAndOpacity(FLinearColor::White)
						.ShadowColorAndOpacity(FLinearColor::Black)
						.ShadowOffset(FIntPoint(-1, 1))
						.Font(FSlateFontInfo("Microsoft YaHei UI Light", 30))
						.Text_Raw(this, &SMAD_Widget::GetLyricText)
					]
				]
			]  
			
		  ];
	SetupAnimation();
}

void SMAD_Widget::PlayStartAnimation()
{
	BeginPlayAnimation.Play(this->AsShared());

//	DynamicMaterial->SetScalarParameterValue(TEXT("MainArea"), 1.0f);

//	RegisterActiveTimer(0, FWidgetActiveTimerDelegate::CreateSP(this, &SMAD_Widget::UpdateUVRegion));
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
	PanelTransformCurve=BeginPlayAnimation.AddCurve(2,1,ECurveEaseFunction::CubicOut);

	TimeTransformCurve= BeginPlayAnimation.AddCurve(3.5, 1, ECurveEaseFunction::CubicOut);

	TitleTransformCurve = BeginPlayAnimation.AddCurve(5, 1, ECurveEaseFunction::CubicOut);
}





TOptional<FSlateRenderTransform> SMAD_Widget::GetPanelTransform() const
{
	FSlateRenderTransform  TheTransform(FScale2D(PanelTransformCurve.GetLerp(), 1));

	return TheTransform;
}

TOptional<FSlateRenderTransform> SMAD_Widget::GetTitleTransform() const
{
	FSlateRenderTransform  TheTransform(FScale2D(TitleTransformCurve.GetLerp(), 1));

	return TheTransform;
}

TOptional<FSlateRenderTransform> SMAD_Widget::GetTimeTransform() const
{
	FSlateRenderTransform  TheTransform(FScale2D(TimeTransformCurve.GetLerp(), 1));

	return TheTransform;
}

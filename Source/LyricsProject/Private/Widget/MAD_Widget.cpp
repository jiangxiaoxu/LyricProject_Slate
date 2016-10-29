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
	TitleText=InArgs._TitleText;

	TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	TitleTextSize = FontMeasure->Measure(TitleText.Get(), FSlateFontInfo("Microsoft YaHei UI Light", 30));

	MyStyle = &FLyricProjectWidgetStyles::Get().GetWidgetStyle<FLyricWidgetStyle>("Style1");
//	UsedtextBlockStyle = &FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("LyricStyleAsset");

	{
		BackgroundDynamicMI = UKismetMaterialLibrary::CreateDynamicMaterialInstance(OwnerHUD.Get(), MyStyle->BackGroundMat);
		check(BackgroundDynamicMI.IsValid());

		UTexture* UsedTexture = nullptr;
		check(BackgroundDynamicMI->GetTextureParameterValue("UsedTexture", UsedTexture));

		BackGroundBrush.ImageSize = FVector2D(UsedTexture->GetSurfaceWidth(), UsedTexture->GetSurfaceHeight());
		BackGroundBrush.ImageType = ESlateBrushImageType::FullColor;
		BackGroundBrush.SetResourceObject(BackgroundDynamicMI.Get());
	}

	{
		ProgressDynamicMI = UKismetMaterialLibrary::CreateDynamicMaterialInstance(OwnerHUD.Get(), MyStyle->ProgressMat);
		check(ProgressDynamicMI.IsValid());

		ProgressBrush.ImageSize = FVector2D(200, 200);
		ProgressBrush.ImageType = ESlateBrushImageType::FullColor;
		ProgressBrush.SetResourceObject(ProgressDynamicMI.Get());
	}

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
					.Image(&BackGroundBrush)
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
				.RenderTransformPivot(FVector2D(0,0.5f))
				[
					SNew(SConstraintCanvas)
					+SConstraintCanvas::Slot()
					.Anchors(FAnchors(0, 0, 1, 1))
					[
					   SNew(SImage)
					   .ColorAndOpacity(FSlateColor(FLinearColor(0,0,0,0.5f)) )
						.RenderTransform_Raw(this, &SMAD_Widget::GetPanelImageTransform)
					]
					+SConstraintCanvas::Slot()
					.Alignment(FVector2D(0,0.5f))
					.Anchors(FAnchors(0, 0.5, 0, 0.5))
					.AutoSize(true)
					.Offset(FMargin(50, 0, 0, 0))
					[
						SNew(SImage)
						.Image(&ProgressBrush)
						.RenderTransform_Raw(this, &SMAD_Widget::GetProgressImageTransform)
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
					.Anchors(FAnchors(0.3f, 0.5f, 0.3f, 0.5f))
					.Alignment(FVector2D(0, 0.5f))
					//.Offset(FMargin(100, 0, 0, 0))
					.AutoSize(true)
					[
						SNew(STextBlock)
						.ColorAndOpacity(FLinearColor::White)
						.ShadowColorAndOpacity(FLinearColor::Black)
						.ShadowOffset(FIntPoint(-1, 1))
						.Font(FSlateFontInfo("Microsoft YaHei UI Light", 30))
						.Text_Raw(this, &SMAD_Widget::GetLyricText)
						.Visibility_Raw(this,&SMAD_Widget::GetLyricTextVisibility)
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

void SMAD_Widget::RollbackAnimation()
{
    BeginPlayAnimation.PlayReverse(this->AsShared());
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
	PanelTransformCurve = BeginPlayAnimation.AddCurve(2, 1, ECurveEaseFunction::CubicOut);
	TimeTransformCurve = BeginPlayAnimation.AddCurve(3.5, 0.5, ECurveEaseFunction::CubicOut);
	TitleTransformCurve = BeginPlayAnimation.AddCurve(3.5, 0.5, ECurveEaseFunction::CubicOut);
	ProgressImageTransformCurve = BeginPlayAnimation.AddCurve(2.5, 0.5, ECurveEaseFunction::QuadInOut);

	RegisterActiveTimer(0, FWidgetActiveTimerDelegate::CreateSP(this, &SMAD_Widget::UpdateProgressImage));
}





TOptional<FSlateRenderTransform> SMAD_Widget::GetPanelImageTransform() const
{

	FSlateRenderTransform  TheTransform(FMath::Lerp(FVector2D(-2500, 0), FVector2D::ZeroVector, PanelTransformCurve.GetLerp()));

	return TheTransform;
}

TOptional<FSlateRenderTransform> SMAD_Widget::GetTitleTransform() const
{

	FSlateRenderTransform  TheTransform(FMath::Lerp(FVector2D(100+TitleTextSize.X, 0), FVector2D::ZeroVector, TitleTransformCurve.GetLerp()));

	return TheTransform;
}

TOptional<FSlateRenderTransform> SMAD_Widget::GetProgressImageTransform() const
{
	float Lerp = ProgressImageTransformCurve.GetLerp();

	if (FMath::IsNearlyEqual(Lerp, 0))	  //初始未开始播放时,设置成0以方便隐藏
	{
		return	 FSlateRenderTransform(FScale2D(0));
	}

	float Scale = FMath::Lerp<float>(0.8f, 1.0f, FMath::Abs(Lerp-0.5f)/0.5f);

	return	  FSlateRenderTransform(FScale2D(Scale));
}

EVisibility SMAD_Widget::GetLyricTextVisibility() const
{
	return TimeTransformCurve.GetLerp()>0.95f ? EVisibility::Visible : EVisibility::Hidden;
}

EActiveTimerReturnType SMAD_Widget::UpdateProgressImage(double InCurrentTime, float InDeltaTime)
{
	if (ProgressDynamicMI.IsValid()&&OwnerHUD.IsValid())
	{

		ProgressDynamicMI->SetScalarParameterValue(TEXT("Alpha"), OwnerHUD->GetPlayedProgress())   ;

		return EActiveTimerReturnType::Continue;
	}
	else
	{
		return EActiveTimerReturnType::Stop;
	}
}

TOptional<FSlateRenderTransform> SMAD_Widget::GetTimeTransform() const
{
	FSlateRenderTransform  TheTransform(FMath::Lerp(FVector2D(400, 0), FVector2D::ZeroVector, TimeTransformCurve.GetLerp()));

	return TheTransform;
}

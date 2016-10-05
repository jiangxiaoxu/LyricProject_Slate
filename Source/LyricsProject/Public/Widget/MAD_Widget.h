// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SlateBasics.h"
#include "SlateExtras.h"
/**
 * 
 */
class LYRICSPROJECT_API SMAD_Widget :public SCompoundWidget
{

	SLATE_BEGIN_ARGS(SMAD_Widget):_TitleText(FText::FromString("UnKnow")){}

	SLATE_ARGUMENT(TAutoWeakObjectPtr<class ALrcHUD>, OwnerHUD)


	SLATE_ATTRIBUTE(FText,TitleText)

	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);

	void PlayStartAnimation();
private:

	const struct FTextBlockStyle* UsedtextBlockStyle;
	const struct FLyricWidgetStyle* MyStyle;

	FSlateBrush DynamicBrush;

	TAutoWeakObjectPtr<UMaterialInstanceDynamic>	   DynamicMaterial;

	TAutoWeakObjectPtr<class ALrcHUD> OwnerHUD;

	FText GetLyricText()const;

	void SetupAnimation();

	/*���ƿ�ʼ����ʱ�Ķ���*/
	FCurveSequence BeginPlayAnimation;

	FCurveHandle PanelTransformCurve;

	FCurveHandle TimeTransformCurve;

	FCurveHandle TitleTransformCurve;


	TOptional<FSlateRenderTransform> GetPanelTransform() const;

	TOptional<FSlateRenderTransform> GetTimeTransform() const;

	TOptional<FSlateRenderTransform> GetTitleTransform() const;


};

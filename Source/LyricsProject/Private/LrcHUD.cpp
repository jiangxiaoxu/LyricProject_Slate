// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LrcHUD.h"

#include "StandardWidget.h"
#include "LrcMenuWidget.h"



ALrcHUD::ALrcHUD()
{
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("MyAudioComponent"));
	AudioComp->bAutoActivate = false;
}

void ALrcHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

void ALrcHUD::BeginPlay()
{
	Super::BeginPlay();

	GetOwningPlayerController()->bShowMouseCursor = true;

	if (GEngine&&GEngine->GameViewport)
	{
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(
			SAssignNew(MyWidget, SLrcMenuWidget)
			.OwnerHUD(this)
			.Intext_Lambda([ this ] {return  FText::FromString(FString::Printf(TEXT("%.1f"), GetPlayedSeconds()));  })
			.LyricLineText_UObject(this,&ALrcHUD::GetLyricText)
		));
	}
}

void ALrcHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ALrcHUD::PlaySound()
{
	if (AudioComp&&AudioComp->Sound)
	{
		SoundDuration = AudioComp->Sound->Duration;
		StartTime = GetWorld()->GetTimeSeconds();

		AudioComp->Play();

	}
}

float ALrcHUD::GetPlayedSeconds() const
{
	if (StartTime > 0 && ensure(SoundDuration > 0))
	{
		return  FMath::Min(GetWorld()->GetTimeSeconds() - StartTime, SoundDuration);
	}
	else
	{
		return 0;
	}
}

FText ALrcHUD::GetLyricText() const
{
	if (!LyricAsset)
	{
		return FText::GetEmpty();
	}

	int32  Index= LyricAsset->LineArray.FindLastByPredicate([ this ](const FLyricLine&  TestLine) {return TestLine.StartTime <= GetPlayedSeconds(); });

	return	  LyricAsset->LineArray.IsValidIndex(Index) ? FText::FromString(LyricAsset->LineArray[Index].StringBody) : FText::GetEmpty();
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LrcHUD.h"

#include "StandardWidget.h"
#include "LrcMenuWidget.h"



ALrcHUD::ALrcHUD()
{
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("MyAudioComponent"));
	AudioComp->bAutoActivate = false;

	//ConstructorHelpers::FObjectFinder<USoundCue> MyTestSound (TEXT("SoundWave'/Game/青空のナミダ.青空のナミダ'")) ;

	//if (MyTestSound.Succeeded())
	//{
	//	AudioComp->SetSound(MyTestSound.Object)   ;
	//}

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
			.PlayedSeconds_UObject(this,&ALrcHUD::GetPlayedSeconds)
		));
	}

}

void ALrcHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ALrcHUD::PlaySound()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, TEXT("PlaySound start"));

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

// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LrcHUD.h"

#include "StandardWidget.h"
#include "LrcMenuWidget.h"



ALrcHUD::ALrcHUD()
{
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("MyAudioComponent"));
	AudioComp->bAutoActivate = false;

	ConstructorHelpers::FObjectFinder<USoundBase> MySound(TEXT("SoundWave'/Game/青空のナミダ.青空のナミダ'"));
	ConstructorHelpers::FObjectFinder<ULyricAsset>	 MyLrcAsset(TEXT("LyricAsset'/Game/高桥瞳_-_青空のナミダ.高桥瞳_-_青空のナミダ'"));

	if (MySound.Succeeded())
	{
		AudioComp->SetSound(MySound.Object);
	}

	if (MyLrcAsset.Succeeded())
	{
		LyricAsset = MyLrcAsset.Object;
	}
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

	FTimerHandle  TheHandle;

	GetWorldTimerManager().SetTimer(TheHandle, this, &ALrcHUD::PlaySound, 2);
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


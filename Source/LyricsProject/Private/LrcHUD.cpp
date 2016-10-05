// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LrcHUD.h"

#include "StandardWidget.h"
#include "LrcMenuWidget.h"
#include "VorbisAudioInfo.h"
#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"
#include "LSoundPlayerComp.h"

#include "MAD_Widget.h"
#include "Kismet/KismetMaterialLibrary.h"


ALrcHUD::ALrcHUD()
{
	AudioComp = CreateDefaultSubobject<ULSoundPlayerComp>(TEXT("MyAudioComponent"));
	AudioComp->bAutoActivate = false;

}




void ALrcHUD::BeginPlay()
{
	Super::BeginPlay();

	USoundWave* MySondWave = ImportMusicByFilePath(TEXT("GARNiDELiA - 極楽浄土.ogg"));

	LyricAsset = ImportLyricByFilePath(TEXT("GARNiDELiA - 極楽浄土.lrc"));

	check(MySondWave);

	AudioComp->SetSound(MySondWave);

	GetOwningPlayerController()->bShowMouseCursor = true;

	SAssignNew(MyWidget, SMAD_Widget)
		.OwnerHUD(this)
		.TitleText(FText::FromString(TEXT("GARNiDELiA - 極楽浄土")));

	if (GEngine&&GEngine->GameViewport)
	{
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MyWidget.ToSharedRef()));
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
		StartTime = GetWorld()->GetAudioTimeSeconds();

		AudioComp->Play();

		StaticCastSharedRef<SMAD_Widget>(MyWidget.ToSharedRef())->PlayStartAnimation();
	}
}

float ALrcHUD::GetPlayedSeconds() const
{
	if (StartTime > 0 && ensure(SoundDuration > 0))
	{
		return  FMath::Min(GetWorld()->GetAudioTimeSeconds() - StartTime, SoundDuration);
	}
	else
	{
		return 0;
	}
}

class USoundWave* ALrcHUD::ImportMusicByFilePath(const FString& Filename)
{
	TArray<uint8>  RawFile;

	const	FString Path = FPaths::GameDir() + Filename;
	if (!FFileHelper::LoadFileToArray(RawFile, *Path))
	{
	UE_LOG(LogTemp,Warning,TEXT(" File not exist path:[%s] "), *Path)  ;
		return nullptr;
	}

	USoundWave*  Sound = NewObject<USoundWave>(this);
	check(Sound);

	
	FSoundQualityInfo Info;
	if (!FVorbisAudioInfo().ReadCompressedInfo(RawFile.GetData(), RawFile.Num(), &Info))
	{
		UE_LOG(LogTemp, Warning, TEXT(" File can not read as Ogg "), *Path);
		return nullptr;
	}

	Sound->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
	Sound->NumChannels = Info.NumChannels;
	Sound->Duration = Info.Duration;
	Sound->RawPCMDataSize = Info.SampleDataSize;
	Sound->SampleRate = Info.SampleRate;

	FByteBulkData& BulkData = Sound->CompressedFormatData.GetFormat("ogg");

	BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(BulkData.Realloc(RawFile.Num()), RawFile.GetData(), RawFile.Num());
	BulkData.Unlock();

	return Sound;
}

class ULyricAsset* ALrcHUD::ImportLyricByFilePath(const FString& FileName)
{
	FString LrcString;
	FString Path = FPaths::GameDir() + FileName;

	if (!FFileHelper::LoadFileToString(LrcString, *Path))
	{
		UE_LOG(LogTemp, Warning, TEXT(" File not exist path:[%s] "), *Path);
		return nullptr;
	}

	ULyricAsset* NewAsset = NewObject<ULyricAsset>(this);
	NewAsset->LoadFromString(LrcString);
	return  NewAsset;
}


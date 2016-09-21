// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LTypes.generated.h"


USTRUCT(BlueprintType)
struct LYRICSPROJECT_API FLyricLine
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lyric")
		float StartTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lyric")
		float EndTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lyric")
		FString StringBody;

};


UCLASS(BlueprintType,meta = (DisplayThumbnail = "true"))
class LYRICSPROJECT_API ULyricAsset :public UObject
{
	GENERATED_BODY()

public:

	virtual bool IsAsset()const override { return true; }

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere, Instanced, Category = Reimport)
		class UAssetImportData* AssetImportData;
#endif

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Lyric")
		TArray<FLyricLine>  LineArray;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Lyric")
		FString Title;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Lyric")
		FString Artist;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Lyric")
		float TimeOffset;


	void  LoadFromString(const FString& InStr);

	static const FString FormatString;
};




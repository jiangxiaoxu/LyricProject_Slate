// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LTypes.h"
#include "Regex.h"



void ULyricAsset::LoadFromString(const FString& InStr)
{
	this->Artist = FString();
	this->Title = FString();
	this->LineArray.Reset();

	FRotator().Quaternion().GetAxisX();
//	this->TimeOffset = 0.0f;
	{
		const FRegexPattern myPattern(R"(\[(\d{2}:\d{2}\.\d{2})\].*)");

		FRegexMatcher  MyMatcher(myPattern, InStr);
		while (MyMatcher.FindNext())
		{
			int32 b = MyMatcher.GetMatchBeginning();
			int32 e = MyMatcher.GetMatchEnding();

			FString TempString(InStr.Mid(b, e - b));

			float Secends = 0;
			Secends += FCString::Atoi(*TempString.Mid(1, 2)) * 60;
			Secends += FCString::Atoi(*TempString.Mid(4, 2));
			Secends += FCString::Atoi(*TempString.Mid(7, 2))*0.01f;

			FString TextString = TempString.RightChop(10);
			TextString.Trim();

			if (!TextString.IsEmpty())
			{
				this->LineArray.Add({ Secends,TextString });
			}
		}
	}

	this->LineArray.StableSort([](const FLyricLine& A, const FLyricLine& B) {  return A.StartTime < B.StartTime;  });

}

const FString ULyricAsset::FormatString = TEXT("lrc; Lyric Asset");

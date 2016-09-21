// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LTypes.h"
#include "Regex.h"



void ULyricAsset::LoadFromString(const FString& InStr)
{
	this->Artist = FString();
	this->Title = FString();
	this->LineArray.Reset();


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
				this->LineArray.Add({ Secends,-1,TextString });
			}
		}
	}

	this->LineArray.StableSort([](const FLyricLine& A, const FLyricLine& B) {  return A.StartTime < B.StartTime;  });


	for (int32 i=0;i<LineArray.Num();i++)
	{
	    if (i+1<LineArray.Num())
	    {
			LineArray[i].EndTime= LineArray[i + 1].StartTime;
		}
		else
		{
			LineArray[i].EndTime = LineArray[i].StartTime + 100;
		}

	}

}

const FString ULyricAsset::FormatString = TEXT("lrc; Lyric Asset");

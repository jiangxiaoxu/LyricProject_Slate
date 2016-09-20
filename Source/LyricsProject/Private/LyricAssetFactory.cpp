// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LyricAssetFactory.h"
#include "LTypes.h"
#include "Regex.h"




ULyricAssetFactory::ULyricAssetFactory()
{
	SupportedClass = ULyricAsset::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
	bText = true;
	Formats.Add(ULyricAsset::FormatString);

}



bool ULyricAssetFactory::FactoryCanImport(const FString& Filename)
{
	FString Extension = FPaths::GetExtension(Filename);
	for (const FString& Str : Formats)
	{
		int32  SeparatorIndex;
		Str.FindChar(';', SeparatorIndex);

		if (Str.Left(SeparatorIndex)==Extension)
		{
			return true;
		}
	}
	return false;
}

UObject* ULyricAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{

	ULyricAsset* NewMyAsset = NewObject<ULyricAsset>(InParent, InClass, InName, Flags);
	check(NewMyAsset);

	return 	NewMyAsset;

}

UObject* ULyricAssetFactory::FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
	ULyricAsset* NewMyAsset = NewObject<ULyricAsset>(InParent, InClass, InName, Flags);

	NewMyAsset->LoadFromString(Buffer);

	if (!NewMyAsset->AssetImportData)
	{
		NewMyAsset->AssetImportData = NewObject<UAssetImportData>(NewMyAsset);
	}

	NewMyAsset->AssetImportData->Update(CurrentFilename);

	return NewMyAsset;

}

bool ULyricAssetFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	ULyricAsset* MyAsset = Cast<ULyricAsset>(Obj);

	if (MyAsset&&MyAsset->AssetImportData)
	{
		OutFilenames.Add(MyAsset->AssetImportData->GetFirstFilename());
		return true;
	}
	return false;
}

void ULyricAssetFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	ULyricAsset* MyAsset = Cast<ULyricAsset>(Obj);

	if (MyAsset&&ensure(NewReimportPaths.Num() == 1))
	{
		MyAsset->AssetImportData->Update(NewReimportPaths[0]);
	}

}

EReimportResult::Type ULyricAssetFactory::Reimport(UObject* Obj)
{
	ULyricAsset* MyAsset = Cast<ULyricAsset>(Obj);

	if (!MyAsset)
	{
		return EReimportResult::Failed;
	}

	const FString Filename = MyAsset->AssetImportData->GetFirstFilename();

	if (!Filename.Len() || IFileManager::Get().FileSize(*Filename) == INDEX_NONE)
	{
		return EReimportResult::Failed;
	}

	if (StaticImportObject(MyAsset->GetClass(), MyAsset->GetOuter(), MyAsset->GetFName(), RF_Public | RF_Standalone, *Filename, nullptr, this))
	{
		MyAsset->MarkPackageDirty();

		return EReimportResult::Succeeded;

	}
	return EReimportResult::Failed;
}

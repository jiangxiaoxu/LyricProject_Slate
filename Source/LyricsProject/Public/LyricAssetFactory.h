// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Factories/Factory.h"
#include "EditorReimportHandler.h"
#include "LyricAssetFactory.generated.h"

/**
 *
 */
UCLASS()
class LYRICSPROJECT_API ULyricAssetFactory : public UFactory, public FReimportHandler
{
	GENERATED_BODY()

public:
	ULyricAssetFactory();

	// UFactory
	virtual bool FactoryCanImport(const FString& Filename)override;
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)override;
	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)override;
   // End of UFactory interface

   // FReimportHandler
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	// End of FReimportHandler
};

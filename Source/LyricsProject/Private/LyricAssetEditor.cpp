// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LyricAssetEditor.h"

TSharedRef<FLyricAssetEditor> FLyricAssetEditor::CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UObject* ObjectToEdit, FGetDetailsViewObjects GetDetailsViewObjects /*= FGetDetailsViewObjects()*/)
{

	TSharedRef<FLyricAssetEditor>  NewEditor(new FLyricAssetEditor);

	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(ObjectToEdit);

	NewEditor->InitEditor(Mode, InitToolkitHost, ObjectsToEdit, GetDetailsViewObjects);

	return NewEditor;
}

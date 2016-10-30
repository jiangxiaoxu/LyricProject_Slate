// Copyright 2015 BlackMa9. All Rights Reserved.
#pragma once

#include "Engine.h"
#include "Factories/Factory.h"
#include "PmxImporter.h"
//#include "PmxMaterialImport.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCategoryPMXMaterialImport, Log, All)

//UCLASS()
class UPmxMaterialImport 
	//: public UFactory // public UFbxFactory
{
	//GENERATED_UCLASS_BODY()

	// Begin UFactory Interface
	//virtual bool DoesSupportClass(UClass * Class) override;
	//virtual UClass* ResolveSupportedClass() override;

	UObject * InParent ; //parent
public:
	void InitializeBaseValue(
		UObject * InParentPtr
		)
	{
		InParent = InParentPtr;
	}
	//////////////////////////////////////
	void AssetsCreateTextuer(
		//UObject * InParent,
		//EObjectFlags Flags,
		//FFeedbackContext * Warn,
		FString CurPath ,
		FString filePath,
		TArray<UTexture*> &textureAssetList
		);
	///////////////////////////////////////
	UTexture* ImportTexture(
		//FbxFileTexture* FbxTexture, 
		FString InTextureFileName,
		bool bSetupAsNormalMap
		);
	///////////////////////////////////////
	/*void AssetsCreateUnrealMaterial(
		UObject * InParent,
		FString MaterialName,
		TArray<UMaterialInterface*>& OutMaterials
		);
		*/
	/////////////////////////////////////
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	void CreateUnrealMaterial(
		//UObject * InParent,
		MMD4UE4::PMX_MATERIAL& PmxMaterial,
		TArray<UMaterialInterface*>& OutMaterials,
		TArray<FString>& UVSets,
		TArray<UTexture*> &textureAssetList
		);
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	void FixupMaterial(
		MMD4UE4::PMX_MATERIAL& PmxMaterial,
		UMaterial* UnrealMaterial
		);
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	bool CreateAndLinkExpressionForMaterialProperty(
		MMD4UE4::PMX_MATERIAL& PmxMaterial,
		UMaterial* UnrealMaterial,
		const char* MaterialProperty,
		FExpressionInput& MaterialInput,
		bool bSetupAsNormalMap,
		TArray<FString>& UVSet,
		const FVector2D& Location,
		TArray<UTexture*> &textureAssetList);

	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	bool CreateAndLinkExpressionForMaterialProperty_ForMmdAutoluminus(
		MMD4UE4::PMX_MATERIAL& PmxMaterial,
		UMaterial* UnrealMaterial,
		//const char* MaterialProperty,
		FExpressionInput& MaterialInput,
		//bool bSetupAsNormalMap,
		//TArray<FString>& UVSet,
		const FVector2D& Location,
		TArray<UTexture*> &textureAssetList);
	///////////////////////////////////////

};
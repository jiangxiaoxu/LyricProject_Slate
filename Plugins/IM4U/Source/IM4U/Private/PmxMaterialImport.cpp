// Copyright 2015 BlackMa9. All Rights Reserved.


#include "IM4UPrivatePCH.h"
#include "PmxMaterialImport.h"

#include "UnrealEd.h"
#include "ComponentReregisterContext.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"

#include "PackageTools.h"

#include "ObjectTools.h"


#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialExpressionTextureCoordinate.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionMultiply.h"
#include "Materials/MaterialExpressionConstant.h"

DEFINE_LOG_CATEGORY(LogCategoryPMXMaterialImport)

/*UPmxMaterialImport::UPmxMaterialImport(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//SupportedClass = UMyUObjectTestCls::StaticClass();
	bCreateNew = false;
	////////////
	bEditorImport = false;
	bText = false;

	InParent = 0;
}
*/
void UPmxMaterialImport::AssetsCreateTextuer(
	//UObject * InParent,
	//EObjectFlags Flags,
	//FFeedbackContext * Warn,
	FString CurPath,
	FString filePath,
	TArray<UTexture*> &textureAssetList)
{
	TArray<FString> ImagePaths;
	//FString CurPath = FPaths::GetPath(GetCurrentFilename());
	TMap<FString, UTexture*>* ExistImages = NULL;
	FAssetToolsModule& AssetToolsModule
		= FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	ImagePaths.AddUnique(filePath);

	// texture
	for (int i = 0; i < ImagePaths.Num(); ++i)
	{
		//TODO: ImageBaseDirectory���l��
		FString FileName = CurPath / ImagePaths[i];

		UTexture* ImportedTexture = NULL;
		if (ExistImages && ExistImages->Contains(ImagePaths[i]))
		{
			ImportedTexture = ExistImages->FindChecked(ImagePaths[i]);
		}

		FString TextureName = ("T_" + FPaths::GetBaseFilename(ImagePaths[i]));

		FString TexturePackageName;
		FString BasePackageName 
			= FPackageName::GetLongPackagePath(
				InParent->GetOutermost()->GetName()) / TextureName;


		FString BasePackageName_2 = PackageTools::SanitizePackageName(BasePackageName);

		UTexture* ExistingTexture = NULL;
		// First check if the asset already exists.
		{
			FString ObjectPath = BasePackageName_2 + TEXT(".") + TextureName;
			ExistingTexture = LoadObject<UTexture>(NULL, *ObjectPath);
		}
		if (!ExistingTexture)
		{
		}
		else
		{
			textureAssetList.Add(ExistingTexture);
			continue;
		}

		TArray<uint8> Data;
		if (FFileHelper::LoadFileToArray(Data, *FileName))
		{
#if 0
#if 1 //defined(SS_UE4_4) || defined(SS_UE4_5)
			UTextureFactory* TextureFact
				= new UTextureFactory(FPostConstructInitializeProperties());
#else
			UTextureFactory* TextureFact 
				= new UTextureFactory(FObjectInitializer());
#endif
#else
			//From fbx texture import code
			auto TextureFact = NewObject<UTextureFactory>();
#endif
			TextureFact->AddToRoot();
			// save texture settings if texture exist
			TextureFact->SuppressImportOverwriteDialog();
			UPackage* TexturePackage = NULL;
			if (ImportedTexture)
			{
				TexturePackage = ImportedTexture->GetOutermost();
			}
			else
			{
				AssetToolsModule.Get().CreateUniqueAssetName(
					BasePackageName, TEXT(""), 
					TexturePackageName, TextureName);
				TexturePackage = CreatePackage(NULL, *TexturePackageName);
			}

			const uint8* BufferBegin = Data.GetData();
			const uint8* BufferEnd = BufferBegin + Data.Num();
			UTexture2D* NewTexture = (UTexture2D*)TextureFact->FactoryCreateBinary(
				UTexture2D::StaticClass(),
				TexturePackage,
				FName(*TextureName),
				RF_Standalone | RF_Public,
				NULL,
				*FPaths::GetExtension(ImagePaths[i]),
				BufferBegin, 
				BufferEnd,
				GWarn
				);
			if (NewTexture)
			{
				if (!ImportedTexture)
				{
					NewTexture->MipGenSettings = TMGS_NoMipmaps;
					NewTexture->AddressX = TA_Clamp;
					NewTexture->AddressY = TA_Clamp;
					NewTexture->CompressionSettings = TC_Default;
					NewTexture->LODGroup = TEXTUREGROUP_World;
					//Test
					NewTexture->SRGB = 1;
				}

				FAssetRegistryModule::AssetCreated(NewTexture);
				TexturePackage->SetDirtyFlag(true);

				TextureFact->RemoveFromRoot();

				ImportedTexture = NewTexture;


				UE_LOG(LogCategoryPMXMaterialImport, Warning,
					TEXT("PMX Import NewTexture Complete.[%s]"), *TextureName);

			}
			TextureFact->RemoveFromRoot();
			/**/
		}
		/*
		if (ImportedTexture)
		{
			for (int ii = 0; ii < NewProject->CellmapList.Num(); ++ii)
			{
				if (NewProject->CellmapList[ii].ImagePath == ImagePaths[i])
				{
					NewProject->CellmapList[ii].Texture = ImportedTexture;
				}
			}
		}
		*/
		textureAssetList.Add(ImportedTexture);
	}

}
#if 0
void FPmxMaterialImport::AssetsCreateUnrealMaterial(
	UObject * InParent,
	FString MaterialName,
	TArray<UMaterialInterface*>& OutMaterials)
{
	/// From @ UnFbx::FFbxImporter::CreateUnrealMaterial
	// copy code 

	FString MaterialFullName = /*ANSI_TO_TCHAR*/(("M_" + MaterialName));

	if (MaterialFullName.Len() > 6)
	{
		int32 Offset = MaterialFullName.Find(TEXT("_SKIN"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		if (Offset != INDEX_NONE)
		{
			// Chop off the material name so we are left with the number in _SKINXX
			FString SkinXXNumber = MaterialFullName.Right(MaterialFullName.Len() - (Offset + 1)).RightChop(4);

			if (SkinXXNumber.IsNumeric())
			{
				// remove the '_skinXX' suffix from the material name					
				MaterialFullName = MaterialFullName.LeftChop(Offset + 1);
			}
		}
	}

	//	MaterialFullName = ObjectTools::SanitizeObjectName(MaterialFullName);

	// Make sure we have a parent
	if (!ensure(InParent))
	{
		return;
	}

	FString BasePackageName = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetName()) / MaterialFullName;
	//BasePackageName = PackageTools::SanitizePackageName(BasePackageName);

	// The material could already exist in the project
	FName ObjectPath = *(BasePackageName + TEXT(".") + MaterialFullName);

	/*if (ImportedMaterialData.IsUnique(FbxMaterial, ObjectPath))
	{
	UMaterialInterface* FoundMaterial = ImportedMaterialData.GetUnrealMaterial(FbxMaterial);
	if (FoundMaterial)
	{
	// The material was imported from this FBX.  Reuse it
	OutMaterials.Add(FoundMaterial);
	return;
	}
	}
	else
	{
	UMaterialInterface* FoundMaterial = LoadObject<UMaterialInterface>(NULL, *ObjectPath.ToString());
	// do not override existing materials
	if (FoundMaterial)
	{
	ImportedMaterialData.AddImportedMaterial(FbxMaterial, *FoundMaterial);
	OutMaterials.Add(FoundMaterial);
	return;
	}
	}*/

	const FString Suffix(TEXT(""));
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	FString FinalPackageName;
	AssetToolsModule.Get().CreateUniqueAssetName(BasePackageName, Suffix, FinalPackageName, MaterialFullName);

	UPackage* Package = CreatePackage(NULL, *FinalPackageName);


	// create an unreal material asset
	UMaterialFactoryNew* MaterialFactory = new UMaterialFactoryNew(FObjectInitializer());

	UMaterial* UnrealMaterial = (UMaterial*)MaterialFactory->FactoryCreateNew(
		UMaterial::StaticClass(), Package, *MaterialFullName, RF_Standalone | RF_Public, NULL, GWarn);

	if (UnrealMaterial != NULL)
	{
		// Notify the asset registry
		FAssetRegistryModule::AssetCreated(UnrealMaterial);

		// Set the dirty flag so this package will get saved later
		Package->SetDirtyFlag(true);

		UE_LOG(LogCategoryPMXMaterialImport, Warning, TEXT("PMX Import NewMateriale Complete.[%s]"), *FinalPackageName);

	}
}
#endif

//-----
UTexture* UPmxMaterialImport::ImportTexture(
	//FbxFileTexture* FbxTexture, 
	FString InTextureFileName,
	bool bSetupAsNormalMap
	)
{

	// create an unreal texture asset
	UTexture* UnrealTexture = NULL;
	FString Filename1 = InTextureFileName;// ANSI_TO_TCHAR(FbxTexture->GetFileName());
	FString Extension = FPaths::GetExtension(Filename1).ToLower();
	// name the texture with file name
	FString TextureName = FPaths::GetBaseFilename(Filename1);

	TextureName = ObjectTools::SanitizeObjectName(TextureName);

	// set where to place the textures
	FString BasePackageName
		= FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetName()) / TextureName;
	BasePackageName = PackageTools::SanitizePackageName(BasePackageName);

	UTexture* ExistingTexture = NULL;
	UPackage* TexturePackage = NULL;
	// First check if the asset already exists.
	{
		FString ObjectPath = BasePackageName + TEXT(".") + TextureName;
		ExistingTexture = LoadObject<UTexture>(NULL, *ObjectPath);
	}


	if (!ExistingTexture)
	{
		const FString Suffix(TEXT(""));

		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
		FString FinalPackageName;
		AssetToolsModule.Get().CreateUniqueAssetName(BasePackageName, Suffix, FinalPackageName, TextureName);

		TexturePackage = CreatePackage(NULL, *FinalPackageName);
	}
	else
	{
		TexturePackage = ExistingTexture->GetOutermost();

		return ExistingTexture;
	}


	// try opening from absolute path
	FString Filename = Filename1;
	TArray<uint8> DataBinary;
	if (!FFileHelper::LoadFileToArray(DataBinary, *Filename))
	{
#if 0 //test
		// try fbx file base path + relative path
		FString Filename2 = FileBasePath / ANSI_TO_TCHAR(FbxTexture->GetRelativeFileName());
		Filename = Filename2;
		if (!FFileHelper::LoadFileToArray(DataBinary, *Filename))
		{
			// try fbx file base path + texture file name (no path)
			FString Filename3 = ANSI_TO_TCHAR(FbxTexture->GetRelativeFileName());
			FString FileOnly = FPaths::GetCleanFilename(Filename3);
			Filename3 = FileBasePath / FileOnly;
			Filename = Filename3;
			if (!FFileHelper::LoadFileToArray(DataBinary, *Filename))
			{
				UE_LOG(LogFbxMaterialImport, Warning, TEXT("Unable to find TEXTure file %s. Tried:\n - %s\n - %s\n - %s"), *FileOnly, *Filename1, *Filename2, *Filename3);
			}
		}
#endif
	}
	if (DataBinary.Num()>0)
	{
		UE_LOG(LogCategoryPMXMaterialImport, Verbose, 
			TEXT("Loading texture file %s"), *Filename);
		const uint8* PtrTexture = DataBinary.GetData();
		auto TextureFact = NewObject<UTextureFactory>();
		TextureFact->AddToRoot();

		// save texture settings if texture exist
		TextureFact->SuppressImportOverwriteDialog();
		const TCHAR* TextureType = *Extension;

		// Unless the normal map setting is used during import, 
		//	the user has to manually hit "reimport" then "recompress now" button
		if (bSetupAsNormalMap)
		{
			if (!ExistingTexture)
			{
				TextureFact->LODGroup = TEXTUREGROUP_WorldNormalMap;
				TextureFact->CompressionSettings = TC_Normalmap;
			}
			else
			{
				UE_LOG(LogCategoryPMXMaterialImport, Warning,
					TEXT("Manual texture reimport and recompression may be needed for %s"), *TextureName);
			}
		}

		UnrealTexture = (UTexture*)TextureFact->FactoryCreateBinary(
			UTexture2D::StaticClass(), TexturePackage, *TextureName,
			RF_Standalone | RF_Public, NULL, TextureType,
			PtrTexture, PtrTexture + DataBinary.Num(), GWarn);

		if (UnrealTexture != NULL)
		{
			// Notify the asset registry
			FAssetRegistryModule::AssetCreated(UnrealTexture);

			// Set the dirty flag so this package will get saved later
			TexturePackage->SetDirtyFlag(true);
		}
		TextureFact->RemoveFromRoot();
	}

	return UnrealTexture;
}


//--------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool UPmxMaterialImport::CreateAndLinkExpressionForMaterialProperty(
	MMD4UE4::PMX_MATERIAL& PmxMaterial,
	UMaterial* UnrealMaterial,
	const char* MaterialProperty,
	FExpressionInput& MaterialInput,
	bool bSetupAsNormalMap,
	TArray<FString>& UVSet,
	const FVector2D& Location,
	TArray<UTexture*> &textureAssetList)
{
	bool bCreated = false;
#if 0
	FbxProperty FbxProperty = FbxMaterial.FindProperty(MaterialProperty);
	if (FbxProperty.IsValid())
	{
		int32 LayeredTextureCount = FbxProperty.GetSrcObjectCount<FbxLayeredTexture>();
		if (LayeredTextureCount>0)
		{
			UE_LOG(LogFbxMaterialImport, Warning, TEXT("Layered Textures are not supported (material %s)"), ANSI_TO_TCHAR(FbxMaterial.GetName()));
		}
		else
		{
#endif
#if 1
			int32 TextureCount = PmxMaterial.TextureIndex;//FbxProperty.GetSrcObjectCount<FbxTexture>();
			if (TextureCount >= 0 && TextureCount <textureAssetList.Num())
			{
				//for (int32 TextureIndex = 0; TextureIndex<TextureCount; ++TextureIndex)
				{
					//FbxFileTexture* FbxTexture = FbxProperty.GetSrcObject<FbxFileTexture>(TextureIndex);

					// create an unreal texture asset
					UTexture* UnrealTexture = textureAssetList[TextureCount];//ImportTexture(FbxTexture, bSetupAsNormalMap);

					if (UnrealTexture)
					{
						UnrealMaterial->BlendMode = BLEND_Masked;
						//float ScaleU = FbxTexture->GetScaleU();
						//float ScaleV = FbxTexture->GetScaleV();

						//Multipule
						UMaterialExpressionMultiply* MulExpression
							= NewObject< UMaterialExpressionMultiply >(UnrealMaterial);
						UnrealMaterial->Expressions.Add(MulExpression);
						//UnrealMaterial->BaseColor.Expression = MulExpression;
						MulExpression->MaterialExpressionEditorX = -250;
						MulExpression->MaterialExpressionEditorY = 0;
						MulExpression->bHidePreviewWindow = 0;

						MulExpression->Desc = TEXT("Textuer * Texture alpha -> BaseColor");

						//Multipule
						UMaterialExpressionMultiply* MulExpression_2
							= NewObject< UMaterialExpressionMultiply >(UnrealMaterial);
						UnrealMaterial->Expressions.Add(MulExpression_2);
						//UnrealMaterial->OpacityMask.Expression = MulExpression_2;
						UnrealMaterial->BaseColor.Expression = MulExpression_2;
						MulExpression_2->B.Expression = MulExpression;
						MulExpression_2->MaterialExpressionEditorX = -250;
						MulExpression_2->MaterialExpressionEditorY = 200;
						MulExpression_2->bHidePreviewWindow = 0;

						MulExpression_2->Desc = TEXT("Textuer alpha * Specure Coloer -> OpacityMask");
						//MulExpression->ConstA = 1.0f;
						//MulExpression->ConstB = FresnelBaseReflectFraction_DEPRECATED;

						//MulExpression->A.Connect(SpecularColor_DEPRECATED.OutputIndex, SpecularColor_DEPRECATED.Expression);
						//SpecularColor_DEPRECATED.Connect(0, MulExpression);


						// A
						// and link it to the material 
						UMaterialExpressionTextureSample* UnrealTextureExpression
							= NewObject<UMaterialExpressionTextureSample>(UnrealMaterial);
						UnrealMaterial->Expressions.Add(UnrealTextureExpression);
						//MaterialInput.Expression = UnrealTextureExpression;
						MulExpression->A.Expression = UnrealTextureExpression;
						MulExpression->B.Connect(4, UnrealTextureExpression);
						//MulExpression_2->B.Connect(4, UnrealTextureExpression);
						//MulExpression->B.Expression = UnrealTextureExpression.Outputs[4];
						UnrealMaterial->OpacityMask.Connect(4, UnrealTextureExpression);
						UnrealTextureExpression->Texture = UnrealTexture;
						UnrealTextureExpression->SamplerType = bSetupAsNormalMap ? SAMPLERTYPE_Normal : SAMPLERTYPE_Color;
						UnrealTextureExpression->MaterialExpressionEditorX = -500; //FMath::TruncToInt(Location.X);
						UnrealTextureExpression->MaterialExpressionEditorY = 0;//FMath::TruncToInt(Location.Y);
						UnrealTextureExpression->SamplerSource = SSM_Wrap_WorldGroupSettings;//For minus UV asix MMD(e.g. AnjeraBalz///)

																							 //MulExpression->B.Connect(UnrealTextureExpression->Outputs[4].Expression);

																							 //B 
						UMaterialExpressionVectorParameter* MyColorExpression
							= NewObject<UMaterialExpressionVectorParameter>(UnrealMaterial);
						UnrealMaterial->Expressions.Add(MyColorExpression);
						//UnrealMaterial->BaseColor.Expression = MyColorExpression;
						//MulExpression->B.Expression = MyColorExpression;
						MulExpression_2->A.Expression = MyColorExpression;

						MyColorExpression->DefaultValue.R = PmxMaterial.Diffuse[0];
						MyColorExpression->DefaultValue.G = PmxMaterial.Diffuse[1];
						MyColorExpression->DefaultValue.B = PmxMaterial.Diffuse[2];
						MyColorExpression->DefaultValue.A = PmxMaterial.Diffuse[3];//A
						MyColorExpression->MaterialExpressionEditorX = -500;
						MyColorExpression->MaterialExpressionEditorY = 300;
						MyColorExpression->SetEditableName("DiffuseColor");
						/*
						// add/find UVSet and set it to the texture
						FbxString UVSetName = FbxTexture->UVSet.Get();
						FString LocalUVSetName = ANSI_TO_TCHAR(UVSetName.Buffer());
						int32 SetIndex = UVSet.Find(LocalUVSetName);
						if ((SetIndex != 0 && SetIndex != INDEX_NONE) || ScaleU != 1.0f || ScaleV != 1.0f)
						{
						// Create a texture coord node for the texture sample
						UMaterialExpressionTextureCoordinate* MyCoordExpression = ConstructObject<UMaterialExpressionTextureCoordinate>(UMaterialExpressionTextureCoordinate::StaticClass(), UnrealMaterial);
						UnrealMaterial->Expressions.Add(MyCoordExpression);
						MyCoordExpression->CoordinateIndex = (SetIndex >= 0) ? SetIndex : 0;
						MyCoordExpression->UTiling = ScaleU;
						MyCoordExpression->VTiling = ScaleV;
						UnrealTextureExpression->Coordinates.Expression = MyCoordExpression;
						MyCoordExpression->MaterialExpressionEditorX = FMath::TruncToInt(Location.X - 175);
						MyCoordExpression->MaterialExpressionEditorY = FMath::TruncToInt(Location.Y);

						}

						*/
						bCreated = true;
					}
				}
			}

			if (MaterialInput.Expression)
			{
				TArray<FExpressionOutput> Outputs = MaterialInput.Expression->GetOutputs();
				FExpressionOutput* Output = Outputs.GetData();
				MaterialInput.Mask = Output->Mask;
				MaterialInput.MaskR = Output->MaskR;
				MaterialInput.MaskG = Output->MaskG;
				MaterialInput.MaskB = Output->MaskB;
				MaterialInput.MaskA = Output->MaskA;
			}
#endif
#if 0
		}
	}
#endif
	return bCreated;
}
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void UPmxMaterialImport::FixupMaterial(
	MMD4UE4::PMX_MATERIAL& PmxMaterial,
	UMaterial* UnrealMaterial
	)
{
	// add a basic diffuse color if no texture is linked to diffuse
	if (UnrealMaterial->BaseColor.Expression == NULL)
	{
		UnrealMaterial->BlendMode = BLEND_Masked;
		//FbxDouble3 DiffuseColor;

		UMaterialExpressionVectorParameter* MyColorExpression
			= NewObject<UMaterialExpressionVectorParameter>(UnrealMaterial);
		UnrealMaterial->Expressions.Add(MyColorExpression);
		UnrealMaterial->BaseColor.Expression = MyColorExpression;
		UnrealMaterial->OpacityMask.Connect(4, MyColorExpression);
		MyColorExpression->MaterialExpressionEditorX = -500;
		MyColorExpression->MaterialExpressionEditorY = 00;
		MyColorExpression->SetEditableName("DiffuseColor");

		bool bFoundDiffuseColor = true;
		/*
		if (PmxMaterial.GetClassId().Is(FbxSurfacePhong::ClassId))
		{
		DiffuseColor = ((FbxSurfacePhong&)(PmxMaterial)).Diffuse.Get();
		}
		else if (PmxMaterial.GetClassId().Is(FbxSurfaceLambert::ClassId))
		{
		DiffuseColor = ((FbxSurfaceLambert&)(PmxMaterial)).Diffuse.Get();
		}
		else
		{
		bFoundDiffuseColor = false;
		}*/
		if (bFoundDiffuseColor)
		{
			MyColorExpression->DefaultValue.R = PmxMaterial.Diffuse[0];//R
			MyColorExpression->DefaultValue.G = PmxMaterial.Diffuse[1];//G
			MyColorExpression->DefaultValue.B = PmxMaterial.Diffuse[2];//B
			MyColorExpression->DefaultValue.A = PmxMaterial.Diffuse[3];//A
		}
		else
		{
			// use random color because there may be multiple materials, then they can be different 
			MyColorExpression->DefaultValue.R = 0.5f + (0.5f*FMath::Rand()) / RAND_MAX;
			MyColorExpression->DefaultValue.G = 0.5f + (0.5f*FMath::Rand()) / RAND_MAX;
			MyColorExpression->DefaultValue.B = 0.5f + (0.5f*FMath::Rand()) / RAND_MAX;
		}

		TArray<FExpressionOutput> Outputs = UnrealMaterial->BaseColor.Expression->GetOutputs();
		FExpressionOutput* Output = Outputs.GetData();
		UnrealMaterial->BaseColor.Mask = Output->Mask;
		UnrealMaterial->BaseColor.MaskR = Output->MaskR;
		UnrealMaterial->BaseColor.MaskG = Output->MaskG;
		UnrealMaterial->BaseColor.MaskB = Output->MaskB;
		UnrealMaterial->BaseColor.MaskA = Output->MaskA;
	}

	//////////////////////////
#if 1
	// add a basic diffuse color if no texture is linked to diffuse
	if (UnrealMaterial->AmbientOcclusion.Expression == NULL)
	{
		//FbxDouble3 DiffuseColor;

		UMaterialExpressionVectorParameter* MyColorExpression
			= NewObject<UMaterialExpressionVectorParameter>(UnrealMaterial);
		UnrealMaterial->Expressions.Add(MyColorExpression);
		UnrealMaterial->AmbientOcclusion.Expression = MyColorExpression;
		MyColorExpression->MaterialExpressionEditorX = -500;
		MyColorExpression->MaterialExpressionEditorY = 500;
		MyColorExpression->SetEditableName("AmbientColor");

		bool bFoundDiffuseColor = true;
		/*
		if (PmxMaterial.GetClassId().Is(FbxSurfacePhong::ClassId))
		{
		DiffuseColor = ((FbxSurfacePhong&)(PmxMaterial)).Diffuse.Get();
		}
		else if (PmxMaterial.GetClassId().Is(FbxSurfaceLambert::ClassId))
		{
		DiffuseColor = ((FbxSurfaceLambert&)(PmxMaterial)).Diffuse.Get();
		}
		else
		{
		bFoundDiffuseColor = false;
		}*/
		if (bFoundDiffuseColor)
		{
			MyColorExpression->DefaultValue.R = PmxMaterial.Ambient[0];
			MyColorExpression->DefaultValue.G = PmxMaterial.Ambient[1];
			MyColorExpression->DefaultValue.B = PmxMaterial.Ambient[2];
		}
		else
		{
			// use random color because there may be multiple materials, then they can be different 
			MyColorExpression->DefaultValue.R = 0.5f + (0.5f*FMath::Rand()) / RAND_MAX;
			MyColorExpression->DefaultValue.G = 0.5f + (0.5f*FMath::Rand()) / RAND_MAX;
			MyColorExpression->DefaultValue.B = 0.5f + (0.5f*FMath::Rand()) / RAND_MAX;
		}

		TArray<FExpressionOutput> Outputs = UnrealMaterial->AmbientOcclusion.Expression->GetOutputs();
		FExpressionOutput* Output = Outputs.GetData();
		UnrealMaterial->AmbientOcclusion.Mask = Output->Mask;
		UnrealMaterial->AmbientOcclusion.MaskR = Output->MaskR;
		UnrealMaterial->AmbientOcclusion.MaskG = Output->MaskG;
		UnrealMaterial->AmbientOcclusion.MaskB = Output->MaskB;
		UnrealMaterial->AmbientOcclusion.MaskA = Output->MaskA;
	}
#endif
	//CullingOff//
	UnrealMaterial->TwoSided = PmxMaterial.CullingOff;

	// IF Translucent Opacu?
	//UnrealMaterial->BlendMode = BLEND_Translucent;
	// IF Addtion
	//UnrealMaterial->BlendMode = BLEND_Additive;
}

void UPmxMaterialImport::CreateUnrealMaterial(
	//UObject * InParent,
	MMD4UE4::PMX_MATERIAL& PmxMaterial,
	TArray<UMaterialInterface*>& OutMaterials,
	TArray<FString>& UVSets,
	TArray<UTexture*> &textureAssetList
	)
{
	FString MaterialFullName = "M_" + PmxMaterial.Name;// ANSI_TO_TCHAR(MakeName(PmxMaterial.Name));

	if (MaterialFullName.Len() > 6)
	{
		int32 Offset = MaterialFullName.Find(TEXT("_SKIN"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		if (Offset != INDEX_NONE)
		{
			// Chop off the material name so we are left with the number in _SKINXX
			FString SkinXXNumber = MaterialFullName.Right(MaterialFullName.Len() - (Offset + 1)).RightChop(4);

			if (SkinXXNumber.IsNumeric())
			{
				// remove the '_skinXX' suffix from the material name					
				MaterialFullName = MaterialFullName.LeftChop(Offset + 1);
			}
		}
	}

	MaterialFullName = ObjectTools::SanitizeObjectName(MaterialFullName);

	// Make sure we have a parent
	if (!ensure(InParent))
	{
		return;
	}

	FString BasePackageName = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetName()) / MaterialFullName;
	BasePackageName = PackageTools::SanitizePackageName(BasePackageName);

	// The material could already exist in the project
	FName ObjectPath = *(BasePackageName + TEXT(".") + MaterialFullName);

	/*
	if( ImportedMaterialData.IsUnique( PmxMaterial, ObjectPath ) )
	{
		UMaterialInterface* FoundMaterial = ImportedMaterialData.GetUnrealMaterial( PmxMaterial );
		if (FoundMaterial)
		{
			// The material was imported from this FBX.  Reuse it
			OutMaterials.Add(FoundMaterial);
			return;
		}
	}
	else*/
	{
		UMaterialInterface* FoundMaterial = LoadObject<UMaterialInterface>(NULL, *ObjectPath.ToString());
		// do not override existing materials
		if (FoundMaterial)
		{
			//ImportedMaterialData.AddImportedMaterial( PmxMaterial, *FoundMaterial );
			OutMaterials.Add(FoundMaterial);
			return;
		}
	}
	

	const FString Suffix(TEXT(""));
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	FString FinalPackageName;
	AssetToolsModule.Get().CreateUniqueAssetName(BasePackageName, Suffix, FinalPackageName, MaterialFullName);

	UPackage* Package = CreatePackage(NULL, *FinalPackageName);


	// create an unreal material asset
	auto MaterialFactory = NewObject<UMaterialFactoryNew>();

	UMaterial* UnrealMaterial = (UMaterial*)MaterialFactory->FactoryCreateNew(
		UMaterial::StaticClass(), Package, *MaterialFullName, RF_Standalone | RF_Public, NULL, GWarn);

	if (UnrealMaterial != NULL)
	{
		// Notify the asset registry
		FAssetRegistryModule::AssetCreated(UnrealMaterial);

		// Set the dirty flag so this package will get saved later
		Package->SetDirtyFlag(true);
	}


	// textures and properties
	if (
		CreateAndLinkExpressionForMaterialProperty_ForMmdAutoluminus(
			PmxMaterial,
			UnrealMaterial,
			//NULL,
			UnrealMaterial->BaseColor,
			//false,
			//UVSets,
			FVector2D(240, -320),
			textureAssetList
		) == true)
	{
	}
	else if (
		CreateAndLinkExpressionForMaterialProperty(
			PmxMaterial,
			UnrealMaterial,
			NULL,
			UnrealMaterial->BaseColor,
			false,
			UVSets,
			FVector2D(240, -320),
			textureAssetList
		) == true)
	{

	}
	else
	{

	}
	/*
	CreateAndLinkExpressionForMaterialProperty( PmxMaterial, UnrealMaterial, FbxSurfaceMaterial::sDiffuse, UnrealMaterial->BaseColor, false, UVSets, FVector2D(240,-320) );
	CreateAndLinkExpressionForMaterialProperty( PmxMaterial, UnrealMaterial, FbxSurfaceMaterial::sEmissive, UnrealMaterial->EmissiveColor, false, UVSets, FVector2D(240,-64) );
	CreateAndLinkExpressionForMaterialProperty( PmxMaterial, UnrealMaterial, FbxSurfaceMaterial::sSpecular, UnrealMaterial->Specular, false, UVSets, FVector2D(240, -128) );
	//CreateAndLinkExpressionForMaterialProperty( *FbxMaterial, UnrealMaterial, FbxSurfaceMaterial::sSpecularFactor, UnrealMaterial->SpecularColor, false, UVSets); // SpecularFactor modulates the SpecularColor value if there's one
	//CreateAndLinkExpressionForMaterialProperty( *FbxMaterial, UnrealMaterial, FbxSurfaceMaterial::sShininess, UnrealMaterial->SpecularPower, false, UVSets);
	if (!CreateAndLinkExpressionForMaterialProperty( PmxMaterial, UnrealMaterial, FbxSurfaceMaterial::sNormalMap, UnrealMaterial->Normal, true, UVSets, FVector2D(240,256) ) )
	{
	CreateAndLinkExpressionForMaterialProperty( PmxMaterial, UnrealMaterial, FbxSurfaceMaterial::sBump, UnrealMaterial->Normal, true, UVSets, FVector2D(240,256) ); // no bump in unreal, use as normal map
	}
	//CreateAndLinkExpressionForMaterialProperty( *FbxMaterial, UnrealMaterial, KFbxSurfaceMaterial::sTransparentColor, UnrealMaterial->Opacity, false, UVSets);
	//CreateAndLinkExpressionForMaterialProperty( *FbxMaterial, UnrealMaterial, KFbxSurfaceMaterial::sTransparencyFactor, UnrealMaterial->OpacityMask, false, UVSets);
	*/
	FixupMaterial(PmxMaterial, UnrealMaterial); // add random diffuse if none exists

	// compile shaders for PC (from UPrecompileShadersCommandlet::ProcessMaterial
	// and FMaterialEditor::UpdateOriginalMaterial)

	// make sure that any static meshes, etc using this material will stop using the FMaterialResource of the original 
	// material, and will use the new FMaterialResource created when we make a new UMaterial in place
	FGlobalComponentReregisterContext RecreateComponents;

	// let the material update itself if necessary
	UnrealMaterial->PreEditChange(NULL);
	UnrealMaterial->PostEditChange();

	//ImportedMaterialData.AddImportedMaterial( FbxMaterial, *UnrealMaterial );

	OutMaterials.Add(UnrealMaterial);
}

//--------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool UPmxMaterialImport::CreateAndLinkExpressionForMaterialProperty_ForMmdAutoluminus(
	MMD4UE4::PMX_MATERIAL& PmxMaterial,
	UMaterial* UnrealMaterial,
	//const char* MaterialProperty,
	FExpressionInput& MaterialInput,
	//bool bSetupAsNormalMap,
	//TArray<FString>& UVSet,
	const FVector2D& Location,
	TArray<UTexture*> &textureAssetList)
{
	bool bCreated = false;
#if 1
	int32 TextureCount = PmxMaterial.TextureIndex;//FbxProperty.GetSrcObjectCount<FbxTexture>();
	if (TextureCount >= 0 && TextureCount <textureAssetList.Num())
	{
		//for (int32 TextureIndex = 0; TextureIndex<TextureCount; ++TextureIndex)
		if (PmxMaterial.SpecularPower > 100) //auto luminus
		{
			//FbxFileTexture* FbxTexture = FbxProperty.GetSrcObject<FbxFileTexture>(TextureIndex);

			// create an unreal texture asset
			UTexture* UnrealTexture = textureAssetList[TextureCount];//ImportTexture(FbxTexture, bSetupAsNormalMap);

			if (UnrealTexture)
			{
				UnrealMaterial->BlendMode = BLEND_Additive;
				//float ScaleU = FbxTexture->GetScaleU();
				//float ScaleV = FbxTexture->GetScaleV();

				//Multipule
				UMaterialExpressionMultiply* MulExpression
					= NewObject< UMaterialExpressionMultiply >(UnrealMaterial);
				UnrealMaterial->Expressions.Add(MulExpression);
				UnrealMaterial->BaseColor.Expression = MulExpression; //test
				MulExpression->MaterialExpressionEditorX = -250;
				MulExpression->MaterialExpressionEditorY = 0;
				MulExpression->bHidePreviewWindow = 0;

				MulExpression->Desc = TEXT("Textuer * Texture alpha -> BaseColor");

				//Multipule
				UMaterialExpressionMultiply* MulExpression_2
					= NewObject< UMaterialExpressionMultiply >(UnrealMaterial);
				UnrealMaterial->Expressions.Add(MulExpression_2);
				//UnrealMaterial->OpacityMask.Expression = MulExpression_2;
				MulExpression_2->MaterialExpressionEditorX = -250;
				MulExpression_2->MaterialExpressionEditorY = 200;
				MulExpression_2->bHidePreviewWindow = 0;

				MulExpression_2->Desc = TEXT("Textuer alpha * Specure Coloer -> OpacityMask");
				//MulExpression->ConstA = 1.0f;
				//MulExpression->ConstB = FresnelBaseReflectFraction_DEPRECATED;

				//MulExpression->A.Connect(SpecularColor_DEPRECATED.OutputIndex, SpecularColor_DEPRECATED.Expression);
				//SpecularColor_DEPRECATED.Connect(0, MulExpression);


				//Multipule
				UMaterialExpressionMultiply* MulExpression_3
					= NewObject< UMaterialExpressionMultiply >( UnrealMaterial);
				UnrealMaterial->Expressions.Add(MulExpression_3);
				//UnrealMaterial->EmissiveColor.Expression = MulExpression_3; //TES: lighting EmmisicveColor For AutoLuminous 
				MulExpression_3->MaterialExpressionEditorX = -250;
				MulExpression_3->MaterialExpressionEditorY = 400;
				MulExpression_3->bHidePreviewWindow = 0;
				MulExpression_3->A.Expression = MulExpression_2;

				MulExpression_3->Desc = TEXT("Textuer alpha * Specure Coloer -> OpacityMask");
				//MulExpression->ConstA = 1.0f;
				//MulExpression->ConstB = FresnelBaseReflectFraction_DEPRECATED;

				//MulExpression->A.Connect(SpecularColor_DEPRECATED.OutputIndex, SpecularColor_DEPRECATED.Expression);
				//SpecularColor_DEPRECATED.Connect(0, MulExpression);

				// A
				// and link it to the material 
				UMaterialExpressionTextureSample* UnrealTextureExpression
					= NewObject<UMaterialExpressionTextureSample>(UnrealMaterial);
				UnrealMaterial->Expressions.Add(UnrealTextureExpression);
				//MaterialInput.Expression = UnrealTextureExpression;
				MulExpression->A.Expression = UnrealTextureExpression;
				MulExpression->B.Connect(4, UnrealTextureExpression);
				MulExpression_2->B.Connect(4, UnrealTextureExpression);
				UnrealMaterial->OpacityMask.Connect(4, UnrealTextureExpression);//TEST: Non Light EmmisciveColor For Easy AutoLuminous
				//MulExpression->B.Expression = UnrealTextureExpression.Outputs[4];
				UnrealTextureExpression->Texture = UnrealTexture;
				UnrealTextureExpression->SamplerType = /*bSetupAsNormalMap ? SAMPLERTYPE_Normal :*/ SAMPLERTYPE_Color;
				UnrealTextureExpression->MaterialExpressionEditorX = -500; //FMath::TruncToInt(Location.X);
				UnrealTextureExpression->MaterialExpressionEditorY = 0;//FMath::TruncToInt(Location.Y);
				UnrealTextureExpression->SamplerSource = SSM_Wrap_WorldGroupSettings;//For minus UV asix MMD(e.g. AnjeraBalz///)

				//MulExpression->B.Connect(UnrealTextureExpression->Outputs[4].Expression);

				//B 
				UMaterialExpressionVectorParameter* MyColorExpression
					= NewObject<UMaterialExpressionVectorParameter>(UnrealMaterial);
				UnrealMaterial->Expressions.Add(MyColorExpression);
				//UnrealMaterial->BaseColor.Expression = MyColorExpression;
				//MulExpression->B.Expression = MyColorExpression;
				MulExpression_2->A.Expression = MyColorExpression;

				MyColorExpression->DefaultValue.R = PmxMaterial.Diffuse[0];
				MyColorExpression->DefaultValue.G = PmxMaterial.Diffuse[1];
				MyColorExpression->DefaultValue.B = PmxMaterial.Diffuse[2];
				MyColorExpression->DefaultValue.A = PmxMaterial.Diffuse[3];//A
				MyColorExpression->MaterialExpressionEditorX = -500;
				MyColorExpression->MaterialExpressionEditorY = 300; 

				//const 
				UMaterialExpressionConstant* MyConstExpression
					= NewObject<UMaterialExpressionConstant>(UnrealMaterial);
				UnrealMaterial->Expressions.Add(MyConstExpression);
				//UnrealMaterial->BaseColor.Expression = MyColorExpression;
				//MulExpression->B.Expression = MyColorExpression;
				MulExpression_3->B.Expression = MyConstExpression;

				MyConstExpression->R = PmxMaterial.SpecularPower - 100.;
				MyConstExpression->MaterialExpressionEditorX = -500;
				MyConstExpression->MaterialExpressionEditorY = 500;

				/*
				// add/find UVSet and set it to the texture
				FbxString UVSetName = FbxTexture->UVSet.Get();
				FString LocalUVSetName = ANSI_TO_TCHAR(UVSetName.Buffer());
				int32 SetIndex = UVSet.Find(LocalUVSetName);
				if ((SetIndex != 0 && SetIndex != INDEX_NONE) || ScaleU != 1.0f || ScaleV != 1.0f)
				{
				// Create a texture coord node for the texture sample
				UMaterialExpressionTextureCoordinate* MyCoordExpression = ConstructObject<UMaterialExpressionTextureCoordinate>(UMaterialExpressionTextureCoordinate::StaticClass(), UnrealMaterial);
				UnrealMaterial->Expressions.Add(MyCoordExpression);
				MyCoordExpression->CoordinateIndex = (SetIndex >= 0) ? SetIndex : 0;
				MyCoordExpression->UTiling = ScaleU;
				MyCoordExpression->VTiling = ScaleV;
				UnrealTextureExpression->Coordinates.Expression = MyCoordExpression;
				MyCoordExpression->MaterialExpressionEditorX = FMath::TruncToInt(Location.X - 175);
				MyCoordExpression->MaterialExpressionEditorY = FMath::TruncToInt(Location.Y);

				}

				*/
				bCreated = true;
			}
		}
	}

	if (MaterialInput.Expression)
	{
		TArray<FExpressionOutput> Outputs = MaterialInput.Expression->GetOutputs();
		FExpressionOutput* Output = Outputs.GetData();
		MaterialInput.Mask = Output->Mask;
		MaterialInput.MaskR = Output->MaskR;
		MaterialInput.MaskG = Output->MaskG;
		MaterialInput.MaskB = Output->MaskB;
		MaterialInput.MaskA = Output->MaskA;
	}
#endif
	return bCreated;
}
//-------------------------------------------------------------------------
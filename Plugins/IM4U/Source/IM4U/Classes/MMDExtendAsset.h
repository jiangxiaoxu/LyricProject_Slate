// Copyright 2015 BlackMa9. All Rights Reserved.
#pragma once

#include "Engine.h"
#include "Factories/Factory.h"
//#include "Factories/FbxFactory.h"
#include "UnrealEd.h"
#include "Factories.h"
#include "BusyCursor.h"
#include "SSkeletonWidget.h"
#include "SkelImport.h"


#include "MMDExtendAsset.generated.h"



/**********************
* MMD Extend Info : Ik info
***********************/
// ÇhÇjÉäÉìÉNèÓïÒ
USTRUCT()
struct FMMD_IKLINK
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "ue4 link bone index"))
	int32		BoneIndex;							// Link bone index ( for skeleton bone index ,use ik func)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "ue4 link bone name") )
	FName	BoneName;							// Link Bone Name

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "If enabled, rotation limite, 0:OFF, 1:ON "))
	uint32	RotLockFlag:1;						// âÒì]êßå¿( 0:OFF 1:ON )

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "Rotation angle limit Euler[dig:-180~180] min"))
	FVector	RotLockMin;							// âÒì]êßå¿ÅAâ∫å¿[x,y,z]

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "Rotation angle limit Euler[dig:-180~180] max"))
	FVector	RotLockMax;							// âÒì]êßå¿ÅAè„å¿[x,y,z]

	FMMD_IKLINK()
	{
		BoneIndex = -1;
		RotLockFlag = 0;
	};
};

// ÇhÇjèÓïÒ
USTRUCT()
struct FMMD_IKInfo
{
	GENERATED_USTRUCT_BODY()

	// IKåvéZâ¬î\ÉtÉâÉO
	bool	checkIKIndex;

	int		IKBoneIndexVMDKey;			// IK target bone index vmd key index

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "ue4 IK bone index"))
	int32		IKBoneIndex;					// IK target bone index ( use ik func. ref skeleton.)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "ue4 IK bone name"))
	FName	IKBoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "ue4 target bone index"))
	int32		TargetBoneIndex;					// IK target bone index ( use ik func. ref skeleton.)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "ue4 target bone name"))
	FName	TargetBoneName;						// IK Target Bone Name 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "CCD-IK loop count"))
	int32		LoopNum;							// IKåvéZÇÃÉãÅ[ÉvâÒêî

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "CCD-IK unit angle[dig]"))
	float	RotLimit;							// åvéZàÍâÒï”ÇËÇÃêßå¿äpìx

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "CCD-IK link IK info"))
	TArray<FMMD_IKLINK> ikLinkList;				// ÇhÇjÉäÉìÉNèÓïÒ

	FMMD_IKInfo()
	{
		checkIKIndex = false;
		IKBoneIndexVMDKey = -1;
		IKBoneIndex = -1;
		TargetBoneIndex = -1;
		LoopNum = 0;
		RotLimit = 0;
	};
};
/**************************************************************/

/**********************
* MMD Extend Info : Asset class
***********************/
//UCLASS(hidecategories = Object)
UCLASS()
class UMMDExtendAsset : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	////////////////////////////////////
	// mmd target model name
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Header)
		FString ModelName;
	// mmd model comment 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Header, meta = (MultiLine = "true"))
		FText ModelComment;

	////////////////////////////////////
	// MMD-IK-Info is used to generate the AnimSequence form VMD file.
	UPROPERTY(EditAnywhere, Category = IK)
		TArray<FMMD_IKInfo> IkInfoList;	
	
	//TBD::ópìrïsñæ
	//	bool CanEditChange( const UProperty* InProperty ) const;
private:

};
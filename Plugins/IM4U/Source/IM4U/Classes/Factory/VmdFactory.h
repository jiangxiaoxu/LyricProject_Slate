
// Copyright 2015 BlackMa9. All Rights Reserved.
#pragma once


#include "Engine.h"

#include "Factories/Factory.h"
#include "Factories.h"


#include "SkelImport.h"
#include "AnimationUtils.h"

#include "PmxImportUI.h"

#include "VmdImporter.h"

#include "MMD2UE4NameTableRow.h"
#include "MMDExtendAsset.h"

#include "Matinee/InterpTrackMoveAxis.h"

#include "VmdFactory.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMMD4UE4_VMDFactory, Log, All)

UCLASS()
class IM4U_API UVmdFactory : public UFactory 
{
	GENERATED_UCLASS_BODY()

	// Begin UFactory Interface
	virtual void PostInitProperties() override;
	virtual bool DoesSupportClass(UClass * Class) override;
	virtual UClass* ResolveSupportedClass() override;
	virtual UObject* FactoryCreateBinary(
		UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		UObject* Context,
		const TCHAR* Type,
		const uint8*& Buffer,
		const uint8* BufferEnd,
		FFeedbackContext* Warn,
		bool& bOutOperationCanceled
		) override;
	// End UFactory Interface

	/**********************
	* Create AnimSequence from VMD data.(�V�K�쐬�p�e�֐�)
	***********************/
	UAnimSequence * ImportAnimations(
		USkeleton* Skeleton, 
		UObject* Outer,
		const FString& Name, 
		//UFbxAnimSequenceImportData* TemplateImportData, 
		//TArray<FbxNode*>& NodeArray
		UDataTable* ReNameTable,
		UMMDExtendAsset* mmdExtend,
		MMD4UE4::VmdMotionInfo* vmdMotionInfo
		);
	//////////////
	class UPmxImportUI* ImportUI;
	/**********
	* MMD�����x�W�F�Ȑ��̎Z�o����
	***********/
	float interpolateBezier(float x1, float y1, float  x2, float y2, float x);
	/*******************
	* ����AnimSequ�̃A�Z�b�g��VMD�̕\��f�[�^��ǉ����鏈��
	* MMD4Mecanimu�Ƃ̑������p�����e�X�g�@�\
	**********************/
	UAnimSequence * AddtionalMorphCurveImportToAnimations(
		UAnimSequence* exsistAnimSequ,
		UDataTable* ReNameTable,
		MMD4UE4::VmdMotionInfo* vmdMotionInfo
		);
	/*******************
	* Import Morph Target AnimCurve
	* VMD�t�@�C���̃f�[�^����Morphtarget��FloatCurve��AnimSeq�Ɏ�荞��
	**********************/
	bool ImportMorphCurveToAnimSequence(
		UAnimSequence* DestSeq,
		USkeleton* Skeleton,
		UDataTable* ReNameTable,
		MMD4UE4::VmdMotionInfo* vmdMotionInfo
		);
	/*******************
	* Import VMD Animation
	* VMD�t�@�C���̃f�[�^���烂�[�V�����f�[�^��AnimSeq�Ɏ�荞��
	**********************/
	bool ImportVMDToAnimSequence(
		UAnimSequence* DestSeq,
		USkeleton* Skeleton,
		UDataTable* ReNameTable,
		UMMDExtendAsset* mmdExtend,
		MMD4UE4::VmdMotionInfo* vmdMotionInfo
		);

	/*****************
	* MMD���̖��̂���TableRow��UE�����̂��������擾����
	* Return :T is Found
	* @param :ue4Name is Found Row Name
	****************/
	bool FindTableRowMMD2UEName(
		UDataTable* ReNameTable,
		FName mmdName,
		FName * ue4Name
		);
	/*****************
	* Bone���̂���RefSkelton�ň�v����BoneIndex���������擾����
	* Return :index, -1 is not found
	* @param :TargetName is Target Bone Name
	****************/
	int32 FindRefBoneInfoIndexFromBoneName(
		const FReferenceSkeleton & RefSkelton,
		const FName & TargetName
		);
	/*****************
	* ���݂̃L�[�ɂ�����w��Bone��Glb���W���ċA�I�ɎZ�o����
	* Return :trncform
	* @param :TargetName is Target Bone Name
	****************/
	FTransform CalcGlbTransformFromBoneIndex(
		UAnimSequence* DestSeq,
		USkeleton* Skeleton,
		int32 BoneIndex,
		int32 keyIndex
		);
	////////////////////////////////////////

	// VMD -> Matinee actor test
	bool ImportMatineeSequence(
		AMatineeActor* InMatineeActor,
		MMD4UE4::VmdMotionInfo* vmdMotionInfo
		);
	void ImportCamera(
		ACameraActor* Actor,
		UInterpGroupInst* MatineeGroup,
		MMD4UE4::VmdCameraTrackList* Camera
		);
	void ImportAnimatedProperty(
		float* Value,
		const TCHAR* ValueName,
		UInterpGroupInst* MatineeGroup,
		const float FbxValue,
		//FbxProperty InProperty,
		bool bImportFOV,
		MMD4UE4::VmdCameraTrackList * Camera
		);
	UInterpGroupInst* CreateMatineeGroup(
		AMatineeActor* InMatineeActor,
		AActor* Actor,
		FString GroupName
		);
	/**
	* Imports a FBX scene node into a Matinee actor group.
	*/
	float ImportMatineeActor(
		MMD4UE4::VmdCameraTrackList* CameraNode,
		//FbxNode* Node,
		//��L�ɊY������f�[�^�𒲍����邱��
		UInterpGroupInst* MatineeGroup
		);
	//
	void ConsolidateMovementTrackInterpModes(
		UInterpTrackMove* MovementTrack
		);
	//���̊֐��̖߂�l��MMD�̃J�[�u�����ɍ��킹�ČŒ�l��ς�������悤�ɏC������
	EInterpCurveMode GetUnrealInterpMode(
		//FbxAnimCurveKey FbxKey
		);
	//���̊֐����K�v���s���BSubTrack�̑��݂ɂ��Ē������K�v�B
	void ImportMoveSubTrack(
		//FbxAnimCurve* FbxCurve,
		int32 FbxDimension,
		UInterpTrackMoveAxis* SubTrack,
		int32 CurveIndex,
		bool bNegative,
		//FbxAnimCurve* RealCurve,
		float DefaultVal
		);
	//���̊֐����K�v���s���BMatineeAnimated���ǂ̕����ɊY�����邩�ɂ��Ē������K�v�B
	void ImportMatineeAnimated(
		MMD4UE4::VmdCameraTrackList * VmdCurve,
		//FbxAnimCurve* FbxCurve,
		FInterpCurveVector& Curve,
		int32 CurveIndex,
		bool bNegative,
		//FbxAnimCurve* RealCurve,
		float DefaultVal
		);
	//base flame rate [ms]
	float baseFrameRate;
};


// Copyright 2015 BlackMa9. All Rights Reserved.
#pragma once


#include "Engine.h"
#include "MMDImportHelper.h"


// Copy From DxLib DxModelLoaderVMD.h
// DX Library Copyright (C) 2001-2008 Takumi Yamada.

//MMD�̃��[�V�����f�[�^(VMD)�`���@�߂�2�@(�\���EIK)
//http://blog.goo.ne.jp/torisu_tetosuki/e/2a2cb5c2de7563c5e6f20b19e1fe6139

//#define uint8 (unsigned char)

namespace MMD4UE4
{

	struct VMD_HEADER
	{
		char header[30];	// Vocaloid Motion Data 0002
		char modelName[20];	// Target Model Name
	};

#define D_VMD_KEY_BEZIER_AR_0_BEZ_0		0		/* [AR0;BEZ-id][AR1:xy][AR1:XYZ R]**/
#define D_VMD_KEY_BEZIER_AR_0_BEZ_1		1		/* [AR0;BEZ-id][AR1:xy][AR1:XYZ R]**/
#define D_VMD_KEY_BEZIER_AR_1_BEZ_X		0		/* [AR0;BEZ-id][AR1:xy][AR1:XYZ R]**/
#define D_VMD_KEY_BEZIER_AR_1_BEZ_Y		1		/* [AR0;BEZ-id][AR1:xy][AR1:XYZ R]**/
#define D_VMD_KEY_BEZIER_AR_2_KND_X		0		/* [AR0;BEZ-id][AR1:xy][AR1:XYZ R]**/
#define D_VMD_KEY_BEZIER_AR_2_KND_Y		1		/* [AR0;BEZ-id][AR1:xy][AR1:XYZ R]**/
#define D_VMD_KEY_BEZIER_AR_2_KND_Z		2		/* [AR0;BEZ-id][AR1:xy][AR1:XYZ R]**/
#define D_VMD_KEY_BEZIER_AR_2_KND_R		3		/* [AR0;BEZ-id][AR1:xy][AR1:XYZ R]**/
	// VMD�L�[�f�[�^( 111byte )
	struct VMD_KEY
	{
		//uint8	Data[111];
		
		char	Name[ 15 ] ;						// ���O
		uint32	Frame ;								// �t���[��
		float	Position[ 3 ] ;						// ���W
		float	Quaternion[ 4 ] ;					// �N�H�[�^�j�I��
		/*float	PosXBezier[ 4 ] ;					// ���W�w�p�x�W�F�Ȑ����
		float	PosYBezier[ 4 ] ;					// ���W�x�p�x�W�F�Ȑ����
		float	PosZBezier[ 4 ] ;					// ���W�y�p�x�W�F�Ȑ����
		float	RotBezier[4 ] ;					// ��]�p�x�W�F�Ȑ����
		*/
		uint8	Bezier[2][2][4];					// [id] [xy][XYZ R]

	};

	// VMD�\��L�[�f�[�^( 23byte )
	struct VMD_FACE_KEY
	{
		/*uint8	Data[23];						// �f�[�^
		*/
		char	Name[ 15 ] ;						// �\�
		uint32	Frame ;								// �t���[��
		float	Factor ;							// �u�����h��
		
	};

#define D_VMD_CAMERA_INTERPOLATION_AR_0_X		0	/*Interpolation[AR0-6:X,Y,Z,Rot,Len,View][AR1-2:X,Y][AR2-2:P1,P2]*/
#define D_VMD_CAMERA_INTERPOLATION_AR_0_Y		1	/*Interpolation[AR0-6:X,Y,Z,Rot,Len,View][AR1-2:X,Y][AR2-2:P1,P2]*/
#define D_VMD_CAMERA_INTERPOLATION_AR_0_Z		2	/*Interpolation[AR0-6:X,Y,Z,Rot,Len,View][AR1-2:X,Y][AR2-2:P1,P2]*/
#define D_VMD_CAMERA_INTERPOLATION_AR_0_ROT		3	/*Interpolation[AR0-6:X,Y,Z,Rot,Len,View][AR1-2:X,Y][AR2-2:P1,P2]*/
#define D_VMD_CAMERA_INTERPOLATION_AR_0_LEN		4	/*Interpolation[AR0-6:X,Y,Z,Rot,Len,View][AR1-2:X,Y][AR2-2:P1,P2]*/
#define D_VMD_CAMERA_INTERPOLATION_AR_0_VIEW	5	/*Interpolation[AR0-6:X,Y,Z,Rot,Len,View][AR1-2:X,Y][AR2-2:P1,P2]*/
#define D_VMD_CAMERA_INTERPOLATION_AR_1_X		0	/*Interpolation[AR0-6:X,Y,Z,Rot,Len,View][AR1-2:X,Y][AR2-2:P1,P2]*/
#define D_VMD_CAMERA_INTERPOLATION_AR_1_Y		1	/*Interpolation[AR0-6:X,Y,Z,Rot,Len,View][AR1-2:X,Y][AR2-2:P1,P2]*/
#define D_VMD_CAMERA_INTERPOLATION_AR_2_X		0	/*Interpolation[AR0-6:X,Y,Z,Rot,Len,View][AR1-2:X,Y][AR2-2:P1,P2]*/
#define D_VMD_CAMERA_INTERPOLATION_AR_2_Y		1	/*Interpolation[AR0-6:X,Y,Z,Rot,Len,View][AR1-2:X,Y][AR2-2:P1,P2]*/
	// VMD�J�����L�[�f�[�^
	struct VMD_CAMERA
	{
		/*uint8	Data[61];						// �f�[�^
		*/
		uint32	Frame;								//  4:  0:�t���[���ԍ�
		float	Length;								//  8:  4: -(����)
		float	Location[3];						// 20:  8:�ʒu
		float	Rotate[3];							// 32: 20:�I�C���[�p // X���͕��������]���Ă���̂Œ���
		uint8	Interpolation[6][2][2];				// 56: 32:��ԏ�� // [6:X,Y,Z,Rot,Len,View][2:X,Y][2:P1,P2]
		uint32	ViewingAngle;						// 60: 56:����
		uint8	Perspective;						// 61: 60:�ˉe�J�������ǂ��� 0:�ˉe�J���� 1:���ˉe�J����
		
	};
	// VMD �Ɩ�
	struct VMD_LIGHT
	{
		uint32	flameNo;
		float	RGB[3];
		float	Loc[3];
	};
	// VMD�Z���t�V���h�[
	struct VMD_SELFSHADOW{
		uint32	FlameNo;
		uint8	Mode;
		float	Distance;
	};
	////////////////////////////////////////////////////
	// mmd 7.40�ȍ~�ō쐬���ꂽVMD�̏ꍇ�ȉ��̊g���f�[�^������
	// �Q�l�FMMD�̃��[�V�����f�[�^(VMD)�`���@�߂�2�@(�\���EIK)
	///////////////////////////////////////////////////
	// �\���EIK�f�[�^
	//int dataCount; // �f�[�^��
	//VMD_VISIBLE_IK_DATA data[dataCount];
	////////////
	// IK�f�[�^�p�\����
	typedef struct _VMD_IK_DATA
	{
		char ikBoneName[20]; // IK�{�[����
		unsigned char ikEnabled; // IK�L�� // 0:off 1:on
	} VMD_IK_DATA;
	// �\���EIK�f�[�^�p�\����
	typedef struct _VMD_VISIBLE_IK_DATA
	{
		int frameNo; // �t���[���ԍ�
		unsigned char visible; // �\�� // 0:off 1:on
		int ikCount; // IK��
		TArray<VMD_IK_DATA> ikData; // ikData[ikCount]; // IK�f�[�^���X�g
	} VMD_VISIBLE_IK_DATA;
	////////////////////////////////////////////////////

	//VMD Motion Data for Read-
	struct VmdReadMotionData
	{
		VMD_HEADER				vmdHeader;
		//Bone
		int32					vmdKeyCount;
		TArray<VMD_KEY>			vmdKeyList;
		//Skin
		int32					vmdFaceCount;
		TArray<VMD_FACE_KEY>	vmdFaceList;
		//camera 
		int32					vmdCameraCount;
		TArray<VMD_CAMERA>		vmdCameraList;
		//Light
		int32					vmdLightCount;
		TArray<VMD_LIGHT>		vmdLightList;
		//Self Shadow
		int32					vmdSelfShadowCount;
		TArray<VMD_SELFSHADOW>	vmdSelfShadowList;
		//extend info, ik visible
		int32					vmdExtIKCount;
		TArray<VMD_VISIBLE_IK_DATA>	vmdExtIKList;
	};

	struct VmdKeyTrackList
	{
		// Track Name
		FString					TrackName;
		// min Frame
		uint32					minFrameCount;
		// max Frame
		uint32					maxFrameCount;
		// Sorted Key Frame Data List
		TArray<VMD_KEY>			keyList;
		// sort frame num
		TArray<int32>			sortIndexList;
	};
	struct VmdFaceTrackList
	{
		// Track Name
		FString					TrackName;
		// min Frame
		uint32					minFrameCount;
		// max Frame
		uint32					maxFrameCount;
		// Sorted Key Frame Data List
		TArray<VMD_FACE_KEY>	keyList;
		// sort frame num
		TArray<int32>			sortIndexList;
	};
	struct VmdCameraTrackList
	{
		// Track Name ( const MMDCAM)
		FString					TrackName;
		// min Frame
		uint32					minFrameCount;
		// max Frame
		uint32					maxFrameCount;
		// Sorted Key Frame Data List
		TArray<VMD_CAMERA>		keyList;
		// sort frame num
		TArray<int32>			sortIndexList;
	};
	//
	DECLARE_LOG_CATEGORY_EXTERN(LogMMD4UE4_VmdMotionInfo, Log, All)
	// Inport�p meta data �i�[�N���X
	class VmdMotionInfo : public MMDImportHelper
	{
	public:
		VmdMotionInfo();
		~VmdMotionInfo();

		///////////////////////////////////////
		bool VMDLoaderBinary(
			const uint8 *& Buffer,
			const uint8 * BufferEnd
			);

		///////////////////////////////////////
		bool ConvertVMDFromReadData(
			VmdReadMotionData * readData
			);


		//////////////////////////////////////////
		enum EVMDKEYFRAMETYPE
		{
			EVMD_KEYBONE,
			EVMD_KEYFACE,
			EVMD_KEYCAM 
		};
		//////////////////////////////////////////
		// �w��List���ŊY������Frame��������΂���Index�l��Ԃ��B�ُ�l=-1�B
		int32 FindKeyTrackName(
			FString targetName,
			EVMDKEYFRAMETYPE listType);

		//////////////////////////////////////////

		// vmd Target Model Name
		FString					ModelName;
		// min Frame 
		uint32					minFrame;
		// max Frams
		uint32					maxFrame;

		//Keys
		TArray<VmdKeyTrackList>	keyBoneList;
		//Skins
		TArray<VmdFaceTrackList> keyFaceList;
		//camera
		TArray<VmdCameraTrackList> keyCameraList;
	};

}
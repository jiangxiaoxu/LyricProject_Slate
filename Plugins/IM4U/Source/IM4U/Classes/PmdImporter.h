// Copyright 2015 BlackMa9. All Rights Reserved.
#pragma once


#include "Engine.h"

#include "PmxImporter.h"
#include "MMDImportHelper.h"

// Copy From DxLib DxModelLoader3.h
// DX Library Copyright (C) 2001-2008 Takumi Yamada.

//#define uint8 (unsigned char)

namespace MMD4UE4
{

	// �}�N����` -----------------------------------

	
	// �f�[�^�^��` ---------------------------------

	// PMD�f�[�^�w�b�_( 283byte )
	struct PMD_HEADER
	{
		/*uint8	Data[283];						// �f�[�^
		*/
		char	Magic[ 3 ] ;						// "Pmd"
		uint8	Version[4];							// 1.0f ( 0x3f800000 )
		char	Name[ 20 ] ;						// ���O
		char	Comment[ 256 ] ;					// �R�����g
		
	};

	// ���_�f�[�^��ӂ�̏��( 38byte )
	struct PMD_VERTEX
	{
		float	Position[3];						// ���W
		float	Normal[3];						// �@��
		float	Uv[2];							// �e�N�X�`�����W
		uint16	BoneNo[2];						// �{�[���ԍ�
		uint8	BoneWeight;						// BoneNo[ 0 ] �̃{�[���̉e���x( 0�`100 ) BoneNo[ 1 ] �̉e���x��( 100 - BoneWeight )
		uint8	Edge;								// �G�b�W�t���O  0:�G�b�W�L��  1:�G�b�W����
	};

	// ���_�f�[�^
	struct PMD_VERTEX_DATA
	{
		int	Count;				// ���_�f�[�^�̐�
		/* ������ PMD_VERTEX �� Count �̐��������݂��� */
	};
	// �ʃ��X�g
	struct PMD_FACE
	{
		uint16	VertexIndx[3];		// ���_�̐�( �ʂ̐��� VertexCount / 3 )
		/* ������ uint16 �̒��_�C���f�b�N�X�� VertexCount �̐��������݂��� */
	};
	// �ʃ��X�g
	struct PMD_FACE_DATA
	{
		uint32	VertexCount;		// ���_�̐�( �ʂ̐��� VertexCount / 3 )
		/* ������ uint16 �̒��_�C���f�b�N�X�� VertexCount �̐��������݂��� */
	};

	// �}�e���A����ӂ�̏��( 70byte )
	struct PMD_MATERIAL
	{
		/*uint8	Data[70];						// �f�[�^
		*/
		float	DiffuseR, DiffuseG, DiffuseB ;		// �f�B�t���[�Y�J���[
		float	Alpha ;								// ���l
		float	Specularity ;						// �X�y�L�����W��
		float	SpecularR, SpecularG, SpecularB ;	// �X�y�L�����J���[
		float	AmbientR, AmbientG, AmbientB ;		// �A���r�G���g�J���[
		uint8	ToolImage ;							// �g�D�[�������_�����O�p�̃e�N�X�`���̃C���f�b�N�X
		uint8	Edge ;								// �֊s�E�e
		int		FaceVertexCount ;					// ���̃}�e���A�����g�p����ʒ��_�̐�
		char	TextureFileName[20] ;				// �e�N�X�`���t�@�C����
		
	};

	// �}�e���A���̏��
	struct PMD_MATERIAL_DATA
	{
		int	Count;								// �}�e���A���̐�
		/* ������ PMD_MATERIAL �� Count �̐��������݂��� */
	};

	// �{�[����ӂ�̏��( 39byte )
	struct PMD_BONE
	{
		/*uint8	Data[39];						// �f�[�^
		*/
		char	Name[ 20 ] ;						//  0:�{�[����
		short	Parent ;							// 20:�e�{�[��( �����ꍇ�� 0xffff )
		short	TailPosBone ;						// 22:Tail �ʒu�̃{�[��( �`�F�[�����[�̏ꍇ�� 0xffff )
		uint8	Type ;								// 24:�{�[���̃^�C�v( 0:��]  1:��]�ƈړ�  2:IK  3:�s��  4:IK�e����  5:��]�e����  6:IK�ڑ���  7:��\��  8:�P��  9:��]�^�� )
		short	IkParent ;							// 25:IK�{�[���ԍ�( �e��IK�{�[��  �����ꍇ�� 0xffff )
		float	HeadPos[ 3 ] ;						// 27:�w�b�h�̈ʒu
		
	};

	// �{�[���̏��
	struct PMD_BONE_DATA
	{
		uint16	Count;								// �{�[���̐�
		/* ������ PMD_BONE �� Count �̐��������݂��� */
	};

	// IK�f�[�^��ӂ�̏��( ChainBoneIndex �𔲂����T�C�Y 11byte )
	struct PMD_IK
	{
		/*uint8	Data[11];						// �f�[�^
		*/
		uint16	Bone ;								// IK�{�[��
		uint16	TargetBone ;						// IK�^�[�Q�b�g�{�[��
		uint8	ChainLength ;						// IK�`�F�[���̒���(�q�̐�)
		uint16	Iterations ;						// �ċN���Z��
		//float	ControlWeight ;						// IK�̉e���x
		float	RotLimit;							// �P�ʐ����p[dig] (PMD�n�̂݊Y��Bone���Ђ��̏ꍇX���݂̂̐����ƂȂ�)
		TArray<uint16>	ChainBoneIndexs;	// IK�e�����̃{�[���ԍ�
		
	};

	// IK�̏��
	struct PMD_IK_DATA
	{
		uint16	Count;								// IK�f�[�^�̐�
		/* ������ PMD_IK �� Count �̐��������݂��� */
	};

	// �\��̒��_���
	struct PMD_SKIN_VERT
	{
		int		TargetVertexIndex;					// �Ώۂ̒��_�̃C���f�b�N�X
		float	Position[3];						// ���W( PMD_SKIN �� SkinType �� 0 �Ȃ��΍��W�A����ȊO�Ȃ� base �ɑ΂��鑊�΍��W )
	};

	// �\���ӂ�̏��( Vertex �𔲂����T�C�Y 25byte )
	struct PMD_SKIN
	{
		/*uint8	Data[25];						// �f�[�^
		*/
		char	Name[ 20 ] ;						// �\�
		int		VertexCount ;						// ���_�̐�
		uint8	SkinType ;							// �\��̎��( 0:base 1�F�܂�A2�F�ځA3�F���b�v�A4�F���̑� )
		TArray<PMD_SKIN_VERT> Vertex;		// �\��p�̒��_�f�[�^
		
	};

	// �\��̏��
	struct PMD_SKIN_DATA
	{
		uint16	Count;								// �\��f�[�^�̐�
		/* ������ PMD_SKIN �� Count �̐��������݂��� */
	};

#if 0
	// �������Z�f�[�^��ӂ�̏��( 83byte )
	struct PMD_PHYSICS
	{
		uint8	Data[83];
		/*
		char	RigidBodyName[ 20 ] ;				//  0 : ���̖�
		uint16	RigidBodyRelBoneIndex ;				// 20 : ���̊֘A�{�[���ԍ�
		uint8	RigidBodyGroupIndex ;				// 22 : ���̃O���[�v�ԍ�
		uint16	RigidBodyGroupTarget ;				// 23 : ���̃O���[�v�Ώ�
		uint8	ShapeType ;							// 25 : �`��( 0:��  1:��  2:�J�v�Z�� )
		float	ShapeW ;							// 26 : ��
		float	ShapeH ;							// 30 : ����
		float	ShapeD ;							// 34 : ���s
		float	Position[ 3 ] ;						// 38 : �ʒu
		float	Rotation[ 3 ] ;						// 50 : ��]( ���W�A�� )
		float	RigidBodyWeight ;					// 62 : ����
		float	RigidBodyPosDim ;					// 66 : �ړ���
		float	RigidBodyRotDim ;					// 70 : ��]��
		float	RigidBodyRecoil ;					// 74 : ������
		float	RigidBodyFriction ;					// 78 : ���C��
		uint8	RigidBodyType ;						// 82 : ���̃^�C�v( 0:Bone�Ǐ]  1:�������Z  2:�������Z(Bone�ʒu���킹) )
		*/
	};

	// �������Z�f�[�^�̏��
	struct PMD_PHYSICS_DATA
	{
		int	Count;								// ���̃f�[�^�̐�
		/* ������ PMD_PHYSICS �� Count �̐��������݂��� */
	};

	// �������Z�p�W���C���g�f�[�^��ӂ�̏��( 124byte )
	struct PMD_PHYSICS_JOINT
	{
		char	Name[20];						// ���O
		int	RigidBodyA;						// �ڑ��捄�̂`
		int	RigidBodyB;						// �ڑ��捄�̂a
		float	Position[3];						// �ʒu
		float	Rotation[3];						// ��]( ���W�A�� )
		float	ConstrainPosition1[3];			// �ړ������l�P
		float	ConstrainPosition2[3];			// �ړ������l�Q
		float	ConstrainRotation1[3];			// ��]�����l�P
		float	ConstrainRotation2[3];			// ��]�����l�Q
		float	SpringPosition[3];				// �΂ˈړ��l
		float	SpringRotation[3];				// �΂ˉ�]�l
	};

	// �������Z�p�W���C���g���
	struct PMD_PHYSICS_JOINT_DATA
	{
		int	Count;								// �W���C���g�f�[�^�̐�
		/* ������ PMD_PHYSICS_JOINT �� Count �̐��������݂��� */
	};

	// VMD�ǂݍ��ݏ����pPMD�{�[���f�[�^
	struct PMD_READ_BONE_INFO
	{
		PMD_BONE			*Base;					// �{�[����f�[�^
		MV1_ANIM_R			*Anim;					// ���̃{�[���p�̃A�j���[�V�������ւ̃|�C���^
		MV1_FRAME_R			*Frame;				// ���̃{�[���p�̃t���[�����ւ̃|�C���^

		VECTOR				*KeyPos;				// ���W�A�j���[�V�����L�[�z��̐擪
		FLOAT4				*KeyRot;				// ��]�A�j���[�V�����L�[�z��̐擪
		float				*KeyPosTime;			// ���W�A�j���[�V�����^�C���L�[�z��̐擪
		float				*KeyRotTime;			// ��]�A�j���[�V�����^�C���L�[�z��̐擪
		MATRIX				*KeyMatrix;			// �s��A�j���[�V�����L�[�z��̐擪
		MATRIX				*KeyMatrix2;			// �s��A�j���[�V�����L�[�z��̐擪( IsIKChild �� TRUE �̃{�[���p )

		int					Type;					// �^�C�v
		int					IsPhysics;				// �������g�p���Ă��邩�ǂ���
		int					IsIK;					// �h�j�{�[�����ǂ���
		int					IsIKAnim;				// �h�j�A�j���[�V���������邩�ǂ���
		int					IsIKChild;				// �h�j�A�j���[�V��������{�[���̎q�{�[��( ���h�j�̉e�����󂯂Ȃ��{�[�� )���ǂ���
		int					IKLimitAngle;			// �h�j���Ɋp�x���������邩�ǂ���
		MATRIX				LocalWorldMatrix;		// �\�z�������[�J�������[���h�s��
		VECTOR				Translate;				// ���s�ړ��l
		FLOAT4				Rotate;				// ��]�l
		VECTOR				OrgTranslate;			// ���_����̏����ʒu

		MATRIX				IKmat;					// IK�����Ŏg�p����s��\����
		FLOAT4				IKQuat;				// IK�����Ŏg�p����N�H�[�^�j�I��

		VECTOR				InitTranslate;			// ���s�ړ��l�̏����l
		FLOAT4				InitRotate;			// ��]�l�̏����l
#ifndef DX_NON_BULLET_PHYSICS
		int					PhysicsIndex;			// �������Z�p���̔ԍ�
		int					SetupPhysicsAnim;		// �{�[���A�j���[�V������t�������ǂ���
#endif

		struct VMD_READ_NODE_INFO	*Node;			// �{�[���Ɗ֘A�t�����Ă���m�[�h
		struct VMD_READ_KEY_INFO	*NowKey;		// ���ݍĐ����Ă���L�[
	};

	// VMD�ǂݍ��ݏ����pPMDIK�{�[���f�[�^
	struct PMD_READ_IK_INFO
	{
		PMD_IK				*Base;					// IK�{�[����f�[�^
		PMD_READ_BONE_INFO	*Bone;					// IK�^�[�Q�b�g�{�[��
		PMD_READ_BONE_INFO	*TargetBone;			// IK��[�{�[��

		uint16				LimitAngleIK;			// �p�x�����{�[�����܂܂ꂽ�h�j���ǂ���( 0:�Ⴄ 1:���� ) 
		uint16				Iterations;			// �ċN���Z��
		float				ControlWeight;			// IK�̉e���x
		float				IKTotalLength;			// IK�����̍�������`�F�C���̍Ō�܂ł̋���

		int					ChainBoneNum;			// IK�`�F�[���̒���(�q�̐�)
		uint16				*ChainBone;			// IK�e�����̃{�[���ւ̃C���f�b�N�X�z��ւ̃|�C���^

		PMD_READ_IK_INFO	*Prev;					// ���X�g�̑O�̃f�[�^�ւ̃A�h���X
		PMD_READ_IK_INFO	*Next;					// ���X�g�̎��̃f�[�^�ւ̃A�h���X
	};
	//////////////////////////////////////////////////////////////

	struct PMX_BONE_HIERARCKY
	{

	};
#endif
	//////////////////////////////////////////////////////////////

	DECLARE_LOG_CATEGORY_EXTERN(LogMMD4UE4_PmdMeshInfo, Log, All)

	// Inport�p meta data �i�[�N���X
	class PmdMeshInfo : public MMDImportHelper
	{
	public:
		PmdMeshInfo();
		~PmdMeshInfo();

		///////////////////////////////////////
		bool PMDLoaderBinary(
			const uint8 *& Buffer,
			const uint8 * BufferEnd
			);
		//////////////////////////////////////////
		bool ConvertToPmxFormat(
			PmxMeshInfo * pmxImportPtr
			);
		//////////////////////////////////////////
		PMD_HEADER			header;
		PMD_VERTEX_DATA		vertexData;
		TArray<PMD_VERTEX>	vertexList;
		PMD_FACE_DATA		faceData;
		TArray<PMD_FACE>	faceList;
		PMD_MATERIAL_DATA	materialData;
		TArray<PMD_MATERIAL>	materialList;

		PMD_BONE_DATA		boneData;
		TArray<PMD_BONE>	boneList;

		PMD_IK_DATA		ikData;
		TArray<PMD_IK>	ikList;

		PMD_SKIN_DATA		skinData;
		TArray<PMD_SKIN>	skinList;

	};

}
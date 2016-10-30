// Copyright 2015 BlackMa9. All Rights Reserved.

#include "IM4UPrivatePCH.h"
#include "PmdImporter.h"
#include "MMDImportHelper.h"

namespace MMD4UE4
{

	DEFINE_LOG_CATEGORY(LogMMD4UE4_PmdMeshInfo)

		PmdMeshInfo::PmdMeshInfo()
	{
	}


	PmdMeshInfo::~PmdMeshInfo()
	{
	}

	bool PmdMeshInfo::PMDLoaderBinary(
		const uint8 *& Buffer,
		const uint8 * BufferEnd
		)
	{
		////////////////////////////////////////////

		uint32 memcopySize = 0;
		float modelScale = 10.0f;

		memcopySize = sizeof(header);
		FMemory::Memcpy(&header, Buffer, memcopySize);
		// PMD�t�@�C�����ǂ������m�F
		if (header.Magic[0] == 'P' && header.Magic[1] == 'm' && header.Magic[2] == 'd')
		{
			UE_LOG(LogMMD4UE4_PmdMeshInfo, Warning, TEXT("PMX Import START /Correct Magic[PMX]"));
		}
		else
		{
			UE_LOG(LogMMD4UE4_PmdMeshInfo, Error, TEXT("PMX Import FALSE/Return /UnCorrect Magic[PMX]"));
			return false;
		}
		// �o�[�W�����P�ȊO�͓ǂݍ��߂Ȃ�
		/*if (*((float *)header.Version) != 0x0000803f)
		{
			//DXST_ERRORLOGFMT_ADD((_T("PMD Load Error : �o�[�W�����P�D�O�ȊO�͓ǂݍ��߂܂���\n")));
			return false;
		}*/
		Buffer += memcopySize;
		// �e�f�[�^�̐擪�A�h���X���Z�b�g
		{
			//���_
			memcopySize = sizeof(PMD_VERTEX_DATA);
			FMemory::Memcpy(&vertexData, Buffer, memcopySize);
			Buffer += memcopySize;

			memcopySize = 38;// sizeof(PMD_VERTEX);
			vertexList.AddZeroed(vertexData.Count); 
			for (int32 i = 0; i < vertexData.Count; ++i)
			{
				FMemory::Memcpy(&vertexList[i], Buffer, memcopySize);
				Buffer += memcopySize;
			}

		}
		{
			//�ʃf�[�^
			memcopySize = sizeof(PMD_FACE_DATA);
			FMemory::Memcpy(&faceData, Buffer, memcopySize);
			Buffer += memcopySize;

			memcopySize = sizeof(PMD_FACE);
			faceList.AddZeroed(faceData.VertexCount / 3);
			for (uint32 i = 0; i < faceData.VertexCount / 3; ++i)
			{
				FMemory::Memcpy(&faceList[i], Buffer, memcopySize);
				Buffer += memcopySize;
			}

		}
		{
			//�}�e���A��
			memcopySize = sizeof(PMD_MATERIAL_DATA);
			FMemory::Memcpy(&materialData, Buffer, memcopySize);
			Buffer += memcopySize;

			//memcopySize = 70;//sizeof(PMD_MATERIAL);
			materialList.AddZeroed(materialData.Count);
			for (int32 i = 0; i < materialData.Count; ++i)
			{
				memcopySize = sizeof(float) * 11 + sizeof(uint8) * 2;//DiffuseR --> Edge;
				FMemory::Memcpy(&materialList[i], Buffer, memcopySize);
				Buffer += memcopySize;
				memcopySize = sizeof(int) * 1 + sizeof(char) * 20;//FaceVertexCount --> TextureFileName[20];
				FMemory::Memcpy(&(materialList[i].FaceVertexCount), Buffer, memcopySize);
				Buffer += memcopySize;
			}

		}
		{
			//�{�[��
			memcopySize = sizeof(PMD_BONE_DATA);
			FMemory::Memcpy(&boneData, Buffer, memcopySize);
			Buffer += memcopySize;

			//memcopySize = 39;//sizeof(PMD_BONE);
			boneList.AddZeroed(boneData.Count);
			for (int32 i = 0; i < boneData.Count; ++i)
			{
				memcopySize =
					sizeof(char) * 20
					+ sizeof(uint16) * 2
					+ sizeof(uint8) * 1
					+ sizeof(uint16) * 1;//name --> IkParent;
				FMemory::Memcpy(&boneList[i], Buffer, memcopySize);
				Buffer += memcopySize;
				memcopySize =
					sizeof(float) * 3;//name --> IkParent;
				FMemory::Memcpy(&(boneList[i].HeadPos[0]), Buffer, memcopySize);
				Buffer += memcopySize;
			}

		}
		{
			//IK
			memcopySize = sizeof(PMD_IK_DATA);
			FMemory::Memcpy(&ikData, Buffer, memcopySize);
			Buffer += memcopySize;

			//memcopySize = 39;//sizeof(PMD_BONE);
			ikList.AddZeroed(ikData.Count);
			for (int32 i = 0; i < ikData.Count; ++i)
			{
				memcopySize = 0
					+ sizeof(uint16) * 2	//Bone,TargetBone
					+ sizeof(uint8) * 1;		//ChainLength
				FMemory::Memcpy(&ikList[i].Bone, Buffer, memcopySize);
				Buffer += memcopySize;

				memcopySize = 0
					+ sizeof(uint16) * 1	//Iterations
					+ sizeof(float) * 1;	//RotLimit
				FMemory::Memcpy(&ikList[i].Iterations, Buffer, memcopySize);
				Buffer += memcopySize;
				//fix : pmd rotlimit (1.0 == 4.0 rad) -> dig
				ikList[i].RotLimit = FMath::RadiansToDegrees(ikList[i].RotLimit * 4.0);

				uint8 tempChainLength = ikList[i].ChainLength;//Byte ChainLength;
				ikList[i].ChainBoneIndexs.AddZeroed(tempChainLength);
				memcopySize = sizeof(uint16);// *tempChainLength;
				for (int32 k = 0; k < tempChainLength; ++k)
				{
					FMemory::Memcpy(&(ikList[i].ChainBoneIndexs[k]), Buffer, memcopySize);
					Buffer += memcopySize;
				}
			}

		}
		{
			//���[�t
			memcopySize = sizeof(PMD_SKIN_DATA);
			FMemory::Memcpy(&skinData, Buffer, memcopySize);
			Buffer += memcopySize;

			//memcopySize = 39;//sizeof(PMD_BONE);
			skinList.AddZeroed(skinData.Count);
			for (int32 i = 0; i < skinData.Count; ++i)
			{
				memcopySize = 0
					+ sizeof(char) * 20
					+ sizeof(int) * 1
					+ sizeof(uint8) * 1;//Name --> SkinType;
				FMemory::Memcpy(&skinList[i], Buffer, memcopySize);
				Buffer += memcopySize;

				int tempVertexCount = skinList[i].VertexCount;
				skinList[i].Vertex.AddZeroed(tempVertexCount);
				memcopySize = sizeof(PMD_SKIN_VERT);// *tempVertexCount;
				for (int32 k = 0; k < tempVertexCount; ++k)
				{
					FMemory::Memcpy(&(skinList[i].Vertex[k]), Buffer, memcopySize);
					Buffer += memcopySize;
				}
			}

		}
		//�\���g�i�\��A�{�[���j
		//�g���d�l(�p���Ή�)
		//�g���d�l(ToonTexture)
		//�g���d�l(�����F����)
		//�g���d�l(�����FJoint)
		//////////////////////////////////////////////
		UE_LOG(LogMMD4UE4_PmdMeshInfo, Warning, TEXT("PMX Importer Class Complete"));
		return true;
	}

	/////////////////////////////////////

	bool PmdMeshInfo::ConvertToPmxFormat(
		PmxMeshInfo * pmxMeshInfoPtr 
		)
	{
		uint32 memcopySize = 0;
		float modelScale = 10.0f;
		PmdMeshInfo * pmdMeshInfoPtr = this;

		pmxMeshInfoPtr->modelNameJP 
			= ConvertMMDSJISToFString((uint8 *)&(header.Name), sizeof(header.Name));

		pmxMeshInfoPtr->modelNameJP 
			= pmxMeshInfoPtr->modelNameJP.Replace(TEXT("."), TEXT("_"));// [.] is broken filepath for ue4 


		pmxMeshInfoPtr->modelCommentJP
			= ConvertMMDSJISToFString((uint8 *)&(header.Comment), sizeof(header.Comment));

		{
			//���v
			uint32 statics_bdef1 = 0;
			uint32 statics_bdef2 = 0;

			pmxMeshInfoPtr->vertexList.AddZeroed(vertexData.Count);
			for (int32 VertexIndex = 0; VertexIndex < vertexData.Count; ++VertexIndex)
			{
				PMD_VERTEX & pmdVertexPtr = vertexList[VertexIndex];
				PMX_VERTEX & pmxVertexPtr = pmxMeshInfoPtr->vertexList[VertexIndex];
				///
				//�ʒu(x,y,z)
				memcopySize = sizeof(pmxVertexPtr.Position);
				FMemory::Memcpy(&pmxVertexPtr.Position, pmdVertexPtr.Position, memcopySize);
				pmxVertexPtr.Position = ConvertVectorAsixToUE4FromMMD(pmxVertexPtr.Position)*modelScale;
				//�@��(x,y,z)
				memcopySize = sizeof(pmxVertexPtr.Normal);
				FMemory::Memcpy(&pmxVertexPtr.Normal, pmdVertexPtr.Normal, memcopySize);
				pmxVertexPtr.Normal = ConvertVectorAsixToUE4FromMMD(pmxVertexPtr.Normal);
				//UV(u,v)
				memcopySize = sizeof(pmxVertexPtr.UV);
				FMemory::Memcpy(&pmxVertexPtr.UV, pmdVertexPtr.Uv, memcopySize);
				/*
				float tempUV = pmxVertexPtr.UV.X;//UE4���W�n���]
				pmxVertexPtr.UV.X = 1 - pmxVertexPtr.UV.Y;
				pmxVertexPtr.UV.Y = 1 - tempUV;
				*/
				//�ǉ�UV(x,y,z,w)  PMX�w�b�_�̒ǉ�UV���ɂ��	n:�ǉ�UV�� 0�`4

				// �E�F�C�g�ό`���� 0:BDEF1 1:BDEF2 2:BDEF4 3:SDEF
				//pmxVertexPtr.WeightType = 0;
				// �{�[��Index�������ꍇ��BDEF1�Ƃ��Ă�����
				/*
				if (pmdVertexPtr.BoneNo[0] != pmdVertexPtr.BoneNo[1]
					|| (pmdVertexPtr.BoneNo[1] == 0 && pmdVertexPtr.BoneWeight == 100))
				{
					//bdef1
					pmxVertexPtr.WeightType = 0;
					pmxVertexPtr.BoneIndex[0] = pmdVertexPtr.BoneNo[0]+1;
					pmxVertexPtr.BoneIndex[1] = pmdVertexPtr.BoneNo[1]+1;
					pmxVertexPtr.BoneWeight[0] = pmdVertexPtr.BoneWeight/100.0f;
					pmxVertexPtr.BoneWeight[1] = (100 - pmdVertexPtr.BoneWeight)/100.0f;
					//
					statics_bdef1++;
				}
				else*/
				{
					//BUG::
					//BDEF1�Ƃ̍��݂��Ɖ��̂��E�F�C�g�ݒ肪�ُ�ł����Make���Ɍx�����o�邽�߁A
					//�b��Ώ��Ƃ��āAPMD�`���ł͈ꗥBDEF2�`���Ƃ���B�f�[�^�������Ȃ��肩�͐��������삷�邱�Ƃ�D��B
					//bdef2
					pmxVertexPtr.WeightType = 1;
					pmxVertexPtr.BoneIndex[0] = pmdVertexPtr.BoneNo[0] + 1;
					pmxVertexPtr.BoneIndex[1] = pmdVertexPtr.BoneNo[1] + 1;
					pmxVertexPtr.BoneWeight[0] = pmdVertexPtr.BoneWeight / 100.0f;
					pmxVertexPtr.BoneWeight[1] = (100 - pmdVertexPtr.BoneWeight) / 100.0f;
					//
					statics_bdef2++;
				}
				//�G�b�W�{��  �ގ��̃G�b�W�T�C�Y�ɑ΂��Ă̔{���l
			}
			UE_LOG(LogMMD4UE4_PmdMeshInfo, Warning,
				TEXT("PMX convert [Vertex:: statics bone type, bdef1 = %u] Complete"), statics_bdef1);
			UE_LOG(LogMMD4UE4_PmdMeshInfo, Warning,
				TEXT("PMX convert [Vertex:: statics bone type, bdef2 = %u] Complete"), statics_bdef2);
			UE_LOG(LogMMD4UE4_PmxMeshInfo, Warning, TEXT("PMX convert [VertexList] Complete"));
		}
		{
			/*
			����

			n : ���_Index�T�C�Y     | ���_�̎Q��Index

			��3�_(3���_Index)��1��
			�ގ����̖ʐ��͍ގ����̖�(���_)���ŊǗ� (��PMD����)
			*/
			uint32 PmxFaceNum = 0;
			PmxFaceNum = faceData.VertexCount;
			PmxFaceNum /= 3;

			pmxMeshInfoPtr->faseList.AddZeroed(PmxFaceNum);
			for (uint32 FaceIndex = 0; FaceIndex < PmxFaceNum; ++FaceIndex)
			{
				PMD_FACE & pmdFaceListPtr = faceList[FaceIndex];
				PMX_FACE & pmxFaseListPtr = pmxMeshInfoPtr->faseList[FaceIndex];
				//
				for (int SubNum = 0; SubNum < 3; ++SubNum)
				{
					pmxFaseListPtr.VertexIndex[SubNum]
						= pmdFaceListPtr.VertexIndx[SubNum];
				}
			}
			UE_LOG(LogMMD4UE4_PmxMeshInfo, Warning, TEXT("PMX convert [faceList] Complete"));
		}
		/*
		{

			// �e�N�X�`���̐����擾
			uint32 PmxTextureNum = 0;
			//
			memcopySize = sizeof(PmxTextureNum);
			FMemory::Memcpy(&PmxTextureNum, Buffer, memcopySize);
			Buffer += memcopySize;

			// �e�N�X�`���f�[�^���i�[���郁�����̈�̊m��
			textureList.AddZeroed(PmxTextureNum);

			// �e�N�X�`���̏����擾
			for (uint32 i = 0; i < PmxTextureNum; i++)
			{
				textureList[i].TexturePath = PMXTexBufferToFString(&Buffer, pmxEncodeType);
			}
			UE_LOG(LogMMD4UE4_PmxMeshInfo, Warning, TEXT("PMX Import [textureList] Complete"));
		}
		*/
		{
			FString tempAlphaStr;
			TArray<FString> tempTexPathList;
			// �}�e���A���̐����擾
			uint32 PmxMaterialNum = 0;
			//
			PmxMaterialNum = materialData.Count;

			// �}�e���A���f�[�^���i�[���郁�����̈�̊m��
			pmxMeshInfoPtr->materialList.AddZeroed(PmxMaterialNum);

			// �}�e���A���̓ǂ݂���
			for (uint32 i = 0; i < PmxMaterialNum; i++)
			{
				PMD_MATERIAL & pmdMaterialPtr = materialList[i];
				PMX_MATERIAL & pmxMaterialPtr = pmxMeshInfoPtr->materialList[i];

				// �ގ����̎擾
				pmxMaterialPtr.Name = FString::Printf(TEXT("mat_%d"), i);
				pmxMaterialPtr.NameEng = pmxMaterialPtr.Name;

				//Diffuse (R,G,B,A)
				pmxMaterialPtr.Diffuse[0] = pmdMaterialPtr.DiffuseR;
				pmxMaterialPtr.Diffuse[1] = pmdMaterialPtr.DiffuseG;
				pmxMaterialPtr.Diffuse[2] = pmdMaterialPtr.DiffuseB;
				pmxMaterialPtr.Diffuse[3] = pmdMaterialPtr.Alpha;
				//Specular (R,G,B))
				pmxMaterialPtr.Specular[0] = pmdMaterialPtr.SpecularR;
				pmxMaterialPtr.Specular[1] = pmdMaterialPtr.SpecularG;
				pmxMaterialPtr.Specular[2] = pmdMaterialPtr.SpecularB;
				//Specular�W��
				pmxMaterialPtr.SpecularPower = pmdMaterialPtr.Specularity;
				//Ambient (R,G,B)
				pmxMaterialPtr.Ambient[0] = pmdMaterialPtr.AmbientR;
				pmxMaterialPtr.Ambient[1] = pmdMaterialPtr.AmbientG;
				pmxMaterialPtr.Ambient[2] = pmdMaterialPtr.AmbientB;


				/*
				�`��t���O(8bit) - �ebit 0:OFF 1:ON
				0x01:���ʕ`��, 0x02:�n�ʉe, 0x04:�Z���t�V���h�E�}�b�v�ւ̕`��, 0x08:�Z���t�V���h�E�̕`��,
				0x10:�G�b�W�`��
				*/
				tempAlphaStr = FString::Printf(TEXT("%.03f"), pmdMaterialPtr.Alpha);
				//pmxMaterialPtr.CullingOff = (pmdMaterialPtr.Alpha < 1.0f) ? 1 : 0;//�{���̎d�l�̂͂��H�������n�ɍ��G�b�W�o���Ȃ��̂�1.0f���Ɠ�����
				pmxMaterialPtr.CullingOff = 1;//��L���R����PMD�̏ꍇ���ʂɂ���B���ƂœK�X�Жʂɂ���Ȃǃh���[�R�[�������炵�Ă��炢�����B�B�B
				pmxMaterialPtr.GroundShadow = (0) ? 1 : 0;
				pmxMaterialPtr.SelfShadowMap = tempAlphaStr.Equals("0.980") ? 1 : 0;
				pmxMaterialPtr.SelfShadowDraw = tempAlphaStr.Equals( "0.980") ? 1 : 0;
				pmxMaterialPtr.EdgeDraw = (0) ? 1 : 0;

				//�G�b�W�F (R,G,B,A)
				pmxMaterialPtr.EdgeColor[0] = 0;
				pmxMaterialPtr.EdgeColor[1] = 0;
				pmxMaterialPtr.EdgeColor[2] = 0;
				pmxMaterialPtr.EdgeColor[3] = 0;

				//�G�b�W�T�C�Y
				pmxMaterialPtr.EdgeSize = 0;


				//�X�t�B�A���[�h 0:���� 1:��Z(sph) 2:���Z(spa) 
				//3:�T�u�e�N�X�`��(�ǉ�UV1��x,y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��)
				pmxMaterialPtr.SphereMode = 0;//�����l
				
				PMX_TEXTURE tempTex;
				FString tempTexPathStr;
				FString tempShaPathStr;
				tempTex.TexturePath
					= ConvertMMDSJISToFString(
						(uint8 *)pmdMaterialPtr.TextureFileName,
						sizeof(pmdMaterialPtr.TextureFileName));
				if (tempTex.TexturePath.Split("/", &tempTexPathStr, &tempShaPathStr))
				{
					//�T�u�t�H���_�Ƀ}�e���A�����Ȃ��O��
					tempTex.TexturePath = tempTexPathStr;
					if (tempTex.TexturePath.Find(".spa") != -1)
					{
						//���Z�łȂ��ꍇ
						pmxMaterialPtr.SphereMode = 1;
						UE_LOG(LogMMD4UE4_PmxMeshInfo, Warning,
							TEXT("PMX convert [materialList] multi texture[%s] / sphere[%s]")
							, *tempTex.TexturePath
							, *tempShaPathStr);
					}
					else
					{
						pmxMaterialPtr.SphereMode = 2;
						UE_LOG(LogMMD4UE4_PmxMeshInfo, Warning,
							TEXT("PMX convert [materialList] add texture[%s] / sphere[%s]")
							, *tempTex.TexturePath
							, *tempShaPathStr);
					}
				}
				else if (tempTex.TexturePath.Split("*", &tempTexPathStr, &tempShaPathStr))
				{
					tempTex.TexturePath = tempTexPathStr;
					if (tempTex.TexturePath.Find(".spa") != -1)
					{
						//���Z�łȂ��ꍇ
						pmxMaterialPtr.SphereMode = 1;
						UE_LOG(LogMMD4UE4_PmxMeshInfo, Warning,
							TEXT("PMX convert [materialList] multi texture[%s] / sphere[%s]")
							, *tempTex.TexturePath
							, *tempShaPathStr);
					}
					else
					{
						pmxMaterialPtr.SphereMode = 2;
						UE_LOG(LogMMD4UE4_PmxMeshInfo, Warning,
							TEXT("PMX convert [materialList] add texture[%s] / sphere[%s]")
							, *tempTex.TexturePath
							, *tempShaPathStr);
					}
				}
				else
				{
					//�e�N�X�`������̂�
					if (tempTex.TexturePath.Find(".sp") != -1)
					{
						//�X�t�B�A�̂�
						tempShaPathStr = tempTex.TexturePath;
						tempTex.TexturePath = "";
						if (tempTex.TexturePath.Find(".spa") != -1)
						{
							//���Z�łȂ��ꍇ
							pmxMaterialPtr.SphereMode = 1;
						}
						else
						{
							pmxMaterialPtr.SphereMode = 2;
						}
					}
					else
					{
						//�X�t�B�A�Ȃ�
						tempShaPathStr = "";
					}
					UE_LOG(LogMMD4UE4_PmxMeshInfo, Warning,
						TEXT("PMX convert [materialList] texture[%s] mono sphere[%s]")
						, *tempTex.TexturePath
						, *tempShaPathStr);
					//tempTex.TexturePath = tempTexPathStr;
				}
				//�ʏ�e�N�X�`��, �e�N�X�`���e�[�u���̎Q��Index
				if (tempTex.TexturePath.Equals(""))
				{
					//not
					pmxMaterialPtr.TextureIndex = -1;
				}
				else
				{
					pmxMaterialPtr.TextureIndex
						//= tempTexPathList.AddUnique(pmdMaterialPtr.TextureFileName);
						= tempTexPathList.AddUnique(tempTex.TexturePath);
					//pmxMeshInfoPtr->textureList.Add/*Unique*/(tempTex);
					if (pmxMaterialPtr.TextureIndex > pmxMeshInfoPtr->textureList.Num() - 1)
					{
						pmxMeshInfoPtr->textureList.Add(tempTex);
					}
				}
				if (tempShaPathStr.Equals(""))
				{
					//not
					//�X�t�B�A�e�N�X�`��, �e�N�X�`���e�[�u���̎Q��Index  ���e�N�X�`���g���q�̐����Ȃ�
					pmxMaterialPtr.SphereTextureIndex
						= -1;
				}
				else
				{

					PMX_TEXTURE tempSphTex;
					tempSphTex.TexturePath = tempShaPathStr;
					pmxMaterialPtr.SphereTextureIndex
						//= tempTexPathList.AddUnique(pmdMaterialPtr.TextureFileName);
						= tempTexPathList.AddUnique(tempSphTex.TexturePath);
					//pmxMeshInfoPtr->textureList.Add/*Unique*/(tempTex);
					if (pmxMaterialPtr.SphereTextureIndex > pmxMeshInfoPtr->textureList.Num() - 1)
					{
						pmxMeshInfoPtr->textureList.Add(tempSphTex);
					}
				}
				//���LToon�t���O 0:�p���l�͌�Toon 1 : �p���l�͋��LToon
				pmxMaterialPtr.ToonFlag = 1;

				if (pmxMaterialPtr.ToonFlag == 0)
				{//Toon�e�N�X�`��, �e�N�X�`���e�[�u���̎Q��Index
					pmxMaterialPtr.ToonTextureIndex
						= 0;
				}
				else
				{//���LToon�e�N�X�`��[0�`9] -> ���ꂼ�� toon01.bmp�`toon10.bmp �ɑΉ�
					pmxMaterialPtr.ToonTextureIndex = pmdMaterialPtr.ToolImage;
				}

				// �����̓X�L�b�v

				//�ގ��ɑΉ������(���_)�� (�K��3�̔{���ɂȂ�)
				pmxMaterialPtr.MaterialFaceNum = pmdMaterialPtr.FaceVertexCount;
			}
			UE_LOG(LogMMD4UE4_PmxMeshInfo, Warning, TEXT("PMX convert [materialList] Complete"));
		}
		{
			// ���ׂĂ̐eBone��UE4�����ɒǉ�
			// �{�[���f�[�^���i�[���郁�����̈�̊m��
			pmxMeshInfoPtr->boneList.AddZeroed(1);

			// �{�[���̐����擾
			uint32 PmxBoneNum = 0;
			uint32 offsetBoneIndx = 1;

			// �{�[�����̎擾
			{
				pmxMeshInfoPtr->boneList[PmxBoneNum].Name = TEXT("Root");
				pmxMeshInfoPtr->boneList[PmxBoneNum].NameEng = "AllTopRootBone";
				pmxMeshInfoPtr->boneList[PmxBoneNum].Position = FVector(0);
				pmxMeshInfoPtr->boneList[PmxBoneNum].ParentBoneIndex = INDEX_NONE;
			}
			PmxBoneNum = boneData.Count;
			// �{�[���f�[�^���i�[���郁�����̈�̊m��
			pmxMeshInfoPtr->boneList.AddZeroed(PmxBoneNum);

			// �{�[�����̎擾
			uint32 PmxIKNum = 0;
			for (uint32 i = offsetBoneIndx; i < PmxBoneNum + offsetBoneIndx; i++)
			{
				PMD_BONE & pmdBonePtr = boneList[i - 1];
				PMX_BONE & pmxBonePtr = pmxMeshInfoPtr->boneList[i];

				pmxBonePtr.Name
					= ConvertMMDSJISToFString((uint8 *)&(pmdBonePtr.Name), sizeof(pmdBonePtr.Name));
				pmxBonePtr.NameEng
					= pmxBonePtr.Name;
				//
				memcopySize = sizeof(pmxBonePtr.Position);
				FMemory::Memcpy(&pmxBonePtr.Position, pmdBonePtr.HeadPos, memcopySize);
				pmxBonePtr.Position = ConvertVectorAsixToUE4FromMMD(pmxBonePtr.Position) * modelScale;

				pmxBonePtr.ParentBoneIndex
					= pmdBonePtr.Parent + offsetBoneIndx;

				//
				if (pmdBonePtr.TailPosBone != -1)
				{
					pmxBonePtr.LinkBoneIndex
						= pmdBonePtr.TailPosBone + offsetBoneIndx;
				}
				else
				{
					pmxBonePtr.LinkBoneIndex = -1;
				}

				//bone type
				switch (pmdBonePtr.Type)
				{
				case 0://0 : ��]
					pmxBonePtr.Flag_EnableRot = 1;
					break;
				case 1: //1:��]�ƈړ�
					pmxBonePtr.Flag_EnableRot = 1;
					pmxBonePtr.Flag_EnableMov = 1;
					break;
				case 2: //2:IK
					pmxBonePtr.Flag_IK = 1;
					break;
				case 3: //3:�s���H
					break;
				case 4: //4:IK�e����
					break;
				case 5: //5:��]�e����
					break;
				case 6: //6:IK�ڑ���
					break;
				case 7: //7:��\��
					break;
				case 8: //8:�P��
					break;
				case 9: //9:��]�^�� 
					break;
				default:
					break;
				}
#if 0
				//
				memcopySize = sizeof(pmxBonePtr.TransformLayer);
				FMemory::Memcpy(&pmxBonePtr.TransformLayer, Buffer, memcopySize);
				Buffer += memcopySize;

				uint16 Flag;
				//
				memcopySize = sizeof(Flag);
				FMemory::Memcpy(&Flag, Buffer, memcopySize);
				Buffer += memcopySize;

				pmxBonePtr.Flag_LinkDest = (Flag & 0x0001) != 0 ? 1 : 0;
				pmxBonePtr.Flag_EnableRot = (Flag & 0x0002) != 0 ? 1 : 0;
				pmxBonePtr.Flag_EnableMov = (Flag & 0x0004) != 0 ? 1 : 0;
				pmxBonePtr.Flag_Disp = (Flag & 0x0008) != 0 ? 1 : 0;
				pmxBonePtr.Flag_EnableControl = (Flag & 0x0010) != 0 ? 1 : 0;
				pmxBonePtr.Flag_IK = (Flag & 0x0020) != 0 ? 1 : 0;
				pmxBonePtr.Flag_AddRot = (Flag & 0x0100) != 0 ? 1 : 0;
				pmxBonePtr.Flag_AddMov = (Flag & 0x0200) != 0 ? 1 : 0;
				pmxBonePtr.Flag_LockAxis = (Flag & 0x0400) != 0 ? 1 : 0;
				pmxBonePtr.Flag_LocalAxis = (Flag & 0x0800) != 0 ? 1 : 0;
				pmxBonePtr.Flag_AfterPhysicsTransform = (Flag & 0x1000) != 0 ? 1 : 0;
				pmxBonePtr.Flag_OutParentTransform = (Flag & 0x2000) != 0 ? 1 : 0;

				if (pmxBonePtr.Flag_LinkDest == 0)
				{
					//
					memcopySize = sizeof(pmxBonePtr.OffsetPosition);
					FMemory::Memcpy(&pmxBonePtr.OffsetPosition, Buffer, memcopySize);
					pmxBonePtr.OffsetPosition = ConvertVectorAsixToUE4FromMMD(pmxBonePtr.OffsetPosition) *modelScale;
					Buffer += memcopySize;
				}
				else
				{
					pmxBonePtr.LinkBoneIndex
						= PMXExtendBufferSizeToUint32(&Buffer, this->baseHeader.BoneIndexSize);
				}

				if (pmxBonePtr.Flag_AddRot == 1 || pmxBonePtr.Flag_AddMov == 1)
				{
					pmxBonePtr.AddParentBoneIndex
						= PMXExtendBufferSizeToUint32(&Buffer, this->baseHeader.BoneIndexSize);
					//
					memcopySize = sizeof(pmxBonePtr.AddRatio);
					FMemory::Memcpy(&pmxBonePtr.AddRatio, Buffer, memcopySize);
					Buffer += memcopySize;
				}

				if (pmxBonePtr.Flag_LockAxis == 1)
				{
					//
					memcopySize = sizeof(pmxBonePtr.LockAxisVector);
					FMemory::Memcpy(&pmxBonePtr.LockAxisVector, Buffer, memcopySize);
					Buffer += memcopySize;
				}

				if (pmxBonePtr.Flag_LocalAxis == 1)
				{

					//
					memcopySize = sizeof(pmxBonePtr.LocalAxisXVector);
					FMemory::Memcpy(&pmxBonePtr.LocalAxisXVector, Buffer, memcopySize);
					Buffer += memcopySize;

					//
					memcopySize = sizeof(pmxBonePtr.LocalAxisZVector);
					FMemory::Memcpy(&pmxBonePtr.LocalAxisZVector, Buffer, memcopySize);
					Buffer += memcopySize;
				}

				if (pmxBonePtr.Flag_OutParentTransform == 1)
				{
					//
					memcopySize = sizeof(pmxBonePtr.OutParentTransformKey);
					FMemory::Memcpy(&pmxBonePtr.OutParentTransformKey, Buffer, memcopySize);
					Buffer += memcopySize;
				}
#endif
#if 1
				if (pmxBonePtr.Flag_IK == 1)
				{

					//search ik-list from pmd data.
					PMD_IK * tempPmdIKPtr = NULL;
					for (int32 listIKIndx = 0; listIKIndx < pmdMeshInfoPtr->ikData.Count; ++listIKIndx)
					{
						if (pmdMeshInfoPtr->ikList[listIKIndx].Bone == (i - 1))
						{
							tempPmdIKPtr = &(pmdMeshInfoPtr->ikList[listIKIndx]);
						}
					}
					if (tempPmdIKPtr)
					{
						PmxIKNum++;

						pmxBonePtr.IKInfo.TargetBoneIndex
							= tempPmdIKPtr->TargetBone + offsetBoneIndx;
						//
						pmxBonePtr.IKInfo.LoopNum = tempPmdIKPtr->Iterations; //const 40?

						//
						pmxBonePtr.IKInfo.RotLimit = tempPmdIKPtr->RotLimit;

						//
						pmxBonePtr.IKInfo.LinkNum = tempPmdIKPtr->ChainLength;
						if (pmxBonePtr.IKInfo.LinkNum >= PMX_MAX_IKLINKNUM)
						{
							return false;
						}

						for (int32 j = 0; j < pmxBonePtr.IKInfo.LinkNum; j++)
						{
							pmxBonePtr.IKInfo.Link[j].BoneIndex
								= tempPmdIKPtr->ChainBoneIndexs[j] + offsetBoneIndx;

							FString tempChldBoneName
								= ConvertMMDSJISToFString((uint8 *)&(boneList[tempPmdIKPtr->ChainBoneIndexs[j]].Name),
								sizeof(boneList[tempPmdIKPtr->ChainBoneIndexs[j]].Name));
							const char hiza[20] = "�Ђ�";
							FString tempHizaName
								= ConvertMMDSJISToFString((uint8 *)&(hiza),
								sizeof(hiza));
							//�G�̏ꍇ�AX��(MMD)�Ŏ�������������
							if (tempChldBoneName.Find(tempHizaName) != -1)
							{
								pmxBonePtr.IKInfo.Link[j].RotLockFlag = 1;

								pmxBonePtr.IKInfo.Link[j].RotLockMin[0] = -180;//x
								pmxBonePtr.IKInfo.Link[j].RotLockMin[1] = 0;//y
								pmxBonePtr.IKInfo.Link[j].RotLockMin[2] = 0;//z
								//
								pmxBonePtr.IKInfo.Link[j].RotLockMax[0] = -0.5;//x
								pmxBonePtr.IKInfo.Link[j].RotLockMax[1] = 0;//y
								pmxBonePtr.IKInfo.Link[j].RotLockMax[2] = 0;//z
							}
							else
							{

								pmxBonePtr.IKInfo.Link[j].RotLockFlag = 0;
							}
						}
					}
				}
#endif
			}
			UE_LOG(LogMMD4UE4_PmxMeshInfo, Warning, TEXT("PMX convert [BoneList] Complete"));
		}
		{
			//IK
		}
		{
			int32 i, j;
			// ���[�t���̐����擾
			int32 PmxMorphNum = 0;
			TArray<int32> pmdMorphIndexList;
			PMD_SKIN * basePmdMorphPtr = NULL; //base
			PMD_SKIN * targetPmdMorphPtr = NULL; //���[�t�ϊ����̕ϊ������[�t���

			for (i = 0; i < pmdMeshInfoPtr->skinData.Count; i++)
			{
				if (0 < pmdMeshInfoPtr->skinList[i].SkinType
					&&  pmdMeshInfoPtr->skinList[i].SkinType < 4)
				{
					//type 0: base���O�B����ȊO�̗L��Skin�Ȃ���Z
					//1: �܂�
					//2: ��
					//3: ���b�v
					//4: ���̑�
					PmxMorphNum++;
					pmdMorphIndexList.Add(i);
				}
				else if (0 == pmdMeshInfoPtr->skinList[i].SkinType && basePmdMorphPtr == NULL)
				{
					//��������Base���o�^����Ă���ꍇ(PMD�t�H�[�}�b�g�Ƃ��Ĉُ�)�͏㏑�����Ȃ�
					basePmdMorphPtr = &pmdMeshInfoPtr->skinList[i];
				}
				else
				{
					//Err
				}
			}
			// ���[�t���肩��Base���[�t����̏ꍇ�A���[�t�o�^������
			if (PmxMorphNum > 0 && basePmdMorphPtr )
			{
				// ���[�t�f�[�^���i�[���郁�����̈�̊m��
				pmxMeshInfoPtr->morphList.AddZeroed(PmxMorphNum);

				// ���[�t���̓ǂݍ���
				int32 PmxSkinNum = 0;
				FVector tempVec;
				for (i = 0; i < PmxMorphNum; i++)
				{
					// target morph ptr �Q��
					targetPmdMorphPtr = &pmdMeshInfoPtr->skinList[pmdMorphIndexList[i]];
					//
					pmxMeshInfoPtr->morphList[i].Name
						= ConvertMMDSJISToFString(
							(uint8 *)&(targetPmdMorphPtr->Name),
							sizeof(targetPmdMorphPtr->Name)
							);
					pmxMeshInfoPtr->morphList[i].NameEng
						= pmxMeshInfoPtr->morphList[i].Name;

					//
					pmxMeshInfoPtr->morphList[i].ControlPanel = targetPmdMorphPtr->SkinType;
					//
					pmxMeshInfoPtr->morphList[i].Type = 1;//���_�Œ�
					//
					pmxMeshInfoPtr->morphList[i].DataNum = targetPmdMorphPtr->VertexCount;

					switch (pmxMeshInfoPtr->morphList[i].Type)
					{
					case 1:	// ���_
						PmxSkinNum++;
						pmxMeshInfoPtr->morphList[i].Vertex.AddZeroed(pmxMeshInfoPtr->morphList[i].DataNum);

						for (j = 0; j < pmxMeshInfoPtr->morphList[i].DataNum; j++)
						{
							pmxMeshInfoPtr->morphList[i].Vertex[j].Index =
								basePmdMorphPtr->Vertex[targetPmdMorphPtr->Vertex[j].TargetVertexIndex].TargetVertexIndex;
							//
							tempVec.X = targetPmdMorphPtr->Vertex[j].Position[0];
							tempVec.Y = targetPmdMorphPtr->Vertex[j].Position[1];
							tempVec.Z = targetPmdMorphPtr->Vertex[j].Position[2];
							tempVec = ConvertVectorAsixToUE4FromMMD(tempVec)*modelScale;

							pmxMeshInfoPtr->morphList[i].Vertex[j].Offset[0] = tempVec.X;
							pmxMeshInfoPtr->morphList[i].Vertex[j].Offset[1] = tempVec.Y;
							pmxMeshInfoPtr->morphList[i].Vertex[j].Offset[2] = tempVec.Z;
						}
						break;
					default:
						//un support ppmd
						break;
					}
				}
			}
			UE_LOG(LogMMD4UE4_PmxMeshInfo, Warning, TEXT("PMX convert [MorphList] Complete"));
		}
		return true;
	}


}
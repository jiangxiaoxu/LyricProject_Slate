// Copyright 2015 BlackMa9. All Rights Reserved.
#pragma once

#include "Engine/DataTable.h"

#include "MMD2UE4NameTableRow.generated.h"

/**
* MMD�n����UE4�����̃f�[�^�ɕϊ�����ۂ̃e�[�u����`
* -> VMD Import���ɒǉ��ł��̃e�[�u�����w�肵MMD->UE4�̖|��Ƃ��Ďg��
* -> CSV ��Import����ۂɓ��{����g���ꍇ��UTF-8�ŕۑ����邱��
* @param Name(�f�t�H���gTag) : UE4������(�{�[�����AMorphtarget�����L�ڂ��邱��)
* @param MMDOriginalName     : MMD������(�{�[�����A�\�����͂��邱��)
*/
USTRUCT()
struct FMMD2UE4NameTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
		FString MmdOriginalName;

};

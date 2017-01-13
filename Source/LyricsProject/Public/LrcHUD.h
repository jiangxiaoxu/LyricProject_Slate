// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "LTypes.h"
#include "LrcHUD.generated.h"

/**
 *
 */


class SStandardWidget;

UCLASS()
class LYRICSPROJECT_API ALrcHUD : public AHUD
{
    GENERATED_BODY()

public:
    ALrcHUD();

    TSharedPtr<class SWidget> MyWidget;

    virtual void BeginPlay()override;

    UPROPERTY(VisibleAnywhere, Category = "Audio")
        class	ULSoundPlayerComp*  AudioComp;

    float StartTime = -1;

    float SoundDuration = -1;

    UPROPERTY(EditDefaultsOnly, Category = "Audio")
        ULyricAsset*   LyricAsset;

    UFUNCTION(BlueprintCallable, Category = "Audio")
        void PlaySound();

    float GetPlayedSeconds()const;

    float GetPlayedProgress()const;

    class USoundWave*  ImportMusicByFilePath(const FString& FileName);

    class ULyricAsset* 	ImportLyricByFilePath(const FString& FileName);



    UFUNCTION(BlueprintCallable, CustomThunk, meta = (ArrayParm = "TargetArray"), Category = "Utilities|Array")
        static void Array_Sort(const TArray<int32> TargetArray, UClass* CompareterClass, FName CompareterFunctionName = TEXT("Compareter"))
    {
        checkNoEntry();
    }


    DECLARE_FUNCTION(execArray_Sort)
    {
        Stack.MostRecentPropertyAddress = nullptr;
        Stack.StepCompiledIn<UArrayProperty>(nullptr);
        void* ArrayAddr = Stack.MostRecentPropertyAddress;
        UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);
        if (!ArrayProperty)
        {
            Stack.bArrayContextFailed = true;
            return;
        }
        P_GET_OBJECT(UClass, CompareterClass);
        P_GET_PROPERTY(UNameProperty, CompareterFunctionName);

        P_FINISH;
        P_NATIVE_BEGIN;
        GenericArray_Sort(ArrayAddr, ArrayProperty, CompareterClass, CompareterFunctionName);
        P_NATIVE_END;
    }


    static void GenericArray_Sort(void* ArrayAddr, UArrayProperty* ArrayProperty, UClass* CompareterClass, const FName& FunctionName)
    {
        UFunction*  CompareFunction = CompareterClass ? CompareterClass->FindFunctionByName(FunctionName) : nullptr;


        if (!ensure(ArrayProperty)||!ensure(ArrayAddr))
        {
            return;
        }

        if (!ensure(CompareFunction))
        {
            UE_LOG(Log, Warning, TEXT(" Can Not Find Function [%s] "), *FunctionName.ToString());
            return;
        }

        FScriptArrayHelper  ArrayHelper(ArrayProperty, ArrayAddr);

        UProperty* InnerProp = ArrayProperty->Inner;
        const int32 PropertySize = InnerProp->ElementSize * InnerProp->ArrayDim;

        bool bIsOK = (CompareFunction->NumParms ==3)&&(CompareFunction->ParmsSize== PropertySize*2+1);

        TFunction<bool(void*, void*)> Compareter = [ & ](void* TestA, void* TestB)
        {
            TArray<uint8> Buffer;
            Buffer.Reserve(PropertySize*2+1);

            FMemoryWriter  MemoryWriter(Buffer);
            bool ReturnBool = false;

            MemoryWriter.Serialize(TestA, PropertySize);
            MemoryWriter.Serialize(TestB, PropertySize);
            MemoryWriter.SerializeBits(&ReturnBool, 1);

            CompareterClass->GetDefaultObject()->ProcessEvent(CompareFunction, Buffer.GetData());

            ReturnBool = !!Buffer[PropertySize*2];
            return ReturnBool;
        };

        if (ensure(bIsOK))
        {
            for (int32 i = ArrayHelper.Num()-1; i>0; i--)
            {
                for (int32 j = 0; j<i; j++)
                {
                    if (!Compareter(ArrayHelper.GetRawPtr(j), ArrayHelper.GetRawPtr(j+1)))
                    {
                        ArrayHelper.SwapValues(j, j+1);
                    }
                }
            }
        }
    }

};

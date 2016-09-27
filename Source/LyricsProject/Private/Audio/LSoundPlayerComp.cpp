// Fill out your copyright notice in the Description page of Project Settings.

#include "LyricsProject.h"
#include "LSoundPlayerComp.h"
#include "AudioDevice.h"
#include "ActiveSound.h"




//void ULSoundPlayerComp::HandlePause(bool bPause)
//{
//	FAudioDevice* AudioDevice = GetAudioDevice();
//	check(AudioDevice);
//
//	FAudioThread::RunCommandOnAudioThread([ AudioDevice, this, bPause ] {
//		FActiveSound* ActiveSound = AudioDevice->FindActiveSound(GetAudioComponentID());
//
//		if (ensure(ActiveSound))
//		{
//			auto Iterator = ActiveSound->WaveInstances.CreateConstIterator();
//			check(Iterator);
//			FSoundSource*  SoundSource = AudioDevice->GetSoundSource(Iterator->Value);
//			if (ensure(SoundSource))
//			{
//				if (bPause&&!SoundSource->IsPaused())
//				{
//					SoundSource->Pause();
//				}
//				else if (!bPause&&SoundSource->IsPaused())
//				{
//					SoundSource->Play();
//				}
//			}
//		}
//	});
//}

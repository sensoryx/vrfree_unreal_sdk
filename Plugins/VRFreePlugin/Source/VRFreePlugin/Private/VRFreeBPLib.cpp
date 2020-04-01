// Fill out your copyright notice in the Description page of Project Settings.

#include "VRFreeBPLib.h"

#include "VRFreePlugin.h"

#include "Vector3.h"
#include "Quaternion.h"
#include "Conversions.h"


FString UVRFreeBPLib::GetVRFreeStatusString()
{
	auto* inputComponent = FVRFreePluginModule::Get().inputComponent;
	if (inputComponent == nullptr)
	{
		return FString(TEXT("VRFree Plugin not running"));
	}
	
	return FVRFreePluginModule::Get().inputComponent->GetStatusString();
}

FString UVRFreeBPLib::GetVRFreeErrorString()
{
	auto* inputComponent = FVRFreePluginModule::Get().inputComponent;
	if (inputComponent == nullptr)
	{
		return FString(TEXT("VRFree Plugin not running"));
	}
	
	return FVRFreePluginModule::Get().inputComponent->GetErrorString();
}

bool UVRFreeBPLib::isVrFreeWorking()
{

	auto* inputComponent = FVRFreePluginModule::Get().inputComponent;
	if (inputComponent == nullptr)
	{
		return false;
	}
	return FVRFreePluginModule::Get().inputComponent->IsWorking();
}

EVRFreePluginMode UVRFreeBPLib::GetCalibrationPhase()
{
	auto* inputComponent = FVRFreePluginModule::Get().inputComponent;
	if (inputComponent == nullptr)
	{
		return EVRFreePluginMode::RUNNING;
	}
	
	return FVRFreePluginModule::Get().inputComponent->operatingMode;
}

float UVRFreeBPLib::GetCalibrationProgress()
{
	auto* inputComponent = FVRFreePluginModule::Get().inputComponent;
	if (inputComponent == nullptr)
	{
		return -1.0f;
	}
	
	return FVRFreePluginModule::Get().inputComponent->calibrationCounter;
}

bool UVRFreeBPLib::isGloveConnected(const EControllerHand DeviceHand)
{
	auto* inputComponent = FVRFreePluginModule::Get().inputComponent;
	if (inputComponent == nullptr)
	{
		return false;
	}
	
	return FVRFreePluginModule::Get().inputComponent->isGloveConnected(DeviceHand);
}
FVRFreeHandData UVRFreeBPLib::GetHandData(EControllerHand DeviceHand)
{
	FVRFreeHandData data;
	if (!(DeviceHand == EControllerHand::Left || DeviceHand == EControllerHand::Right))
	{
		return data; //return nothing 
	}

	auto* inputComponent = FVRFreePluginModule::Get().inputComponent;
	if (inputComponent == nullptr)
	{
		return data;
	}

	return (DeviceHand == EControllerHand::Left) ? FVRFreePluginModule::Get().inputComponent->LeftHandData : FVRFreePluginModule::Get().inputComponent->RightHandData;
}

FVRFreeHandData UVRFreeBPLib::GetLocalAdjustedData(EControllerHand DeviceHand)
{
	FVRFreeHandData source, result;
	if (!(DeviceHand == EControllerHand::Left || DeviceHand == EControllerHand::Right))
	{
		return result; //return nothing 
	}

	auto* inputComponent = FVRFreePluginModule::Get().inputComponent;
	if (inputComponent == nullptr)
	{
		return result;
	}

	source = (DeviceHand == EControllerHand::Left) ? FVRFreePluginModule::Get().inputComponent->LeftHandData : FVRFreePluginModule::Get().inputComponent->RightHandData;
	bool isRightHand = (DeviceHand == EControllerHand::Right) ? true : false;

	result.handRotation = source.Unrotate(source.handRotation, source.wristRotation, isRightHand);
	result.thumb1Rotation = source.Unrotate(source.thumb1Rotation, source.handRotation, isRightHand);
	result.thumb2Rotation = source.Unrotate(source.thumb2Rotation, source.thumb1Rotation, isRightHand);
	result.thumb3Rotation = source.Unrotate(source.thumb3Rotation, source.thumb2Rotation, isRightHand);
	result.index1Rotation = source.Unrotate(source.index1Rotation, source.handRotation, isRightHand);
	result.index2Rotation = source.Unrotate(source.index2Rotation, source.index1Rotation, isRightHand);
	result.index3Rotation = source.Unrotate(source.index3Rotation, source.index2Rotation, isRightHand);
	result.middle1Rotation = source.Unrotate(source.middle1Rotation, source.handRotation, isRightHand);
	result.middle2Rotation = source.Unrotate(source.middle2Rotation, source.middle1Rotation, isRightHand);
	result.middle3Rotation = source.Unrotate(source.middle3Rotation, source.middle2Rotation, isRightHand);
	result.ring1Rotation = source.Unrotate(source.ring1Rotation, source.handRotation, isRightHand);
	result.ring2Rotation = source.Unrotate(source.ring2Rotation, source.ring1Rotation, isRightHand);
	result.ring3Rotation = source.Unrotate(source.ring3Rotation, source.ring2Rotation, isRightHand);
	result.little1Rotation = source.Unrotate(source.little1Rotation, source.handRotation, isRightHand);
	result.little2Rotation = source.Unrotate(source.little2Rotation, source.little1Rotation, isRightHand);
	result.little3Rotation = source.Unrotate(source.little3Rotation, source.little2Rotation, isRightHand);

	return result;
}

void UVRFreeBPLib::StartCalibration()
{
	auto* inputComponent = FVRFreePluginModule::Get().inputComponent;
	if (inputComponent == nullptr)
	{
		return;
	}
	
	FVRFreePluginModule::Get().inputComponent->StartCalibration();
}

void UVRFreeBPLib::CancelCalibration()
{
	auto* inputComponent = FVRFreePluginModule::Get().inputComponent;
	if (inputComponent == nullptr)
	{
		return;
	}
	
	FVRFreePluginModule::Get().inputComponent->CancelCalibration();
}




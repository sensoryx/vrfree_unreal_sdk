// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IInputDeviceModule.h"
#include "ModuleManager.h"
#include "VRFreeInput.h"



class FVRFreePluginModule : public IInputDeviceModule
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FVRFreePluginModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FVRFreePluginModule >("VRFreePlugin");
	}


	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("VRFreePlugin");
	}


	virtual TSharedPtr< class IInputDevice > CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override;
	FVRFreeInput* inputComponent;

	bool inputDeviceCreated;
};
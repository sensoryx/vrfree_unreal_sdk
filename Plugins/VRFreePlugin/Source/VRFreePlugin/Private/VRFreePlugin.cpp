// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "VRFreePlugin.h"

#define LOCTEXT_NAMESPACE "FVRFreePluginModule"



void FVRFreePluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IInputDeviceModule::StartupModule();

	inputDeviceCreated = false;

}

void FVRFreePluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

TSharedPtr< class IInputDevice > FVRFreePluginModule::CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
	inputComponent = new FVRFreeInput(InMessageHandler);
	inputDeviceCreated = true;

	return TSharedPtr< class IInputDevice >(inputComponent);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVRFreePluginModule, VRFreePlugin)
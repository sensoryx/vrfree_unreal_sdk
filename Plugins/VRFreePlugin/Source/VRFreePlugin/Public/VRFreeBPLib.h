// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "VRFreeInput.h"

#include "Vector3.h"
#include "Quaternion.h"
#include "Conversions.h"
#include "HandData.h"

#include "VRFreeBPLib.generated.h"

/**
 * 
 */

UCLASS()
class VRFREEPLUGIN_API UVRFreeBPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure, Category = "VRFree")
	static FString GetVRFreeStatusString();

	UFUNCTION(BlueprintPure, Category = "VRFree")
	static FString GetVRFreeErrorString();

	UFUNCTION(BlueprintPure, Category = "VrFree")
	static bool isVrFreeWorking();

	UFUNCTION(BlueprintPure, Category = "VrFree")
	static bool isGloveConnected(const EControllerHand DeviceHand);

	UFUNCTION(BlueprintPure, Category = "VRFree")
	static FVRFreeHandData GetHandData(EControllerHand DeviceHand);

	UFUNCTION(BlueprintCallable, Category = "VRFree")
	static FVRFreeHandData GetLocalAdjustedData(EControllerHand DeviceHand);

	UFUNCTION(BlueprintCallable, Category = "VRFree")
	static void StartCalibration();

	UFUNCTION(BlueprintCallable, Category = "VRFree")
	static void CancelCalibration();

	UFUNCTION(BlueprintPure, Category = "VrFree")
	static EVRFreePluginMode GetCalibrationPhase();

	UFUNCTION(BlueprintPure, Category = "VrFree")
	static float GetCalibrationProgress();
 
};

/*
 * The coordinate system used by the VRfree dll is (x: right, y: up, z: forward). 
 */

#pragma once

#ifdef VRFREE_BUILD_DLL
#define DLLEXTERN extern "C" __declspec(dllexport)
#else
#define DLLEXTERN extern "C" __declspec(dllimport)
#endif

#include "HandData.h"
#include "HandAngles.h"
#include <string>

/* VRfree_Init must be called before any of the other functions.
 * Returns an error code with the following meaning:
 * 0: init ok
 * 1: file not found error (check path)
 * 2: file load error (locate VRfreeDotNET.dll and Newtonsoft.Json.dll in explorer and unblock in properties, possible x86/x64 mismatch)
 *
 * -1: other error 
 */
DLLEXTERN int VRfree_Init(const wchar_t * VRfreeDotNetDllPath);
DLLEXTERN void VRfree_Shutdown();
DLLEXTERN void VRfree_Start();
DLLEXTERN void VRfree_Stop();
DLLEXTERN unsigned char VRfree_StatusCode();
DLLEXTERN void VRfree_calibratePoseSimple(bool isRightHand, Quaternion direction);
DLLEXTERN void VRfree_calibratePose(bool isRightHand, HandData customCalibrationTargetData);
DLLEXTERN bool VRfree_isNewDataAvailable();
DLLEXTERN HandData VRfree_getHandData(bool isRightHand, Vector3 cameraPosition, Quaternion cameraRotation);
DLLEXTERN HandData VRfree_getHandDataFixedHeadModule(bool isRightHand, Vector3 fixedHeadModulePosition, Quaternion fixedHeadModuleRotation);
DLLEXTERN HandData VRfree_getHandDataAbsolute(bool isRightHand);
DLLEXTERN HandData VRfree_getHandDataRaw(bool isRightHand);
DLLEXTERN void VRfree_getTrackerData(Vector3& outTrackerPosition, Quaternion& outTrackerRotation, bool& outIsTrackerPositionValid);
DLLEXTERN void VRfree_getTrackerDataAbsolute(Vector3& outTrackerPosition, Quaternion& outTrackerRotation, bool& outIsTrackerPositionValid);
DLLEXTERN Quaternion VRfree_getHeadRotation();
DLLEXTERN HandAngles VRfree_getHandAngles(bool isRightHand);
DLLEXTERN HandData VRfree_getHandDataFromAngles(bool isRightHand, HandAngles handAngles);
DLLEXTERN HandData VRfree_getHandDataSynthetic(bool isRightHand, Vector3 cameraPosition, Quaternion cameraRotation);
DLLEXTERN void VRfree_setWristOffset(bool isRightHand, Vector3 offset);

DLLEXTERN void VRfree_loadGestures(std::string subfolder);
DLLEXTERN int VRfree_getNumStaticGestures();
DLLEXTERN int VRfree_getNumMultiGestures();
//DLLEXTERN std::string VRfree_getStaticGestureName(int index);
//DLLEXTERN std::string VRfree_getMultiGestureName(int index);
DLLEXTERN int VRfree_getStaticGesturesDetectedLeft();
DLLEXTERN int VRfree_getStaticGesturesDetectedRight();
DLLEXTERN int VRfree_getMultiGesturesDetectedLeft();
DLLEXTERN int VRfree_getMultiGesturesDetectedRight();
/*
 * In order to use implicit linking of the VRfreeC DLL, define VRFREE_IMPLICIT_LINKING.
 * You'll have to add VRfreeC.lib to your additional dependencies, and the DLL will be linked at 
 * compile time.
 * If you do not define VRFREE_IMPLICIT_LINKING, explicit linking will be used. Make sure to add
 * VRfreeApi.cpp to your project. The advantage of explicit linking is, that the exact addresses
 * of the functions inside VRfreeC.dll do not need to be known at compile time and therefore the
 * dll can later be updated without needing to recompile the application.
 * 
 * The coordinate system used by the VRfree dll is (x: right, y: up, z: forward). 
 */

#pragma once
#pragma warning( disable : 4668 )

#ifdef VRFREE_BUILD_DLL
#define DLLEXTERN extern "C" __declspec(dllexport)
#elif VRFREE_IMPLICIT_LINKING 
#define DLLEXTERN extern "C" __declspec(dllimport)
#else
#define DLLEXTERN
#endif

#include "HandData.h"
#include "HandAngles.h"
#include "VRfree_BoneTransform.h"

/* VRfree_Init must be called before any of the other functions.
 * Params:
 *  const wchar_t * VRfreeDotNetDllPath
 *		full path to VRfreeDotNET.dll including the file name
 * Returns an error code with the following meaning:
 * 		0:  init ok
 * 		1:  file not found error (check path)
 * 		2:  file load error (locate VRfreeDotNET.dll and Newtonsoft.Json.dll in explorer and unblock in properties, possible x86/x64 mismatch)
 *	    -1: other error 
 */
DLLEXTERN int VRfree_Init(const wchar_t * VRfreeDotNetDllPath);
DLLEXTERN void VRfree_Shutdown();
DLLEXTERN void VRfree_Start();
DLLEXTERN void VRfree_Stop();
DLLEXTERN unsigned char VRfree_StatusCode();

// Callback for calibration status with int currentCalibrationStep, int totalNumCalibrationSteps, float progress
typedef void (*CalibrationStatusUpdatePtr_t)(int, int, float);
// Callback for calibration error with error description as c-string
typedef void (*CalibrationErrorCallbackPtr_t)(const char*);
DLLEXTERN void VRfree_startPoseCalibration(CalibrationStatusUpdatePtr_t calibrationStatusUpdate, CalibrationErrorCallbackPtr_t calibrationErrorCallback);

// Obsolete: use VRfree_startPoseCalibration instead
DLLEXTERN void VRfree_calibratePoseSimple(bool isRightHand, Quaternion direction);
// Obsolete: use VRfree_startPoseCalibration instead
DLLEXTERN void VRfree_calibratePose(bool isRightHand, HandData customCalibrationTargetData);

DLLEXTERN void VRfree_updateCameraPose(Vector3 cameraPosition, Quaternion cameraRotation, bool fixedHeadModule);
DLLEXTERN HandData VRfree_getHandData(bool isRightHand);
DLLEXTERN HandData VRfree_getHandDataAbsolute(bool isRightHand);
DLLEXTERN HandData VRfree_getHandDataRaw(bool isRightHand);
// the delta HandData contains in wristPosition the velocity of the wrist in m/ms, and in the Quaternions the rotations performed in 1ms (Quaternion.xyz = angular velocities in rad/ms)
DLLEXTERN HandData VRfree_getHandDelta(bool isRightHand);
DLLEXTERN void VRfree_getTrackerData(Vector3& outTrackerPosition, Quaternion& outTrackerRotation, bool& outIsTrackerPositionValid, unsigned char& outButtonPressed, int trackerID);
DLLEXTERN void VRfree_getTrackerDataAbsolute(Vector3& outTrackerPosition, Quaternion& outTrackerRotation, bool& outIsTrackerPositionValid, unsigned char& outButtonPressed, int trackerID);
DLLEXTERN Quaternion VRfree_getHeadRotation();
DLLEXTERN HandAngles VRfree_getHandAngles(bool isRightHand);
DLLEXTERN HandData VRfree_getHandDataFromAngles(bool isRightHand, HandAngles handAngles);
DLLEXTERN void VRfree_setWristOffset(bool isRightHand, Vector3 offset);
DLLEXTERN void VRfree_setAdditionalPredictionTime(float additionalPredictionTime);

// returns bit mask, compare with defines in DeviceType.h, e.g. 
// if(VRfree_getConnectedDevices() & VRFREE_DEVICE_GLOVE_R_WIRELESS != 0) { /* right glove connected */ }
DLLEXTERN unsigned int VRfree_getConnectedDevices();

DLLEXTERN void VRfree_loadGestures(const char* subfolder);
DLLEXTERN int VRfree_getNumStaticGestures();
DLLEXTERN int VRfree_getNumMultiGestures();
DLLEXTERN void VRfree_getStaticGestureName(int index, char* dst, int dstSize);
DLLEXTERN void VRfree_getMultiGestureName(int index, char* dst, int dstSize);
DLLEXTERN int VRfree_getStaticGesturesDetectedLeft();
DLLEXTERN int VRfree_getStaticGesturesDetectedRight();
DLLEXTERN int VRfree_getMultiGesturesDetectedLeft();
DLLEXTERN int VRfree_getMultiGesturesDetectedRight();

// copies hand VRfree_BoneTransforms to boneTransforms array which needs to be preallocated to VRfree_BoneIndex_NumBones.
// boneTransforms represent hand in a T-pose, for the current pose given by the gloves use rotations from VRfree_getHandData.
DLLEXTERN void VRfree_getRightHandTransforms(VRfree_BoneTransform* boneTransforms);
DLLEXTERN void VRfree_getLeftHandTransforms(VRfree_BoneTransform* boneTransforms);
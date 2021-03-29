#ifndef VRFREE_IMPLICIT_LINKING

#include <VRfreeApi.h>
#include <Quaternion.h>
#include <HandData.h>
#include <HandAngles.h>

#include <windows.h>
#include <PathCch.h>

const int VRFREE_NUM_FUNCTIONS = 32;

const char* VRFREE_FUNCTION_NAMES[VRFREE_NUM_FUNCTIONS] = {
	"VRfree_Init",
	"VRfree_Shutdown",
	"VRfree_Start",
	"VRfree_Stop",
	"VRfree_StatusCode",
	"VRfree_startPoseCalibration",
	"VRfree_calibratePoseSimple",
	"VRfree_calibratePose",
	"VRfree_updateCameraPose",
	"VRfree_getHandData",
	"VRfree_getHandDataAbsolute",
	"VRfree_getHandDataRaw",
	"VRfree_getHandDelta",
	"VRfree_getTrackerData",
	"VRfree_getTrackerDataAbsolute",
	"VRfree_getHeadRotation",
	"VRfree_getHandAngles",
	"VRfree_getHandDataFromAngles",
	"VRfree_setWristOffset",
	"VRfree_setAdditionalPredictionTime",
	"VRfree_getConnectedDevices",
	"VRfree_loadGestures",
	"VRfree_getNumStaticGestures",
	"VRfree_getNumMultiGestures",
	"VRfree_getStaticGestureName",
	"VRfree_getMultiGestureName",
	"VRfree_getStaticGesturesDetectedLeft",
	"VRfree_getStaticGesturesDetectedRight",
	"VRfree_getMultiGesturesDetectedLeft",
	"VRfree_getMultiGesturesDetectedRight",
	"VRfree_getRightHandTransforms",
	"VRfree_getLeftHandTransforms",
};

// generic function pointer
typedef int(__stdcall* func_ptr_t)();

typedef struct {
	int(__stdcall* VRfree_Init_ptr)(const wchar_t* VRfreeDotNetDllPath);
	void(__stdcall* VRfree_Shutdown_ptr)();
	void(__stdcall* VRfree_Start_ptr)();
	void(__stdcall* VRfree_Stop_ptr)();
	unsigned char(__stdcall* VRfree_StatusCode_ptr)();
	void(__stdcall* VRfree_startPoseCalibration_ptr)(CalibrationStatusUpdatePtr_t calibrationStatusUpdate, CalibrationErrorCallbackPtr_t calibrationErrorCallback);
	void(__stdcall* VRfree_calibratePoseSimple_ptr)(bool isRightHand, Quaternion direction);
	void(__stdcall* VRfree_calibratePose_ptr)(bool isRightHand, HandData customCalibrationTargetData);
	void(__stdcall* VRfree_updateCameraPose_ptr)(Vector3 cameraPosition, Quaternion cameraRotation, bool fixedHeadModule);
	HandData(__stdcall* VRfree_getHandData_ptr)(bool isRightHand);
	HandData(__stdcall* VRfree_getHandDataAbsolute_ptr)(bool isRightHand);
	HandData(__stdcall* VRfree_getHandDataRaw_ptr)(bool isRightHand);
	HandData(__stdcall* VRfree_getHandDelta_ptr)(bool isRightHand);
	void(__stdcall* VRfree_getTrackerData_ptr)(Vector3& outTrackerPosition, Quaternion& outTrackerRotation, bool& outIsTrackerPositionValid, unsigned char& outButtonPressed, int trackerID);
	void(__stdcall* VRfree_getTrackerDataAbsolute_ptr)(Vector3& outTrackerPosition, Quaternion& outTrackerRotation, bool& outIsTrackerPositionValid, unsigned char& outButtonPressed, int trackerID);
	Quaternion(__stdcall* VRfree_getHeadRotation_ptr)();
	HandAngles(__stdcall* VRfree_getHandAngles_ptr)(bool isRightHand);
	HandData(__stdcall* VRfree_getHandDataFromAngles_ptr)(bool isRightHand, HandAngles handAngles);
	void(__stdcall* VRfree_setWristOffset_ptr)(bool isRightHand, Vector3 offset);
	void(__stdcall* VRfree_setAdditionalPredictionTime_ptr)(float additionalPredictionTime);
	unsigned int(__stdcall* VRfree_getConnectedDevices_ptr)();
	void(__stdcall* VRfree_loadGestures_ptr)(const char* subfolder);
	int(__stdcall* VRfree_getNumStaticGestures_ptr)();
	int(__stdcall* VRfree_getNumMultiGestures_ptr)();
	void(__stdcall* VRfree_getStaticGestureName_ptr)(int index, char* dst, int dstSize);
	void(__stdcall* VRfree_getMultiGestureName_ptr)(int index, char* dst, int dstSize);
	int(__stdcall* VRfree_getStaticGesturesDetectedLeft_ptr)();
	int(__stdcall* VRfree_getStaticGesturesDetectedRight_ptr)();
	int(__stdcall* VRfree_getMultiGesturesDetectedLeft_ptr)();
	int(__stdcall* VRfree_getMultiGesturesDetectedRight_ptr)();
	void(__stdcall* VRfree_getRightHandTransforms_ptr)(VRfree_BoneTransform* boneTransforms);
	void(__stdcall* VRfree_getLeftHandTransforms_ptr)(VRfree_BoneTransform* boneTransforms);
} vrfree_functions_struct;

typedef union {
	vrfree_functions_struct  by_type;
	func_ptr_t        func_ptr[VRFREE_NUM_FUNCTIONS];
} functions_union;

functions_union g_functionsUnion;

int VRfree_Init(const wchar_t* VRfreeDotNetDllPath) {
	wchar_t tempPath[MAX_PATH];
	wchar_t cDllPath[MAX_PATH];
	wcscpy_s(tempPath, VRfreeDotNetDllPath);
	for (int i = 0; i < wcslen(tempPath); i++)
		if (tempPath[i] == L'/')
			tempPath[i] = L'\\';
	PathCchRemoveFileSpec(tempPath, MAX_PATH);
	swprintf_s(cDllPath, L"%s\\VRfreeC.dll", tempPath);

	HINSTANCE vrfreeDll = LoadLibraryW(cDllPath);

	if (!vrfreeDll) {
		exit(10);
	}

	for (int i = 0; i < VRFREE_NUM_FUNCTIONS; i++) {
		g_functionsUnion.func_ptr[i] = (func_ptr_t)GetProcAddress(vrfreeDll, VRFREE_FUNCTION_NAMES[i]);

		if (g_functionsUnion.func_ptr[i] == NULL) {
			exit(100 + i);
		}
	}

	return g_functionsUnion.by_type.VRfree_Init_ptr(VRfreeDotNetDllPath);
}

void VRfree_Shutdown() {
	g_functionsUnion.by_type.VRfree_Shutdown_ptr();
}

void VRfree_Start() {
	g_functionsUnion.by_type.VRfree_Start_ptr();
}

void VRfree_Stop() {
	g_functionsUnion.by_type.VRfree_Stop_ptr();
}

unsigned char VRfree_StatusCode() {
	return g_functionsUnion.by_type.VRfree_StatusCode_ptr();
}

void VRfree_startPoseCalibration(CalibrationStatusUpdatePtr_t calibrationStatusUpdate, CalibrationErrorCallbackPtr_t calibrationErrorCallback) {
	return g_functionsUnion.by_type.VRfree_startPoseCalibration_ptr(calibrationStatusUpdate, calibrationErrorCallback);
}

void VRfree_calibratePoseSimple(bool isRightHand, Quaternion direction) {
	g_functionsUnion.by_type.VRfree_calibratePoseSimple_ptr(isRightHand, direction);
}

void VRfree_calibratePose(bool isRightHand, HandData customCalibrationTargetData) {
	g_functionsUnion.by_type.VRfree_calibratePose_ptr(isRightHand, customCalibrationTargetData);
}

void VRfree_updateCameraPose(Vector3 cameraPosition, Quaternion cameraRotation, bool fixedHeadModule) {
	g_functionsUnion.by_type.VRfree_updateCameraPose_ptr(cameraPosition, cameraRotation, fixedHeadModule);
}

HandData VRfree_getHandData(bool isRightHand) {
	return g_functionsUnion.by_type.VRfree_getHandData_ptr(isRightHand);
}

HandData VRfree_getHandDataAbsolute(bool isRightHand) {
	return g_functionsUnion.by_type.VRfree_getHandDataAbsolute_ptr(isRightHand);
}

HandData VRfree_getHandDataRaw(bool isRightHand) {
	return g_functionsUnion.by_type.VRfree_getHandDataRaw_ptr(isRightHand);
} 

HandData VRfree_getHandDelta(bool isRightHand) {
	return g_functionsUnion.by_type.VRfree_getHandDelta_ptr(isRightHand);
}

void VRfree_getTrackerData(Vector3& outTrackerPosition, Quaternion& outTrackerRotation, bool& outIsTrackerPositionValid, unsigned char& outButtonPressed, int trackerID) {
	g_functionsUnion.by_type.VRfree_getTrackerData_ptr(outTrackerPosition, outTrackerRotation, outIsTrackerPositionValid, outButtonPressed, trackerID);
}

void VRfree_getTrackerDataAbsolute(Vector3& outTrackerPosition, Quaternion& outTrackerRotation, bool& outIsTrackerPositionValid, unsigned char& outButtonPressed, int trackerID) {
	g_functionsUnion.by_type.VRfree_getTrackerDataAbsolute_ptr(outTrackerPosition, outTrackerRotation, outIsTrackerPositionValid, outButtonPressed, trackerID);
}

Quaternion VRfree_getHeadRotation() {
	return g_functionsUnion.by_type.VRfree_getHeadRotation_ptr();
}

HandAngles VRfree_getHandAngles(bool isRightHand) {
	return g_functionsUnion.by_type.VRfree_getHandAngles_ptr(isRightHand);
}

HandData VRfree_getHandDataFromAngles(bool isRightHand, HandAngles handAngles) {
	return g_functionsUnion.by_type.VRfree_getHandDataFromAngles_ptr(isRightHand, handAngles);
}

void VRfree_setWristOffset(bool isRightHand, Vector3 offset) {
	g_functionsUnion.by_type.VRfree_setWristOffset_ptr(isRightHand, offset);
}

void VRfree_setAdditionalPredictionTime(float additionalPredictionTime) {
	g_functionsUnion.by_type.VRfree_setAdditionalPredictionTime_ptr(additionalPredictionTime);
}

unsigned int VRfree_getConnectedDevices() {
	return g_functionsUnion.by_type.VRfree_getConnectedDevices_ptr();
}

void VRfree_loadGestures(const char* subfolder) {
	g_functionsUnion.by_type.VRfree_loadGestures_ptr(subfolder);
}

int VRfree_getNumStaticGestures() {
	return g_functionsUnion.by_type.VRfree_getNumStaticGestures_ptr();
}

int VRfree_getNumMultiGestures() {
	return g_functionsUnion.by_type.VRfree_getNumMultiGestures_ptr();
}

void VRfree_getStaticGestureName(int index, char* dst, int dstSize) {
	g_functionsUnion.by_type.VRfree_getStaticGestureName_ptr(index, dst, dstSize);
}

void VRfree_getMultiGestureName(int index, char* dst, int dstSize) {
	return g_functionsUnion.by_type.VRfree_getMultiGestureName_ptr(index, dst, dstSize);
}

int VRfree_getStaticGesturesDetectedLeft() {
	return g_functionsUnion.by_type.VRfree_getStaticGesturesDetectedLeft_ptr();
}

int VRfree_getStaticGesturesDetectedRight() {
	return g_functionsUnion.by_type.VRfree_getStaticGesturesDetectedRight_ptr();
}

int VRfree_getMultiGesturesDetectedLeft() {
	return g_functionsUnion.by_type.VRfree_getMultiGesturesDetectedLeft_ptr();
}

int VRfree_getMultiGesturesDetectedRight() {
	return g_functionsUnion.by_type.VRfree_getMultiGesturesDetectedRight_ptr();
}

void VRfree_getRightHandTransforms(VRfree_BoneTransform* boneTransforms) {
	g_functionsUnion.by_type.VRfree_getRightHandTransforms_ptr(boneTransforms);
}

void VRfree_getLeftHandTransforms(VRfree_BoneTransform* boneTransforms) {
	g_functionsUnion.by_type.VRfree_getLeftHandTransforms_ptr(boneTransforms);
}
#endif
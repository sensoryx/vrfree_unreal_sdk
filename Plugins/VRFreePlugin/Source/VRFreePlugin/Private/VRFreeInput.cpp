// Fill out your copyright notice in the Description page of Project Settings.

#include "VRFreeInput.h"
#include "Misc/CoreDelegates.h"
#include "Features/IModularFeatures.h"
#include "Misc/Paths.h"
#include "IPluginManager.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Engine/World.h"
#include "GenericPlatform/GenericPlatformMisc.h"

#include "Conversions.h"
#include "VRfreeApi.h"
#include "HandData.h"


typedef int(*vrfree_initPtr)						(wchar_t * VRfreeDotNetDllPath);
typedef void(*vrfree_startPtr)						(void);
typedef void(*vrfree_calibratePoseSimplePtr)		(bool isRightHand, Quaternion direction);
typedef void(*vrfree_calibratePosePtr)				(bool isRightHand, HandData target);
typedef HandData(*vrfree_getHandDataPtr)			(bool isRightHand, Vector3 cameraPosition, Quaternion cameraRotation);
typedef HandData(*vrfree_getHandDataAbsolutePtr)	(bool isRightHand);
typedef void(*vrfree_getTrackerDataPtr)				(Vector3& outTrackerPosition, Quaternion& outTrackerRotation, bool& outIsTrackerPositionValid, unsigned char& outButtonPressed, int trackerId, bool fixedHeadModule);
typedef void(*vrfree_getTrackerDataAbsolutePtr)		(Vector3& outTrackerPosition, Quaternion& outTrackerRotation, bool& outIsTrackerPositionValid, unsigned char& outButtonPressed, int trackerId);
typedef BYTE(*vrfree_statusCodePtr)					(void);
typedef void(*vrfree_releasePtr)					(void);

vrfree_initPtr						_vrfree_init;
vrfree_startPtr						_vrfree_start;
vrfree_calibratePoseSimplePtr		_vrfree_calibratePoseSimple;
vrfree_calibratePosePtr				_vrfree_calibratePose;
vrfree_getHandDataPtr				_vrfree_getHandData;
vrfree_getHandDataAbsolutePtr		_vrfree_getHandDataAbsolute;
vrfree_getTrackerDataPtr			_vrfree_getTrackerData;
vrfree_getTrackerDataAbsolutePtr	_vrfree_getTrackerDataAbsolute;
vrfree_statusCodePtr				_vrfree_statusCode;
vrfree_releasePtr					_vrfree_release;

const BYTE NOT_CONNECTED			= 0x01;
const BYTE CONNECTING				= 0x02;
const BYTE CONNECTION_FAILED		= 0x04; //will be cleared on reconnect
const BYTE START_STREAMING			= 0x08;
const BYTE STREAMING				= 0x10;
const BYTE READING_FAILED			= 0x20; //will be cleared on next valid read
const BYTE INVALID_ARGUMENTS		= 0x40;	//will be cleared on getHandData with valid input


FVRFreeInput::FVRFreeInput(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
	: MessageHandler(InMessageHandler)
{

	bInitialized = false;
#if PLATFORM_WINDOWS
	const FString BaseDir = IPluginManager::Get().FindPlugin("VRFreePlugin")->GetBaseDir();
	const FString PluginDir = FPaths::Combine(*BaseDir, TEXT("ThirdParty"), TEXT("VRfree"));
	#if PLATFORM_64BITS
		const FString LibDir = FPaths::Combine(*PluginDir, TEXT("Win64"));
	#else
		const FString LibDir = FPaths::Combine(*PluginDir, TEXT("Win32"));
	#endif
#else
	UE_LOG(VRFreeInputLog, Error, TEXT("Unsupported Platform. VRFree unavailable."));
#endif
	FString Path = FPaths::Combine(*LibDir, FString("VRfreeC.dll"));
	
	UE_LOG(VRFreeInputLog, Log, TEXT("Fetching dll from %s"), *Path);

	
	DLLHandle = FPlatformProcess::GetDllHandle(*Path);

	if (!DLLHandle)
	{
		UE_LOG(VRFreeInputLog, Error, TEXT("Failed to load DLL: %s, VRFree unavailable"), *Path);
		return;
	}

	_vrfree_init						= (vrfree_initPtr)FPlatformProcess::GetDllExport(DLLHandle, TEXT("VRfree_Init"));
	_vrfree_start						= (vrfree_startPtr)FPlatformProcess::GetDllExport(DLLHandle, TEXT("VRfree_Start"));
	_vrfree_calibratePoseSimple			= (vrfree_calibratePoseSimplePtr)FPlatformProcess::GetDllExport(DLLHandle, TEXT("VRfree_calibratePoseSimple"));
	_vrfree_calibratePose				= (vrfree_calibratePosePtr)FPlatformProcess::GetDllExport(DLLHandle, TEXT("VRfree_calibratePose"));
	_vrfree_getHandData					= (vrfree_getHandDataPtr)FPlatformProcess::GetDllExport(DLLHandle, TEXT("VRfree_getHandData"));
	_vrfree_getHandDataAbsolute			= (vrfree_getHandDataAbsolutePtr)FPlatformProcess::GetDllExport(DLLHandle, TEXT("VRfree_getHandDataAbsolute"));
	_vrfree_getTrackerData				= (vrfree_getTrackerDataPtr)FPlatformProcess::GetDllExport(DLLHandle, TEXT("VRfree_getTrackerData"));
	_vrfree_getTrackerDataAbsolute		= (vrfree_getTrackerDataAbsolutePtr)FPlatformProcess::GetDllExport(DLLHandle, TEXT("VRfree_getTrackerDataAbsolute"));
	_vrfree_statusCode					= (vrfree_statusCodePtr)FPlatformProcess::GetDllExport(DLLHandle, TEXT("VRfree_StatusCode"));
	_vrfree_release						= (vrfree_releasePtr)FPlatformProcess::GetDllExport(DLLHandle, TEXT("VRfree_Shutdown"));

	FString DotNetPath = FPaths::Combine(*LibDir, FString("VRfreeDotNET.dll"));
	TCHAR * DotNetPathPtr = new TCHAR[_MAX_PATH];
	memcpy(DotNetPathPtr, *DotNetPath, (_tcslen(*DotNetPath)+1) * sizeof(TCHAR));
	//FPlatformMisc::MessageBoxExt(EAppMsgType::Type::Ok, TEXT("You may attach a debugger now"), TEXT("Waiting for input"));

	int error = _vrfree_init(DotNetPathPtr);
	if (error != 0) {
		FPlatformMisc::MessageBoxExt(EAppMsgType::Type::Ok, TEXT("Could not load VRfree Dll!"), TEXT("Error when loading VRfree Dll"));
		exit(error);
	}

	_vrfree_start();

	bInitialized = true;

	IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);

	reportingLiveData = true;
	operatingMode = EVRFreePluginMode::RUNNING;
	InitCalibrationPoses();


	LeftActivityCounter = 0;
	RightActivityCounter = 0;

	LeftRapidMovementCounter = 0;
	RightRapidMovementCounter = 0;

	LastFrameDataLeft.Init();
	LastFrameDataRight.Init();
}

FVRFreeInput::~FVRFreeInput()
{
	IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
	
	if (DLLHandle)
	{
		_vrfree_release();
		FPlatformProcess::FreeDllHandle(DLLHandle);
	}
}

bool FVRFreeInput::IsWorking()
{
	BYTE status = STREAMING & statusCode;
	return status > 0;
}

FString FVRFreeInput::GetStatusString()
{
	if (!bInitialized)
		return "Plugin failed to load properly";
	
	BYTE code = GetStatusCode();
	FString deviceStatus;
	if ((code & NOT_CONNECTED) > 0) {
		deviceStatus = "Please plug-in the VRfree device";
	}
	else if ((code & CONNECTING) > 0) {
		deviceStatus = "Connecting to VRfree device...";
	}
	else if ((code & START_STREAMING) > 0) {
		deviceStatus = "Starting VRfree data stream...";
	}
	else if ((code & STREAMING) > 0) {
		deviceStatus = "Streaming VRfree data...";
	}
	else {
		deviceStatus = "unknown";
	}
	
	return deviceStatus;
}

void FVRFreeInput::InitCalibrationPoses()
{
	
	RightCalibrationPosePhase1.cameraTimestamp = 0;
	RightCalibrationPosePhase1.timeSinceLastLeftHandData = 0;
	RightCalibrationPosePhase1.timeSinceLastRightHandData = 0;
	RightCalibrationPosePhase1.isWristPositionValid = false;
	RightCalibrationPosePhase1.wristPosition =		Vector3(0.04399999439716339f, -0.4670912027359009f, 0.5118892788887024f);
	RightCalibrationPosePhase1.wristRotation =		Quaternion::AngleAxis(-9, Vector3(0, 1, 0)) * Quaternion { 0.08424509316682816f, -0.10160095989704132f, -0.716540515422821f, 0.6849451661109924f };
	RightCalibrationPosePhase1.handRotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.05909373238682747f,  -0.0752812847495079f, -0.7197818160057068f, 0.6875717043876648f};
	RightCalibrationPosePhase1.thumb0Rotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{-0.31001660227775576f, -0.059205494821071628f, -0.21245388686656953f, 0.9247961044311523f};
	RightCalibrationPosePhase1.thumb1Rotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.024540403857827188f, -0.12419610470533371f, -0.28649163246154787f, 0.9496818780899048f};
	RightCalibrationPosePhase1.thumb2Rotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.053653087466955188f, -0.10057975351810456f, -0.28758150339126589f, 0.9509479403495789f};
	RightCalibrationPosePhase1.index0Rotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.0706062838435173f,  -0.054652076214551929f, -0.6882063150405884f, 0.7200000286102295f};
	RightCalibrationPosePhase1.index1Rotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.0923578068614006f, -0.034383274614810947f, -0.682661235332489f, 0.7240591049194336f};
	RightCalibrationPosePhase1.index2Rotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.03177689388394356f, 0.02405988797545433f, -0.6828919053077698f, 0.7294315695762634f};
	RightCalibrationPosePhase1.middle0Rotation =	Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.03205213323235512f, -0.05299308896064758f, -0.73073810338974f,  0.6798427700996399f};
	RightCalibrationPosePhase1.middle1Rotation =	Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.06083670258522034f, -0.06549468636512757f, -0.726658821105957f, 0.6811581254005432f};
	RightCalibrationPosePhase1.middle2Rotation =	Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.052159469574689868f,  -0.01138262078166008f,  -0.728493869304657f, 0.6829689741134644f};
	RightCalibrationPosePhase1.ring0Rotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.049635209143161777f,  -0.08178383857011795f, -0.760272741317749f,  0.6425209045410156f};
	RightCalibrationPosePhase1.ring1Rotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.006445939652621746f, -0.059081077575683597f,-0.7632226347923279f, 0.6433964967727661f};
	RightCalibrationPosePhase1.ring2Rotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.001628807163797319f, -0.04777967184782028f,  -0.7632399201393127f, 0.64434415102005f};
	RightCalibrationPosePhase1.pinky0Rotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{0.013947783969342709f, 0.07012030482292175f, 0.7446255683898926f, -0.6636426448822022f};
	RightCalibrationPosePhase1.pinky1Rotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{-0.017767300829291345f,  0.1291399598121643f,  0.7429212331771851f, -0.6565635204315186f};
	RightCalibrationPosePhase1.pinky2Rotation =		Quaternion::AngleAxis(-9, Vector3(0,1,0)) * Quaternion{-0.005735385697335005f, 0.07677850127220154f,0.7308371663093567f,-0.6781959533691406f};
																		  
	LeftCalibrationPosePhase1.cameraTimestamp = 0;
	LeftCalibrationPosePhase1.timeSinceLastLeftHandData = 0;
	LeftCalibrationPosePhase1.timeSinceLastRightHandData = 0;
	LeftCalibrationPosePhase1.isWristPositionValid = false;
	LeftCalibrationPosePhase1.wristPosition = Vector3(-0.04399999439716339f, -0.4670912027359009f, 0.5118892788887024f);
	LeftCalibrationPosePhase1.wristRotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{0.08454721421003342f, 0.10155735164880753f, 0.7149280905723572f, 0.6865973472595215f};
	LeftCalibrationPosePhase1.handRotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{0.05933082848787308f, 0.07529231160879135f, 0.7181693911552429f, 0.6892342567443848f};
	LeftCalibrationPosePhase1.thumb0Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{0.3094785213470459f, -0.06061876192688942f, -0.21014562249183656f,-0.925412118434906f};
	LeftCalibrationPosePhase1.thumb1Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.023754797875881196f, -0.12460063397884369f, -0.2829053997993469f, -0.9507235288619995f};
	LeftCalibrationPosePhase1.thumb2Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.05214221030473709f, -0.10052698105573654f, -0.28413164615631106f, -0.9520740509033203f};
	LeftCalibrationPosePhase1.index0Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.07086987048387528f, -0.05506007745862007f, -0.6864230036735535f,  -0.7216436862945557f};
	LeftCalibrationPosePhase1.index1Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.09288164973258972f, -0.035118211060762408f,  -0.6808511018753052f, -0.7256596088409424f};
	LeftCalibrationPosePhase1.middle0Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.03256415203213692f, -0.05340956524014473f, -0.7290371060371399f,-0.6816099286079407f};
	LeftCalibrationPosePhase1.middle1Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.06004579737782478f, -0.06413652747869492f,  -0.7250690460205078f, -0.6830492615699768f};
	LeftCalibrationPosePhase1.middle2Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.051481008529663089f, -0.010416594333946705f, -0.7268627882003784f, -0.6847715973854065f};
	LeftCalibrationPosePhase1.ring0Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.05155763030052185f, -0.08244073390960694f, -0.7583721876144409f, -0.6445287466049194f};
	LeftCalibrationPosePhase1.index2Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.03159656748175621f, 0.023702461272478105f, -0.6811313033103943f, -0.7310953736305237f};
	LeftCalibrationPosePhase1.ring1Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.008644403889775277f,-0.06014625355601311f,  -0.7613674402236939f,  -0.6454670429229736f};
	LeftCalibrationPosePhase1.ring2Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.003318685106933117f, -0.04830547422170639f,-0.7614231109619141f, -0.6464449763298035f};
	LeftCalibrationPosePhase1.pinky0Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{0.011672823689877987f,-0.07025684416294098f, -0.7427810430526733f, -0.6657360792160034f};
	LeftCalibrationPosePhase1.pinky1Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.020272130146622659f, -0.12936723232269288f, -0.7410916090011597f,-0.6585111021995544f};
	LeftCalibrationPosePhase1.pinky2Rotation =		Quaternion::AngleAxis(9, Vector3(0,1,0)) * Quaternion{-0.00741207180544734f, -0.07561208307743073f, -0.7290744781494141f, -0.6802050471305847f};


}

void FVRFreeInput::StartCalibration()
{
	operatingMode = EVRFreePluginMode::CALIBRATION_1;

	reportingLiveData = false; //pause the data collection, so our calibration pose is displayed
	
	LeftHandData.Load(LeftCalibrationPosePhase1);
	RightHandData.Load(RightCalibrationPosePhase1);
	calibrationCounter = 0.0f;

}

void FVRFreeInput::CancelCalibration()
{
	operatingMode = EVRFreePluginMode::RUNNING;
	reportingLiveData = true;
}

void FVRFreeInput::TickCalibration()
{


	Quaternion q = Quaternion();

	if (LeftRapidMovementCounter == 0 && RightRapidMovementCounter == 0 && LeftActivityCounter > 35 && RightActivityCounter > 35)
	{
		calibrationCounter += 0.01f;

		if (calibrationCounter > 1.0f)
		{

			_vrfree_calibratePose(false, LeftCalibrationPosePhase1);
			_vrfree_calibratePose(true, RightCalibrationPosePhase1);
			operatingMode = EVRFreePluginMode::RUNNING;
			reportingLiveData = true;
		}
	}
	else {
		if (LeftRapidMovementCounter > 9 || RightRapidMovementCounter > 9)
		{
			calibrationCounter = 0;
		}
	}
}


BYTE FVRFreeInput::GetStatusCode()
{
	return _vrfree_statusCode();
}

FString FVRFreeInput::GetErrorString()
{
	if (!bInitialized)
		return "Plugin failed to load properly";
	
	BYTE code = GetStatusCode();
	FString deviceError;

	if ((code & CONNECTION_FAILED) > 0) {
		deviceError = "Connection failed, please re-connect the device";
	}
	else if ((code & READING_FAILED) > 0) {
		deviceError = "Reading failed, please restart the device";
	}
	else if ((code & INVALID_ARGUMENTS) > 0) {
		deviceError = "Invalid arguments, please pass correct data to the driver";
	}
	else {
		deviceError = "none";
	}
	return deviceError;
}

FVRFreeHandData FVRFreeInput::GetHandData(const EControllerHand DeviceHand) const
{
	FVRFreeHandData data;
		
	if (!(DeviceHand == EControllerHand::Left || DeviceHand == EControllerHand::Right))
	{ // This shouldn't happen. 
		return data;
	}


	return (DeviceHand == EControllerHand::Left) ? LeftHandData : RightHandData;

}

void FVRFreeInput::Tick(float DeltaTime)
{
	if (!bInitialized)
		return;

//	UE_LOG(VRFreeInputLog, Log, TEXT("VRFree tick"));


	// store status code as well, as we're using cached values later.
	statusCode = GetStatusCode();

	// store the HMD transform, as this data cannot be accessed from render thread
	FVector HMDPos;
	FRotator HMDRot;
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDRot, HMDPos);

	FVRFreeHandData currentDataLeft, currentDataRight;

	currentDataLeft.Load(_vrfree_getHandData(false, VectorToVRFree(HMDPos), QuatToVRFree(FQuat(HMDRot))));
	currentDataRight.Load(_vrfree_getHandData(true, VectorToVRFree(HMDPos), QuatToVRFree(FQuat(HMDRot))));

	Vector3 trackerPosition;
	Quaternion trackerRotation;
	bool isTrackerPositionValid;
	int trackerId = 0;
	unsigned char buttonPressed;
	_vrfree_getTrackerData(trackerPosition, trackerRotation, isTrackerPositionValid, buttonPressed, trackerId,false);

	FRotator leftRotationDelta = currentDataLeft.handRotation - LastFrameDataLeft.handRotation;
	FRotator rightRotationDelta = currentDataRight.handRotation - LastFrameDataRight.handRotation;



	if (FMath::Abs(leftRotationDelta.Pitch) + FMath::Abs(leftRotationDelta.Yaw) + FMath::Abs(leftRotationDelta.Roll) < 0.02f)
	{
		if (LeftActivityCounter > 0)
			LeftActivityCounter--;
	}
	else
	{
		if (LeftActivityCounter < 100)
			LeftActivityCounter++;

		if (FMath::Abs(leftRotationDelta.Pitch) + FMath::Abs(leftRotationDelta.Yaw) + FMath::Abs(leftRotationDelta.Roll) < 0.12f)
		{
			if (LeftRapidMovementCounter > 0)
				LeftRapidMovementCounter--;
		}
		else
		{
			if (LeftRapidMovementCounter < 10)
				LeftRapidMovementCounter++;
		}
	}

	if (FMath::Abs(rightRotationDelta.Pitch) + FMath::Abs(rightRotationDelta.Yaw) + FMath::Abs(rightRotationDelta.Roll) < 0.02f)
	{
		if (RightActivityCounter > 0)
			RightActivityCounter--;
	}
	else
	{
		if (RightActivityCounter < 100)
			RightActivityCounter++;

		if (FMath::Abs(rightRotationDelta.Pitch) + FMath::Abs(rightRotationDelta.Yaw) + FMath::Abs(rightRotationDelta.Roll) < 0.12f)
		{
			if (RightRapidMovementCounter > 0)
				RightRapidMovementCounter--;
		}
		else
		{
			if (RightRapidMovementCounter < 10)
				RightRapidMovementCounter++;
		}
	}


	LastFrameDataLeft = currentDataLeft;
	LastFrameDataRight = currentDataRight;

	// if we're not reporting live data, quit at this point
	if (reportingLiveData)
	{
		LeftHandData = currentDataLeft;
		RightHandData = currentDataRight;
	}

	if (operatingMode == EVRFreePluginMode::CALIBRATION_1 || operatingMode == EVRFreePluginMode::CALIBRATION_2)
	{
		TickCalibration();
	}

}

bool FVRFreeInput::isGloveConnected(const EControllerHand DeviceHand)
{
	int activityCounter = (DeviceHand == EControllerHand::Left) ? LeftActivityCounter : RightActivityCounter;

	return activityCounter > 50;
}

void FVRFreeInput::SendControllerEvents()
{
	//let's do nothing here as well :D
}

void FVRFreeInput::SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

bool FVRFreeInput::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	// No exec commands supported, for now.
	return false;
}


// Haptic feedback not supported by VRFree
void FVRFreeInput::SetChannelValue(int32 UnrealControllerId, FForceFeedbackChannelType ChannelType, float Value) 
{

}
void FVRFreeInput::SetChannelValues(int32 UnrealControllerId, const FForceFeedbackValues& Values)
{

}

FName FVRFreeInput::GetMotionControllerDeviceTypeName() const 
{
	return TEXT("Sensoryx VRFree");
}

ETrackingStatus FVRFreeInput::GetControllerTrackingStatus(const int32 ControllerIndex, const EControllerHand DeviceHand) const
{
	// nothing but hands are supported
	if (!(DeviceHand == EControllerHand::Left || DeviceHand == EControllerHand::Right))
		return ETrackingStatus::NotTracked;

	// if we're not streaming data, not tracking anything as well
	if ((statusCode & STREAMING) == 0)
		return ETrackingStatus::NotTracked;

	FVRFreeHandData data = (DeviceHand == EControllerHand::Left) ? LeftHandData : RightHandData;
	if (data.isWristPositionValid)
	{
		return ETrackingStatus::Tracked;
	}
	else {
		return ETrackingStatus::InertialOnly;
	}
}

bool FVRFreeInput::GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition, float WorldToMetersScale) const
{
	if (DeviceHand == EControllerHand::Left || DeviceHand == EControllerHand::Right)
	{
		auto data = GetHandData(DeviceHand);
		OutPosition  = data.wristPosition;
		OutOrientation  = data.handRotation;

		return true;
	}
	return false;
}

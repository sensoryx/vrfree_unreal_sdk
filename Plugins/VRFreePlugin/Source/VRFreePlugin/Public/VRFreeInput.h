// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInputDevice.h"
#include "IInputInterface.h"
#include "XRMotionControllerBase.h"
#include "IHapticDevice.h"

#include "Conversions.h"

#include "HandData.h"

#include "VRFreeInput.generated.h"



DEFINE_LOG_CATEGORY_STATIC(VRFreeInputLog, Log, All);


USTRUCT(BlueprintType)
struct FVRFreeHandData {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
		int timeSinceLastDeviceData;

	UPROPERTY(BlueprintReadWrite)
		int timeSinceLastLeftHandData;

	UPROPERTY(BlueprintReadWrite)
		int timeSinceLastRightHandData;

	UPROPERTY(BlueprintReadWrite)
		bool isWristPositionValid;

	UPROPERTY(BlueprintReadWrite)
		FVector wristPosition;

	UPROPERTY(BlueprintReadWrite)
		FRotator wristRotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator handRotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator thumb1Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator thumb2Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator thumb3Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator index1Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator index2Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator index3Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator middle1Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator middle2Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator middle3Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator ring1Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator ring2Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator ring3Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator little1Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator little2Rotation;

	UPROPERTY(BlueprintReadWrite)
		FRotator little3Rotation;




	FRotator Unrotate(FRotator joint, FRotator parent, bool isRightHand)
	{
		FQuat parentQuat(parent);
		FQuat jointQuat(joint);
		FQuat unrotated = parentQuat.Inverse() * jointQuat;
		// swap axes to match UE mannequin skeleton
		return FRotator(FQuat(isRightHand ? -unrotated.X : unrotated.X, isRightHand ? -unrotated.Z : unrotated.Z, -unrotated.Y, unrotated.W));
	}


	void Load(HandData data)
	{
		timeSinceLastDeviceData = data.timeSinceLastDeviceData;
		timeSinceLastLeftHandData = data.timeSinceLastLeftHandData;
		timeSinceLastRightHandData = data.timeSinceLastRightHandData;
		isWristPositionValid = data.isWristPositionValid;
		wristPosition = VectorFromVRFree(data.wristPosition);
		wristRotation = FRotator(QuatFromVRFree(data.wristRotation));
		handRotation = FRotator(QuatFromVRFree(data.handRotation));
		thumb1Rotation = FRotator(QuatFromVRFree(data.thumb0Rotation));
		thumb2Rotation = FRotator(QuatFromVRFree(data.thumb1Rotation));
		thumb3Rotation = FRotator(QuatFromVRFree(data.thumb2Rotation));
		index1Rotation = FRotator(QuatFromVRFree(data.index0Rotation));
		index2Rotation = FRotator(QuatFromVRFree(data.index1Rotation));
		index3Rotation = FRotator(QuatFromVRFree(data.index2Rotation));
		middle1Rotation = FRotator(QuatFromVRFree(data.middle0Rotation));
		middle2Rotation = FRotator(QuatFromVRFree(data.middle1Rotation));
		middle3Rotation = FRotator(QuatFromVRFree(data.middle2Rotation));
		ring1Rotation = FRotator(QuatFromVRFree(data.ring0Rotation));
		ring2Rotation = FRotator(QuatFromVRFree(data.ring1Rotation));
		ring3Rotation = FRotator(QuatFromVRFree(data.ring2Rotation));
		little1Rotation = FRotator(QuatFromVRFree(data.pinky0Rotation));
		little2Rotation = FRotator(QuatFromVRFree(data.pinky1Rotation));
		little3Rotation = FRotator(QuatFromVRFree(data.pinky2Rotation));

		return;
	}	
	
	void Init()
	{
		timeSinceLastDeviceData = 0;
		timeSinceLastLeftHandData = 0;
		timeSinceLastRightHandData = 0;
		isWristPositionValid = false;
		wristPosition = FVector::ZeroVector;
		wristRotation = FRotator::ZeroRotator;
		handRotation = FRotator::ZeroRotator;
		thumb1Rotation = FRotator::ZeroRotator;
		thumb2Rotation = FRotator::ZeroRotator;
		thumb3Rotation = FRotator::ZeroRotator;
		index1Rotation = FRotator::ZeroRotator;
		index2Rotation = FRotator::ZeroRotator;
		index3Rotation = FRotator::ZeroRotator;
		middle1Rotation = FRotator::ZeroRotator;
		middle2Rotation = FRotator::ZeroRotator;
		middle3Rotation = FRotator::ZeroRotator;
		ring1Rotation = FRotator::ZeroRotator;
		ring2Rotation = FRotator::ZeroRotator;
		ring3Rotation = FRotator::ZeroRotator;
		little1Rotation = FRotator::ZeroRotator;
		little2Rotation = FRotator::ZeroRotator;
		little3Rotation = FRotator::ZeroRotator;

		return;
	}



};

UENUM(BlueprintType)
enum class EVRFreePluginMode : uint8
{
	RUNNING = 0 UMETA(DisplayName = "Running"),
	CALIBRATION_1 = 1 UMETA(DisplayName = "Calibration Phase 1"),
	CALIBRATION_2 = 2 UMETA(DisplayName = "Calibration Phase 2")
};

/**
 * 
 */
class VRFREEPLUGIN_API FVRFreeInput : public IInputDevice, public FXRMotionControllerBase
{
public:

	FVRFreeInput(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler);

	virtual ~FVRFreeInput();

	
	// IInputDevice overrides
	virtual void Tick(float DeltaTime) override;
	virtual void SendControllerEvents() override;
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override;
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values) override;

	// IMotionController overrides
	virtual FName GetMotionControllerDeviceTypeName() const override;
	virtual bool GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition, float WorldToMetersScale) const override;
	virtual ETrackingStatus GetControllerTrackingStatus(const int32 ControllerIndex, const EControllerHand DeviceHand) const override;

	FString GetStatusString();
	FString GetErrorString();
	bool IsWorking();

	EVRFreePluginMode operatingMode;

	void InitCalibrationPoses();
	void StartCalibration();
	void CancelCalibration();
	void TickCalibration();


	bool reportingLiveData;

	// data cached in Tick
	FVRFreeHandData LeftHandData;
	FVRFreeHandData RightHandData;

	FVRFreeHandData LastFrameDataLeft;
	FVRFreeHandData LastFrameDataRight;

	int LeftActivityCounter;
	int RightActivityCounter;

	int LeftRapidMovementCounter;
	int RightRapidMovementCounter;

	float calibrationCounter;

	HandData LeftCalibrationPosePhase1;
	HandData RightCalibrationPosePhase1;

	HandData LeftCalibrationPosePhase2;
	HandData RightCalibrationPosePhase2;

	BYTE statusCode;
	BYTE GetStatusCode();

	bool isGloveConnected(const EControllerHand DeviceHand);

	FVRFreeHandData GetHandData(const EControllerHand DeviceHand) const;


private:
	/** The recipient of motion controller input events */
	TSharedPtr< FGenericApplicationMessageHandler > MessageHandler;

	void* DLLHandle;

	bool bInitialized;

};

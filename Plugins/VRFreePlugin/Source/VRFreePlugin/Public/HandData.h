#pragma once
#include "Quaternion.h"

struct HandData {
	unsigned int cameraTimestamp;
	int timeSinceLastLeftHandData;
	int timeSinceLastRightHandData;
	bool isWristPositionValid;
	Vector3 wristPosition;
	Quaternion wristRotation;
	Quaternion handRotation;
	Quaternion thumb0Rotation;
	Quaternion thumb1Rotation;
	Quaternion thumb2Rotation;
	Quaternion index0Rotation;
	Quaternion index1Rotation;
	Quaternion index2Rotation;
	Quaternion middle0Rotation;
	Quaternion middle1Rotation;
	Quaternion middle2Rotation;
	Quaternion ring0Rotation;
	Quaternion ring1Rotation;
	Quaternion ring2Rotation;
	Quaternion pinky0Rotation;
	Quaternion pinky1Rotation;
	Quaternion pinky2Rotation;
};
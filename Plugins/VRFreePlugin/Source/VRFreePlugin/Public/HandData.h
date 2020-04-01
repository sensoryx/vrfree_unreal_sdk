#pragma once
#include "Quaternion.h"

struct HandData {
	int timeSinceLastDeviceData;
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
	
	HandData() {}

	HandData(int timeSinceLastDeviceData, int timeSinceLastLeftHandData, int timeSinceLastRightHandData,
		bool isWristPositionValid, Vector3 wristPosition,
		Quaternion wristRotation, Quaternion handRotation,
		Quaternion thumb0Rotation, Quaternion thumb1Rotation, Quaternion thumb2Rotation,
		Quaternion index0Rotation, Quaternion index1Rotation, Quaternion index2Rotation,
		Quaternion middle0Rotation, Quaternion middle1Rotation, Quaternion middle2Rotation,
		Quaternion ring0Rotation, Quaternion ring1Rotation, Quaternion ring2Rotation,
		Quaternion pinky0Rotation, Quaternion pinky1Rotation, Quaternion pinky2Rotation) {
		this->timeSinceLastDeviceData = timeSinceLastDeviceData;
		this->timeSinceLastLeftHandData = timeSinceLastLeftHandData;
		this->timeSinceLastRightHandData = timeSinceLastRightHandData;
		this->isWristPositionValid = isWristPositionValid;
		this->wristPosition = wristPosition;
		this->wristRotation = wristRotation;
		this->handRotation = handRotation;
		this->thumb0Rotation = thumb0Rotation;
		this->thumb1Rotation = thumb1Rotation;
		this->thumb2Rotation = thumb2Rotation;
		this->index0Rotation = index0Rotation;
		this->index1Rotation = index1Rotation;
		this->index2Rotation = index2Rotation;
		this->middle0Rotation = middle0Rotation;
		this->middle1Rotation = middle1Rotation;
		this->middle2Rotation = middle2Rotation;
		this->ring0Rotation = ring0Rotation;
		this->ring1Rotation = ring1Rotation;
		this->ring2Rotation = ring2Rotation;
		this->pinky0Rotation = pinky0Rotation;
		this->pinky1Rotation = pinky1Rotation;
		this->pinky2Rotation = pinky2Rotation;
	}
};
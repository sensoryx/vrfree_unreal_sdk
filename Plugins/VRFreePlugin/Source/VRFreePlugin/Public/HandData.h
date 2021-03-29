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

	void setFingerRotation(int finger, int joint, Quaternion q) {
		if(finger == 0 && joint == 0)
			thumb0Rotation = q;
		else if(finger == 0 && joint == 1)
			thumb1Rotation = q;
		else if(finger == 0 && joint == 2)
			thumb2Rotation = q;

		else if(finger == 1 && joint == 0)
			index0Rotation = q;
		else if(finger == 1 && joint == 1)
			index1Rotation = q;
		else if(finger == 1 && joint == 2)
			index2Rotation = q;

		else if(finger == 2 && joint == 0)
			middle0Rotation = q;
		else if(finger == 2 && joint == 1)
			middle1Rotation = q;
		else if(finger == 2 && joint == 2)
			middle2Rotation = q;

		else if(finger == 3 && joint == 0)
			ring0Rotation = q;
		else if(finger == 3 && joint == 1)
			ring1Rotation = q;
		else if(finger == 3 && joint == 2)
			ring2Rotation = q;

		else if(finger == 4 && joint == 0)
			pinky0Rotation = q;
		else if(finger == 4 && joint == 1)
			pinky1Rotation = q;
		else if(finger == 4 && joint == 2)
			pinky2Rotation = q;
	}

	Quaternion getFingerRotation(int finger, int joint) {
		if(finger == 0 && joint == 0)
			return thumb0Rotation;
		else if(finger == 0 && joint == 1)
			return thumb1Rotation;
		else if(finger == 0 && joint == 2)
			return thumb2Rotation;

		else if(finger == 1 && joint == 0)
			return index0Rotation;
		else if(finger == 1 && joint == 1)
			return index1Rotation;
		else if(finger == 1 && joint == 2)
			return index2Rotation;

		else if(finger == 2 && joint == 0)
			return middle0Rotation;
		else if(finger == 2 && joint == 1)
			return middle1Rotation;
		else if(finger == 2 && joint == 2)
			return middle2Rotation;

		else if(finger == 3 && joint == 0)
			return ring0Rotation;
		else if(finger == 3 && joint == 1)
			return ring1Rotation;
		else if(finger == 3 && joint == 2)
			return ring2Rotation;

		else if(finger == 4 && joint == 0)
			return pinky0Rotation;
		else if(finger == 4 && joint == 1)
			return pinky1Rotation;
		else if(finger == 4 && joint == 2)
			return pinky2Rotation;
		else
			return Quaternion();
	}
};
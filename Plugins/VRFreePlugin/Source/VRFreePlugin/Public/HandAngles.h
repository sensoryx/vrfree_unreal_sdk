#pragma once

struct HandAngles {
public:
	/// Angles of the sideways movement of the first joints in each of the 5 fingers (thumb:0 to pinky:4) of the hand
	float fingerAngles0side[5];
	/// Angles of the opening/closing movement of the first joints in each of the 5 fingers (thumb:0 to pinky:4) of the hand
	float fingerAngles0close[5];
	/// Angles of the opening/closing movement of the second joints in each of the 5 fingers (thumb:0 to pinky:4) of the hand
	float fingerAngles1close[5];
	/// Angles of the opening/closing movement of the third joints in each of the 5 fingers (thumb:0 to pinky:4) of the hand
	float fingerAngles2close[5];

	float thumbAngle1side;

	float handAngleSide;
	float handAngleClose;

};
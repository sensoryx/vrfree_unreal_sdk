#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "Vector3.h"
#include <iostream>

 struct VRFREEPLUGIN_API Quaternion {

public:
	float x;
	float y;
	float z;
	float w;

	Quaternion();
	Quaternion(float xx, float yy, float zz, float ww);

	void identity();
	void multiply(Quaternion* q);
	void multiply(Vector3* v);
	float LengthSquared();
	float Length();

	static float Dot(Quaternion a, Quaternion b);
	static float Angle(Quaternion a, Quaternion b);
	static Quaternion Inverse(Quaternion rotation);
	static Quaternion AngleAxis(float angle, Vector3 axis);
	static Quaternion Normalize(Quaternion q);
	static Quaternion Euler(float x, float y, float z);

	void operator*=(const Quaternion& rhs);
	friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);
	friend std::istream& operator>>(std::istream& is, Quaternion& q);	
};

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);
Vector3 operator*(const Quaternion& rotation, const Vector3& point);

std::ostream& operator<<(std::ostream& os, const Quaternion& q);
std::istream& operator>>(std::istream& is, Quaternion& q);



#endif //_QUATERNION_H_

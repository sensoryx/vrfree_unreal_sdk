#include "Vector3.h"

#include <math.h>

Vector3::Vector3() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float xx, float yy, float zz) {
	x = xx; 
	y = yy;
	z = zz;
}

void Vector3::right() {
	x = 1.0f;
	y = 0.0f;
	z = 0.0f;
}

void Vector3::up() {
	x = 0.0f;
	y = 1.0f;
	z = 0.0f;
}

void Vector3::forward() {
	x = 0.0f;
	y = 0.0f;
	z = 1.0f;
}

void Vector3::add(Vector3* v) {
	x += v->x;
	y += v->y;
	z += v->z;
}

void Vector3::subtract(Vector3* v) {
	x -= v->x;
	y -= v->y;
	z -= v->z;
}

void Vector3::multiply(float v) {
	x *= v;
	y *= v;
	z *= v;
}

float Vector3::dot(Vector3* v) {
	return x * v->x + y * v->y + z * v->z;
}

Vector3 Vector3::normalized() {
	float num = Vector3::Magnitude(*this);
	Vector3 returnVal(x, y, z);
	if(num > 1E-05f) {
		returnVal.x /= num;
		returnVal.y /= num;
		returnVal.z /= num;
	} else {
		returnVal.x = 0.0f;
		returnVal.y = 0.0f;
		returnVal.z = 0.0f;
	}
	return returnVal;
}

void Vector3::Normalize() {
	float num = Vector3::Magnitude(*this);
	if(num > 1E-05f) {
		x /= num;
		y /= num;
		z /= num;
	} else {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
}

float Vector3::sqrMagnitude() {
	return x*x + y*y + z*z;
}

float Vector3::magnitude() {
	return (float)sqrt(x * x + y * y + z * z);
}

float Vector3::Magnitude(Vector3 vector) {
	return (float)sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

float Vector3::Dot(Vector3 lhs, Vector3 rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vector3 Vector3::Cross(Vector3 lhs, Vector3 rhs) {
	return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

float Vector3::Angle(Vector3 from, Vector3 to) {
	return (float)acos(fmaxf(fminf(Vector3::Dot(from.normalized(), to.normalized()), 1.0f), -1.0f)) * 57.29578f;
}

float Vector3::SignedAngle(Vector3 from, Vector3 to, Vector3 axis) {
	return (Dot(Cross(from, to), axis) >= 0) ? Angle(from, to) : -Angle(from, to);
}

Vector3 operator+(const Vector3 & a, const Vector3 & b) {
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator-(const Vector3 & a, const Vector3 & b) {
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3 operator*(const Vector3 & a, const float & d) {
	return Vector3(a.x*d, a.y*d, a.z*d);
}

Vector3 operator*(const float & d, const Vector3 & a) {
	return Vector3(a.x*d, a.y*d, a.z*d);
}

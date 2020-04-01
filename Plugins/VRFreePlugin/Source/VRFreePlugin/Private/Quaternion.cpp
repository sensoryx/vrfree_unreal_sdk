#include "Quaternion.h"

#include <math.h>
#include <iostream>
#include <string>

const float radToDeg = (float)(180.0 / 3.14159265359);
const float degToRad = (float)(3.14159265359 / 180.0);

Quaternion::Quaternion() {
	identity();
}

Quaternion::Quaternion(float xx, float yy, float zz, float ww) {
	x = xx;
	y = yy;
	z = zz;
	w = ww;
}

void Quaternion::identity() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

void Quaternion::multiply(Quaternion* q) {
	float tw = w * q->w - x * q->x - y * q->y - z * q->z;
	float tx = w * q->x + x * q->w + y * q->z - z * q->y;
	float ty = w * q->y - x * q->z + y * q->w + z * q->x;
	float tz = w * q->z + x * q->y - y * q->x + z * q->w;
	x = tx;
	y = ty;
	z = tz;
	w = tw;
}

void Quaternion::multiply(Vector3* v) {
	float num = x * 2.0f;
	float num2 = y * 2.0f;
	float num3 = z * 2.0f;
	float num4 = x * num;
	float num5 = y * num2;
	float num6 = z * num3;
	float num7 = x * num2;
	float num8 = x * num3;
	float num9 = y * num3;
	float num10 = w * num;
	float num11 = w * num2;
	float num12 = w * num3;
	float tx = (1.0f - (num5 + num6)) * v->x + (num7 - num12) * v->y + (num8 + num11) * v->z;
	float ty = (num7 + num12) * v->x + (1.0f - (num4 + num6)) * v->y + (num9 - num10) * v->z;
	float tz = (num8 - num11) * v->x + (num9 + num10) * v->y + (1.0f - (num4 + num5)) * v->z;
	v->x = tx;
	v->y = ty;
	v->z = tz;
}

float Quaternion::LengthSquared() {
	return x * x + y * y + z * z + w * w;
}

float Quaternion::Length() {
	return sqrt(x * x + y * y + z * z + w * w);
}

/// <summary>
///   <para>The dot product between two rotations.</para>
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
float Quaternion::Dot(Quaternion a, Quaternion b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

/// <summary>
///   <para>Returns the angle in degrees between two rotations /a/ and /b/.</para>
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
float Quaternion::Angle(Quaternion a, Quaternion b) {
	float f = Quaternion::Dot(a, b);
	return acos(fminf(fabs(f), 1.0f)) * 2.0f * radToDeg;
}

/// <summary>
///   <para>Returns the Inverse of /rotation/.</para>
/// </summary>
/// <param name="rotation"></param>
Quaternion Quaternion::Inverse(Quaternion rotation) {
	float lengthSq = rotation.LengthSquared();
	if(lengthSq != 0.0f) {
		float i = 1.0f / lengthSq;
		return Quaternion(rotation.x * -i, rotation.y * -i, rotation.z * -i, rotation.w * i);
	}
	return rotation;
}

/// <summary>
///   <para>Creates a rotation which rotates /angle/ degrees around /axis/.</para>
/// </summary>
/// <param name="angle"></param>
/// <param name="axis"></param>
Quaternion Quaternion::AngleAxis(float angle, Vector3 axis) {
	if(axis.sqrMagnitude() == 0.0f)
		return Quaternion();

	Quaternion result;
	float radians = angle * degToRad;
	radians *= 0.5f;
	axis.Normalize();
	axis = axis * (float)sin(radians);
	result.x = axis.x;
	result.y = axis.y;
	result.z = axis.z;
	result.w = (float)cos(radians);

	return Normalize(result);
}

Quaternion Quaternion::Normalize(Quaternion q) {
	float scale = 1.0f / q.Length();
	return Quaternion(q.x * scale, q.y * scale, q.z * scale, q.w * scale);
}

/// <summary>
///   <para>Returns a rotation that rotates z degrees around the z axis, x degrees around the x axis, and y degrees around the y axis (in that order).</para>
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
Quaternion Quaternion::Euler(float x, float y, float z) {
	/*float yaw = x*degToRad;
	float pitch = y*degToRad;
	float roll = z*degToRad;
	float rollOver2 = roll * 0.5f;
	float sinRollOver2 = (float)sin((float)rollOver2);
	float cosRollOver2 = (float)cos((float)rollOver2);
	float pitchOver2 = pitch * 0.5f;
	float sinPitchOver2 = (float)sin((float)pitchOver2);
	float cosPitchOver2 = (float)cos((float)pitchOver2);
	float yawOver2 = yaw * 0.5f;
	float sinYawOver2 = (float)sin((float)yawOver2);
	float cosYawOver2 = (float)cos((float)yawOver2);
	Quaternion result;
	result.w = cosYawOver2 * cosPitchOver2 * cosRollOver2 + sinYawOver2 * sinPitchOver2 * sinRollOver2;
	result.z = cosYawOver2 * cosPitchOver2 * sinRollOver2 - sinYawOver2 * sinPitchOver2 * cosRollOver2;
	result.y = cosYawOver2 * sinPitchOver2 * cosRollOver2 + sinYawOver2 * cosPitchOver2 * sinRollOver2;
	result.x = sinYawOver2 * cosPitchOver2 * cosRollOver2 - cosYawOver2 * sinPitchOver2 * sinRollOver2;*/
	Quaternion result = AngleAxis(y, Vector3(0, 1, 0)) * AngleAxis(x, Vector3(1, 0, 0)) * AngleAxis(z, Vector3(0, 0, 1));
	return result;
}

void Quaternion::operator*=(const Quaternion & rhs) {
	float xx = x;
	float yy = y;
	float zz = z;
	float ww = w;
	x = ww*rhs.x + xx*rhs.w + yy*rhs.z - zz*rhs.y;
	y = ww*rhs.y + yy*rhs.w + zz*rhs.x - xx*rhs.z;
	z = ww*rhs.z + zz*rhs.w + xx*rhs.y - yy*rhs.x;
	w = ww*rhs.w - xx*rhs.x - yy*rhs.y - zz*rhs.z;
}

Quaternion operator*(const Quaternion & lhs, const Quaternion & rhs) {
	return Quaternion(
		lhs.w*rhs.x + lhs.x*rhs.w + lhs.y*rhs.z - lhs.z*rhs.y,
		lhs.w*rhs.y + lhs.y*rhs.w + lhs.z*rhs.x - lhs.x*rhs.z,
		lhs.w*rhs.z + lhs.z*rhs.w + lhs.x*rhs.y - lhs.y*rhs.x,
		lhs.w*rhs.w - lhs.x*rhs.x - lhs.y*rhs.y - lhs.z*rhs.z
	);
}

Vector3 operator*(const Quaternion & rotation, const Vector3 & point) {
	float num = rotation.x * 2.0f;
	float num2 = rotation.y * 2.0f;
	float num3 = rotation.z * 2.0f;
	float num4 = rotation.x * num;
	float num5 = rotation.y * num2;
	float num6 = rotation.z * num3;
	float num7 = rotation.x * num2;
	float num8 = rotation.x * num3;
	float num9 = rotation.y * num3;
	float num10 = rotation.w * num;
	float num11 = rotation.w * num2;
	float num12 = rotation.w * num3;
	Vector3 result;
	result.x = (1.0f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
	result.y = (num7 + num12) * point.x + (1.0f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
	result.z = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.0f - (num4 + num5)) * point.z;
	return result;
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
	os << q.x << ' ' << q.y << ' ' << q.z << ' ' << q.w << ' ';
	return os;
}

std::istream& operator>>(std::istream& is, Quaternion& q) {
	is >> q.x >> q.y >> q.z >> q.w;
	return is;
}

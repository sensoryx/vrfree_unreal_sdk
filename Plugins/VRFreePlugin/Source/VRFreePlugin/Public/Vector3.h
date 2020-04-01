#ifndef _VECTOR3_H_
#define _VECTOR3_H_

struct Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float xx, float yy, float zz);

	void right();
	void up();
	void forward();

	void add(Vector3* v);
	void subtract(Vector3* v);
	void multiply(float v);
	float dot(Vector3* v);
	Vector3 normalized();
	void Normalize();

	float sqrMagnitude();
	float magnitude();

	static float Magnitude(Vector3 vector);
	static float Dot(Vector3 lhs, Vector3 rhs);
	static Vector3 Cross(Vector3 lhs, Vector3 rhs);
	static float Angle(Vector3 from, Vector3 to);
	static float SignedAngle(Vector3 from, Vector3 to, Vector3 axis);

	float& operator[] (int i) {
		switch(i) {
		case 0:
			return x;
		case 1:
			return y;
		default:
			return z;
		}
	}
};

Vector3 operator+(const Vector3 & a, const Vector3 & b);
Vector3 operator-(const Vector3 & a, const Vector3 & b);

Vector3 operator*(const Vector3& a, const float& d);
Vector3 operator*(const float& d, const Vector3& a);

#endif //_VECTOR3_H_

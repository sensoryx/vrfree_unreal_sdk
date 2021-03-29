#pragma once

#include "Vector3.h"
#include "Vector.h"

#include "Quaternion.h"
#include "Quat.h"

static inline FVector VectorFromVRFree(Vector3 in)
{
	return FVector(in.z, in.x, in.y) * 100.f;
}

static inline Vector3 VectorToVRFree(FVector in)
{
	return Vector3 { in.Y / 100.0f, in.Z / 100.0f, in.X / 100.0f };
}

static inline FQuat QuatFromVRFree(Quaternion in)
{
	FQuat result = FQuat(in.z, in.x, in.y, in.w);
	return result;
}

static inline Quaternion QuatToVRFree(FQuat in)
{
	return { in.Y, in.Z, in.X, in.W };
}
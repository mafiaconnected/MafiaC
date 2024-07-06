#include "stdafx.h"
#include "VectorTools.h"

using namespace M2;

#define Clamp(value, min, max) if (value < min) value = min; else if (value > max) value = max;

CVector3D CVecToolsII::ConvertFromMafiaVec(const Vector3& vec)
{
	CVector3D v;

	v.x = vec.x;
	v.y = vec.y;
	v.z = vec.z;

	return v;
}

float CVecToolsII::Lerp(float a, float b, float t)
{
	Clamp(t, 0.0f, 1.0f);

	return a + (b - a) * t;
}

CVector3D CVecToolsII::Lerp(const CVector3D& a, const CVector3D& b, float t)
{
	CVector3D vec;

	vec.x = Lerp(a.x, b.x, t);
	vec.y = Lerp(a.y, b.y, t);
	vec.z = Lerp(a.z, b.z, t);

	return vec;
}

CVector3D CVecToolsII::Slerp(const CVector3D& a, const CVector3D& b, float t)
{
	float dot = a.dotProduct(b);

	Clamp(dot, -1.0f, 1.0f);

	float euler = acosf(dot) * t;

	CVector3D relVec = b - a * dot;
	relVec.normalise();

	return ((a * cosf(euler)) + (relVec * sinf(euler)));
}

CVector3D CVecToolsII::Nlerp(const CVector3D& a, const CVector3D& b, float t)
{
	auto vec = Lerp(a, b, t);

	vec.normalise();

	return vec;
}

Vector3 CVecToolsII::ConvertToMafiaVec(const CVector3D& vec)
{
	Vector3 v;
	v.x = vec.x;
	v.y = vec.y;
	v.z = vec.z;

	return v;
}

Quaternion CVecToolsII::ConvertVec3ToMafiaQuat(const CVector3D& vec)
{
	Vector3 v;
	v.x = vec.x;
	v.y = vec.y;
	v.z = vec.z;

	return Quaternion(v);
}

/*
void CVecTools::Rotate(double heading, double attitude, double bank, S_quat& quat) {
	// Assuming the angles are in radians.
	double c1 = cos(heading / 2.0f);
	double s1 = sin(heading / 2.0f);
	double c2 = cos(attitude / 2.0f);
	double s2 = sin(attitude / 2.0f);
	double c3 = cos(bank / 2.0f);
	double s3 = sin(bank / 2.0f);
	double c1c2 = c1 * c2;
	double s1s2 = s1 * s2;
	quat.w = c1c2 * c3 - s1s2 * s3;
	quat.x = c1c2 * s3 + s1s2 * c3;
	quat.y = s1 * c2 * c3 + c1 * s2 * s3;
	quat.z = c1 * s2 * c3 - s1 * c2 * s3;
}
*/
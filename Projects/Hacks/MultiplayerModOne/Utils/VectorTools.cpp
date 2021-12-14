#include "stdafx.h"
#include "VectorTools.h"

using namespace MafiaSDK;

#define Clamp(value, min, max) if (value < min) value = min; else if (value > max) value = max;

CMatrix3x3::CMatrix3x3(const CVector3D & euler)
{
	CVector3D x1 = { 1, 0, 0 };
	CVector3D x2 = { 0, cos(euler[0]), -sin(euler[0]) };
	CVector3D x3 = { 0, sin(euler[0]), cos(euler[0]) };

	CVector3D y1 = { cos(euler[1]), 0, sin(euler[1]) };
	CVector3D y2 = { 0, 1, 0 };
	CVector3D y3 = { -sin(euler[1]), 0, cos(euler[1]) };

	CVector3D z1 = { cos(euler[2]), -sin(euler[2]), 0, };
	CVector3D z2 = { sin(euler[2]), cos(euler[2]), 0, };
	CVector3D z3 = { 0, 0, 1 };

	m_rotX = x1 * y1 * z1;
	m_rotY = x2 * y2 * z2;
	m_rotZ = x3 * y3 * z3;
}

CMatrix3x3::CMatrix3x3(const CVector3D& front, const CVector3D& up, const CVector3D& right)
{
	m_rotX = front;
	m_rotY = up;
	m_rotZ = right;
}

CVector3D CMatrix3x3::GetEuler()
{
	if (CMatrix3x3::CloseEnough(m_rotX.z, -1.0f)) {
		float x = 0.0f;
		float y = PI / 2.0f;
		float z = x + atan2(m_rotY[0], m_rotZ[0]);
		return { x, y, z };
	}
	else if (CloseEnough(m_rotX.z, 1.0f)) {
		float x = 0.0f;
		float y = -PI / 2.0f;
		float z = -x + atan2(-m_rotY[0], -m_rotZ[0]);
		return { x, y, z };
	}
	else { //two solutions exist
		float x1 = -asin(m_rotX[2]);
		float x2 = PI - x1;

		float y1 = atan2(m_rotY[2] / cos(x1), m_rotZ[2] / cos(x1));
		float y2 = atan2(m_rotY[2] / cos(x2), m_rotZ[2] / cos(x2));

		float z1 = atan2(m_rotX[1] / cos(x1), m_rotX[0] / cos(x1));
		float z2 = atan2(m_rotX[1] / cos(x2), m_rotX[0] / cos(x2));

		if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2))) {
			return { x1, y1, z1 };
		}
		else {
			return { x2, y2, z2 };
		}
	}
}

CVector3D CMatrix3x3::GetXAxis()
{
	return m_rotX;
}

CVector3D CMatrix3x3::GetYAxis()
{
	return m_rotY;
}

CVector3D CMatrix3x3::GetZAxis()
{
	return m_rotZ;
}

bool CMatrix3x3::CloseEnough(const float& a, const float& b, const float& epsilon)
{
	return (epsilon > std::abs(a - b));
}

CVector3D CVecTools::ConvertFromMafiaVec(const S_vector& vec)
{
	CVector3D v;

	v.x = vec.x;
	v.y = vec.y;
	v.z = vec.z;

	return v;
}

float CVecTools::Lerp(float a, float b, float t)
{
	Clamp(t, 0.0f, 1.0f);

	return a + (b - a) * t;
}

CVector3D CVecTools::Lerp(const CVector3D& a, const CVector3D& b, float t)
{
	CVector3D vec;

	vec.x = Lerp(a.x, b.x, t);
	vec.y = Lerp(a.y, b.y, t);
	vec.z = Lerp(a.z, b.z, t);

	return vec;
}

CVector3D CVecTools::Slerp(const CVector3D& a, const CVector3D& b, float t)
{
	float dot = a.dotProduct(b);

	Clamp(dot, -1.0f, 1.0f);

	float euler = acosf(dot) * t;

	CVector3D relVec = b - a * dot;
	relVec.normalise();

	return ((a * cosf(euler)) + (relVec * sinf(euler)));
}

CVector3D CVecTools::Nlerp(const CVector3D& a, const CVector3D& b, float t)
{
	auto vec = Lerp(a, b, t);

	vec.normalise();

	return vec;
}

S_vector CVecTools::ConvertToMafiaVec(const CVector3D& vec)
{
	S_vector v;
	v.x = vec.x;
	v.y = vec.y;
	v.z = vec.z;

	return v;
}

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
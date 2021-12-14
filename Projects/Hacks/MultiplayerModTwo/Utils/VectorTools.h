#pragma once

class CMatrix3x3
{
private:
	CVector3D m_rotX;
	CVector3D m_rotY;
	CVector3D m_rotZ;

	static bool CloseEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon());

public:
	CMatrix3x3(const CVector3D& euler);
	CMatrix3x3(const CVector3D& front, const CVector3D& up, const CVector3D& right);
	
	CVector3D GetXAxis();
	CVector3D GetYAxis();
	CVector3D GetZAxis();

	CVector3D operator[](int index)
	{
		switch (index)
		{
		case 0: return GetXAxis();
		case 1: return GetYAxis();
		case 2: return GetZAxis();
		default: return { -1, -1, -1 };
		}
	}

	CVector3D GetEuler();
};

class CVecTools
{
private:
	

public:
	static CVector3D GetAbsoluteValues(const CVector3D& vec)
	{
		CVector3D v;

		v.x = abs(vec.x);
		v.y = abs(vec.y);
		v.z = abs(vec.z);

		return v;
	}

	static CVector3D ComputeDirEuler(float angle)
	{
		CVector3D dir;

		dir.x = (float)(sin(2.0f * M_PI * angle));
		dir.y = 0.0f;
		dir.z = (float)(cos(2.0f * M_PI * angle));

		return dir;
	}

	static CVector3D EulerToDir(const CVector3D& euler)
	{
		CVector3D dir;

		dir.x = euler.x / 360.0f;
		dir.y = euler.y / 360.0f;
		dir.z = euler.z / 360.0f;

		return dir;
	}

	static CVector3D DirToEuler(const CVector3D& dir)
	{
		CVector3D euler;

		euler.x = dir.x * 360.0f;
		euler.y = dir.y * 360.0f;
		euler.z = dir.z * 360.0f;

		return euler;
	}

	static CVector3D ComputeDirVector(float angle)
	{
		CVector3D dir;

		dir.x = (float)(sin(2.0f * M_PI * (angle / 360.0f)));
		dir.y = 0.0f;
		dir.z = (float)(cos(2.0f * M_PI * (angle / 360.0f)));

		return dir;
	}

	static float DirToRotation180(const CVector3D& dir)
	{
		float rad = atan2f(dir.x, dir.z);

		return rad * (180.0f / M_PI);
	}

	static float DirToRotation360(const CVector3D& dir)
	{
		float rad = atan2f(dir.x, dir.z);

		float val =  rad * (180.0f / M_PI);

		if (val < 0.0f) val += 360.0f;

		return val;
	}

	static CVector3D GetEuler(const CVector3D& front, const CVector3D& right, const CVector3D& up);
	static CVector3D* GetMatrix(const CVector3D& euler);

	static float Lerp(float a, float b, float t);

	static CVector3D Lerp(const CVector3D& a, const CVector3D& b, float t);
	static CVector3D Slerp(const CVector3D& a, const CVector3D& b, float t);
	static CVector3D Nlerp(const CVector3D& a, const CVector3D& b, float t);

	static Vector3 ConvertToMafiaVec(const CVector3D& vec);
	static CVector3D ConvertFromMafiaVec(const Vector3& vec);

	static Quaternion CVecTools::ConvertVec3ToMafiaQuat(const CVector3D& vec)

	//static void Rotate(double heading, double attitude, double bank, S_quat& quat);
};
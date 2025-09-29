#pragma once
#include "common.h"
#include <math.h>

#define PI       3.1415926535897932384626433832795029f // PI definition
#define MRG_ZERO 1e-8f // Marginal zero - threshold for values considered zero

template<class T> const T& Max(const T& t1, const T& t2) { return t1 > t2 ? t1 : t2; }
template<class T> const T& Min(const T& t1, const T& t2) { return t1 < t2 ? t1 : t2; }

template<class T> inline void Swap(T& l, T& r) {
    T tmp = r;
    r = l;
    l = tmp;
}

inline float I3DFabs(float f) {
    *(uint32_t*)&f &= 0x7fffffff;
    return f;
}

struct S_matrix;
struct S_quat;

struct S_vector2 {
    float x;
    float y;

    __declspec(dllexport) float __stdcall AngleTo(S_vector2 const&) const;
};

struct S_vector4 {
    S_vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    S_vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

    float x;
    float y;
    float z;
    float w;
};

struct S_vector {
    S_vector(float x, float y, float z) : x(x), y(y), z(z) {}
    S_vector() : x(0.0f), y(0.0f), z(0.0f) {}

    inline S_vector operator*(float const& scalar) {
        return S_vector(x * scalar, y * scalar, z * scalar);
    }

    inline void operator*=(float const& scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
    }

    inline void operator/=(float const& scalar) {
        if (I3DFabs(scalar) > MRG_ZERO) {
            x /= scalar;
            y /= scalar;
            z /= scalar;
        }
    }

    inline S_vector operator*(S_vector const& vec) const {
        return S_vector(x * vec.x, y * vec.y, z * vec.z);
    }

    inline S_vector& operator*=(S_vector const& vec) {
        x *= vec.x;
        y *= vec.y;
        z *= vec.z;
        return *this;
    }

    __declspec(dllexport) S_vector __stdcall operator*(S_matrix const& mat);
    __declspec(dllexport) S_vector& __stdcall operator*=(S_matrix const& mat);

    inline S_vector operator+(S_vector const& vec) {
        return S_vector(x + vec.x, y + vec.y, z + vec.z);
    }

    inline void operator+=(S_vector const& vec) {
        x += vec.x;
        y += vec.y;
        z += vec.z;
    }

    inline float Dot(S_vector const& vec) {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    inline S_vector operator-(S_vector const& vec) const {
        return S_vector(x - vec.x, y - vec.y, z - vec.z);
    }

    inline void GetNormal(S_vector const& vec1, S_vector const& vec2, S_vector const& vec3) {
        S_vector v1 = vec2 - vec1;
        S_vector v2 = vec3 - vec1;
        *this = v1.Cross(v2);
        SetNormalized(*this);
    }

    inline float Magnitude2() const {
        return x * x + y * y + z * z;
    }

    inline float Magnitude() const {
        return sqrtf(Magnitude2());
    }

    inline void SetNormalized(S_vector const& vec) {
        double mag = vec.Magnitude();
        if (mag > MRG_ZERO) {
            x = static_cast<float>(vec.x / mag);
            y = static_cast<float>(vec.y / mag);
            z = static_cast<float>(vec.z / mag);
        }
        else {
            x = y = z = 0.0f;
        }
    }

    inline S_vector Cross(S_vector const& v) const {
        return S_vector(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    __declspec(dllexport) float __stdcall CosAngleTo(S_vector const& v) const;
    __declspec(dllexport) float __stdcall AngleTo(S_vector const& v) const;
    __declspec(dllexport) S_vector __stdcall RotateByMatrix(S_matrix const& mat) const;
    __declspec(dllexport) S_vector __stdcall RotateByNormMatrix(S_matrix const& mat) const;

    float x;
    float y;
    float z;
};

// static_assert(sizeof(S_vector) == 0xC);

struct S_quat {
    S_quat() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
    S_quat(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

    __declspec(dllexport) S_matrix __stdcall RotationMatrix() const;
    __declspec(dllexport) S_vector __stdcall GetDir() const;
    __declspec(dllexport) void __stdcall SetDir(S_vector const& q, float);
    __declspec(dllexport) void __stdcall Make(S_vector const& axis, float angle);
    __declspec(dllexport) void __stdcall Make(S_matrix const& mat);

    inline S_quat operator*(S_quat const& quat) {
        return S_quat(
            w * quat.w - x * quat.x - y * quat.y - z * quat.z,
            w * quat.x + x * quat.w + y * quat.z - z * quat.y,
            w * quat.y - x * quat.z + y * quat.w + z * quat.x,
            w * quat.z + x * quat.y - y * quat.x + z * quat.w
        );
    }

    __declspec(dllexport) void __stdcall Inverse(S_vector& vec, float& val) const;
    __declspec(dllexport) S_quat __stdcall Slerp(S_quat const& q, float t, bool shorten) const;
    __declspec(dllexport) S_quat __stdcall RotateByMatrix(S_matrix const& mat) const;
    __declspec(dllexport) void __stdcall Normalize();

    inline float Dot(const S_quat& v) const {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    float w;
    float x;
    float y;
    float z;
};

// static_assert(sizeof(S_quat) == 0x10);

struct S_plane {
    __declspec(dllexport) bool __stdcall IntersectionLine(const S_plane& plane, S_vector& v1, S_vector& v2) const;
};

struct S_matrix {
    __declspec(dllexport) void __stdcall Identity();
    __declspec(dllexport) bool __stdcall Inverse(S_matrix const& mat);
    __declspec(dllexport) S_matrix __stdcall RotateByMatrix(S_matrix const& mat) const;
    __declspec(dllexport) S_matrix __stdcall operator*(S_matrix const& mat) const;
    __declspec(dllexport) S_matrix& __stdcall operator*=(S_matrix const& mat);
    __declspec(dllexport) S_matrix __stdcall Mult4X4(S_matrix const& mat) const;

    inline S_matrix& Make4X4(S_matrix const& mat1, S_matrix const& mat2) {
        // Assuming concatenation of transformations (mat1 * mat2)
        *this = mat1 * mat2;
        return *this;
    }

    __declspec(dllexport) void __stdcall SetDir(S_vector const& dir);
    __declspec(dllexport) void __stdcall SetDir3(S_vector const& v1, S_vector const& v2);
    __declspec(dllexport) void __stdcall SetDir(S_vector const& v1, S_vector const& v2);
    __declspec(dllexport) void __stdcall SetRot3(S_quat const& rot);
    __declspec(dllexport) void __stdcall SetRot(S_quat const& rot);
    __declspec(dllexport) void __stdcall SetRot(S_matrix const& mat);
    __declspec(dllexport) S_vector __stdcall GetScale() const;
    __declspec(dllexport) S_vector __stdcall GetScale2() const;
    __declspec(dllexport) float __stdcall GetUScale() const;

    union {
        struct {
            float m_11, m_12, m_13, m_14;
            float m_21, m_22, m_23, m_24;
            float m_31, m_32, m_33, m_34;
            float m_41, m_42, m_43, m_44;
        };
        float m_fData[16];
        float e[16];
    };
};
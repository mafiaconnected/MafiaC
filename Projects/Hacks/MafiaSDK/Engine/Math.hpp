/* 
* Engine math functions
* WARNING Be carefull works only for mafia ( mafia game addresses of dll import )
*/
#pragma once

struct S_matrix;

struct S_vector2
{
    float x;
    float y;
};

struct S_vector
{
public:
	S_vector operator*=(const S_matrix& mat)
	{
		DWORD S_vector__mul_equal_operator = 0x0060FC54;
		__asm {
			mov eax, this
			push mat
			push eax
			call S_vector__mul_equal_operator
		}
	}

	S_vector operator*(const S_matrix& mat)
	{
		DWORD S_vector__mul_operator = 0x0060FC60;
		__asm {
			mov eax, this
			push mat
			push eax
			call S_vector__mul_operator
		}
	}

	void RotateByMatrix(const S_matrix& mat)
	{
		DWORD S_vector__rotate_by_matrix = 0x0060FC66;
		__asm {
			mov eax, this
			push mat
			push eax
			call S_vector__rotate_by_matrix
		}
	}

	void AngleTo(const S_vector& vec)
	{
		DWORD S_vector__angle_to = 0x0060FC7E;
		__asm {
			mov eax, this
			push vec
			push eax
			call S_vector__angle_to
		}
	}

	void RotateByNormMatrix(const S_matrix& mat)
	{
		DWORD S_vector__rotate_by_norm_matrix = 0x0060FCE4;
		__asm {
			mov eax, this
			push mat
			push eax
			call S_vector__rotate_by_norm_matrix
		}
	}

	union {
		struct {
			float x, y, z;
		};
		float values[3];
	};
};

struct S_quat;
struct S_matrix
{
public:
	S_matrix operator*(const S_matrix& mat)
	{
		DWORD S_matrix__mul_operator = 0x0060FC96;
		__asm {
			mov eax, this
			push mat
			push eax
			call S_matrix__mul_operator
		}
	}

	S_matrix operator*=(const S_matrix& mat)
	{
		DWORD S_matrix__mul_qual_operator = 0x0060FCC0;
		__asm {
			mov eax, this
			push mat
			push eax
			call S_matrix__mul_qual_operator
		}
	}

	S_vector GetScale()
	{
        S_vector to_get;
		DWORD S_matrix__get_scale = 0x0060FC42;
		__asm {
            mov eax, this
            push eax
            lea eax, to_get;
            push eax
			call S_matrix__get_scale
		}
	}

	void SetRot3(const S_quat& rot)
	{
		DWORD S_matrix__set_rot3 = 0x0060FC5A;
		__asm {
			mov eax, this
			push rot
			push eax
			call S_matrix__set_rot3
		}
	}

	void Identity()
	{
		DWORD S_matrix__identity = 0x0060FC6C;
		__asm {
			mov eax, this
			push eax
			call S_matrix__identity
		}
	}

	void Inverse(const S_matrix& mat)
	{
		DWORD S_matrix__inverse = 0x0060FC72;
		__asm {
			mov eax, this
			push mat
			push eax
			call S_matrix__inverse
		}
	}

	void SetDir3(const S_vector& dir_one, const S_vector& dir_two)
	{
		DWORD S_matrix__setdir3 = 0x0060FC90;
		__asm {
			mov eax, this
			push dir_two
			push dir_one
			push eax
			call S_matrix__setdir3
		}
	}

	void SetRot(const S_quat& rot)
	{
		DWORD S_matrix__set_rot = 0x0060FCD2;
		__asm {
			mov eax, this
			push rot
			push eax
			call S_matrix__set_rot
		}		
	}

	float GetUScale()
	{
		DWORD S_matrix__get_uscale = 0x0060FCEA;
		__asm {
			mov eax, this
			push eax
			call S_matrix__get_uscale
		}
	}

	void SetDir(const S_vector& dir)
	{
		DWORD S_matrix__set_dir = 0x0060FCF0;
		__asm {
			mov eax, this
			push dir
			push eax
			call S_matrix__set_dir
		}
	}

	float values[16];
};

struct S_quat
{
public:
	void Make(const S_matrix& mat)
	{
		DWORD S_quat__make = 0x0060FC36;
		__asm {
			mov eax, this
			push mat
			push eax
			call S_quat__make
		}
	}

	void Normalize()
	{
		DWORD S_quat__normalize = 0x0060FC3C;
		__asm {
			mov eax, this
			push eax
			call S_quat__normalize
		}
	}

	void Slerp(const S_quat& dest, float delta, BOOL unk)
	{
		DWORD S_quat__slerp = 0x0060FC4E;
		__asm {
			mov eax, this
			push unk 
			push delta
			push dest
			push eax
			call S_quat__slerp
		}
	}

	void Make(const S_vector& from, float angle)
	{
		DWORD S_quat__make_euler = 0x0060FC78;
		__asm {
			mov eax, this
			push angle
			push from
			push eax
			call S_quat__make_euler
		}
	}

	S_matrix RotationMatrix()
	{
		DWORD S_quat__rotation_matrix = 0x0060FC84;
		__asm {
			mov eax, this
			push eax
			call S_quat__rotation_matrix
		}
	}

	void SetDir(const S_vector& from, float angle)
	{
		DWORD S_quat__set_dir = 0x0060FC8A;
		__asm {
			mov eax, this
			push angle
			push from
			push eax
			call S_quat__set_dir
		}
	}

	S_vector GetDir()
	{
		DWORD S_quat__get_dir = 0x0060FCA2;
		__asm {
			mov eax, this
			push eax
			call S_quat__get_dir
		}
	}

	void Inverse(const S_vector& from, float angle)
	{
		DWORD S_quat__inverse = 0x0060FCB4;
		__asm {
			mov eax, this
			push angle
			push from
			push eax
			call S_quat__inverse
		}
	}

	void RotateByMatrix(const S_matrix& mat)
	{
		DWORD S_quat__rotate_by_matrix = 0x0060FCBA;
		__asm {
			mov eax, this
			push mat
			push eax
			call S_quat__rotate_by_matrix
		}
	}

	union {
		struct {
			float x, y, z, w;
		};
		float values[4];
	};
};

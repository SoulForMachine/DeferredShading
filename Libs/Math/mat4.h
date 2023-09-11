
/* ----------------------------------------
	File: mat4.h
	Purpose: 4x4 matrix class
	Author: Milan D.
	Date: 01.05.2005
   ---------------------------------------- */


#ifndef _MATH3D_MAT4_H_
#define _MATH3D_MAT4_H_

#include <cstring>
#include <type_traits>
#include <utility>
#include "vec3.h"

/*

Elements are ordered in memory like this:

    0    1    2    3

    4    5    6    7

    8    9    10   11

    12   13   14   15

What elements represent:

                allways [0 0 0 1] for
                orthogonal matrix
                                |
                                V
                 +----------------+
    x axes ->    | Xx  Xy  Xz | 0 |
                 +------------+   |
    y axes ->    | Yx  Yy  Yz | 0 |
                 +------------+   |
    z axes ->    | Zx  Zy  Zz | 0 |
                 +------------+   |
    origin ->    | Tx  Ty  Tz | 1 |
   (transl vec)  +------------+---+
*/

namespace math3d
{

template <class _ST> class vec3;
template <class _ST> class vec4;
template <class _ST> class mat3;


template <class _ST>
class mat4
{
public:
	static_assert(std::is_arithmetic_v<_ST>, "Scalar type _ST must be floating point or integral type.");

	typedef _ST scalar_t;

	static constexpr size_t num_elements = 16;

	constexpr mat4() = default;

	constexpr mat4(_ST m0, _ST m1, _ST m2, _ST m3, 
					_ST m4, _ST m5, _ST m6, _ST m7,
					_ST m8, _ST m9, _ST m10, _ST m11,
					_ST m12, _ST m13, _ST m14, _ST m15);

	constexpr mat4(const vec4<_ST>& row0, const vec4<_ST>& row1, const vec4<_ST>& row2, const vec4<_ST>& row3);

	// conversion constructors
	explicit constexpr mat4(const mat3<_ST>& m3);
	explicit constexpr mat4(const _ST* m);

	template <class _T>
	explicit constexpr mat4(const mat4<_T>& m);

	// access
	vec4<_ST>& operator [] (int row_index);
	constexpr const vec4<_ST>& operator [] (int row_index) const;

	_ST& operator () (int index);
	constexpr _ST operator () (int index) const;

	operator _ST* () { return _data; }
	constexpr operator const _ST* () const { return _data; }

	void set(_ST m0, _ST m1, _ST m2, _ST m3,
			_ST m4, _ST m5, _ST m6, _ST m7,
			_ST m8, _ST m9, _ST m10, _ST m11,
			_ST m12, _ST m13, _ST m14, _ST m15);

	void set(const vec4<_ST>& row0, const vec4<_ST>& row1, const vec4<_ST>& row2, const vec4<_ST>& row3);
	void set(const mat3<_ST>& m3);
	// set only 3x3 submatrix
	void set3x3(const mat3<_ST>& m3);
	void get(vec4<_ST>& row0, vec4<_ST>& row1, vec4<_ST>& row2, vec4<_ST>& row3) const;
	void get(mat3<_ST>& m3) const;
	constexpr vec4<_ST> get_col(int col) const;


	// operations
	constexpr _ST determinant() const;
	void transpose();
	void inverse();
	void inverse_transpose();
	void adjoint();
	void get_transpose(mat4& m) const;
	void get_inverse(mat4& m) const;
	void get_inverse_transpose(mat4& m) const;
	void get_adjoint(mat4& m) const;
	void set_identity();
	void set_transpose(const mat4& m);
	void set_inverse(const mat4& m);
	void set_inverse_transpose(const mat4& m);
	void set_adjoint(const mat4& m);

	// transforms
	void translate(const vec3<_ST>& transl);
	void translate(_ST _x, _ST _y, _ST _z);
	void rotate(_ST angle, _ST _x, _ST _y, _ST _z);
	void rotate(_ST angle, const vec3<_ST>& axis);
	void rotate_x(_ST angle);
	void rotate_y(_ST angle);
	void rotate_z(_ST angle);
	void from_euler(_ST x_angle, _ST y_angle, _ST z_angle);
	void to_euler(_ST& x_angle, _ST& y_angle, _ST& z_angle);
	void scale(_ST s);
	void scale(_ST _x, _ST _y, _ST _z);
	void scale(const vec3<_ST>& s);
	void set_translation(const vec3<_ST>& transl);
	void set_translation(_ST _x, _ST _y, _ST _z);
	void set_rotation(_ST angle, _ST _x, _ST _y, _ST _z);
	void set_rotation(_ST angle, const vec3<_ST>& axis);
	void set_rotation_x(_ST angle);
	void set_rotation_y(_ST angle);
	void set_rotation_z(_ST angle);
	void set_scale(_ST s);
	void set_scale(_ST _x, _ST _y, _ST _z);
	void set_scale(const vec3<_ST>& s);

	// these camera functions work with right handed coordinate system
	void ortho(_ST left, _ST right, _ST bottom, _ST top, _ST znear, _ST zfar);
	void frustum(_ST left, _ST right, _ST bottom, _ST top, _ST znear, _ST zfar);
	void frustum_inf(_ST left, _ST right, _ST bottom, _ST top, _ST znear);
	void perspective(_ST fovy, _ST aspect, _ST znear, _ST zfar);
	void look_at(const vec3<_ST>& eye, const vec3<_ST>& at, const vec3<_ST>& up);

private:
	void _copy(const _ST* src);
	static void _inverse(_ST* dest, const _ST* src);
	static void _transpose(_ST* dest, const _ST* src);
	static void _inverse_transpose(_ST* dest, const _ST* src);
	static void _adjoint(_ST* dest, const _ST* src);

	_ST _data[num_elements];
};


typedef mat4<float> mat4f;
typedef mat4<double> mat4d;
typedef mat4<long double> mat4ld;
typedef mat4<int> mat4i;
typedef mat4<unsigned int> mat4ui;


} // namespace math3d

// ------------------------------------

#include "vec3.h"
#include "mat3.h"
#include "mat4.h"


namespace math3d
{

template <class _ST>
constexpr mat4<_ST> mat4_identity {
	_ST(1), _ST(0), _ST(0), _ST(0),
	_ST(0), _ST(1), _ST(0), _ST(0),
	_ST(0), _ST(0), _ST(1), _ST(0),
	_ST(0), _ST(0), _ST(0), _ST(1)
};


template <class _ST>
constexpr
mat4<_ST>::mat4(_ST m0, _ST m1, _ST m2, _ST m3, 
				_ST m4, _ST m5, _ST m6, _ST m7,
				_ST m8, _ST m9, _ST m10, _ST m11,
				_ST m12, _ST m13, _ST m14, _ST m15)
	: _data {
		m0, m1, m2, m3,
		m4, m5, m6, m7,
		m8, m9, m10, m11,
		m12, m13, m14, m15
	}
{
}

template <class _ST>
constexpr
mat4<_ST>::mat4(const vec4<_ST>& row0, const vec4<_ST>& row1, const vec4<_ST>& row2, const vec4<_ST>& row3)
	: _data {
		row0.x, row0.y, row0.z, row0.w,
		row1.x, row1.y, row1.z, row1.w,
		row2.x, row2.y, row2.z, row2.w,
		row3.x, row3.y, row3.z, row3.w,
	}
{
}

// conversion constructors
template <class _ST>
constexpr
mat4<_ST>::mat4(const mat3<_ST>& m3)
	: _data {
		m3(0), m3(1), m3(2), _ST(0),
		m3(3), m3(4), m3(5), _ST(0),
		m3(6), m3(7), m3(8), _ST(0),
		_ST(0), _ST(0), _ST(0), _ST(1)
	}
{
}

template <class _ST>
constexpr
mat4<_ST>::mat4(const _ST* m)
	: _data {
		m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]
	}
{
}

template <class _ST>
template <class _T>
constexpr
mat4<_ST>::mat4(const mat4<_T>& m)
	: _data {
		_ST(m(0)), _ST(m(1)), _ST(m(2)), _ST(m(3)),
		_ST(m(4)), _ST(m(5)), _ST(m(6)), _ST(m(7)),
		_ST(m(8)), _ST(m(9)), _ST(m(10)), _ST(m(11)),
		_ST(m(12)), _ST(m(13)), _ST(m(14)), _ST(m(15))
	}
{
}

// access

template <class _ST>
inline
vec4<_ST>& mat4<_ST>::operator [] (int row_index)
{
	return *(vec4<_ST>*) (_data + (row_index << 2));
}

template <class _ST>
constexpr
const vec4<_ST>& mat4<_ST>::operator [] (int row_index) const
{
	return *(vec4<_ST>*) (_data + (row_index << 2));
}

template <class _ST>
inline
_ST& mat4<_ST>::operator () (int index)
{
	return _data[index];
}

template <class _ST>
constexpr
_ST mat4<_ST>::operator () (int index) const
{
	return _data[index];
}

template <class _ST>
inline
void mat4<_ST>::set(_ST m0, _ST m1, _ST m2, _ST m3,
					_ST m4, _ST m5, _ST m6, _ST m7,
					_ST m8, _ST m9, _ST m10, _ST m11,
					_ST m12, _ST m13, _ST m14, _ST m15)
{
	_data[0] = m0;
	_data[1] = m1;
	_data[2] = m2;
	_data[3] = m3;
	_data[4] = m4;
	_data[5] = m5;
	_data[6] = m6;
	_data[7] = m7;
	_data[8] = m8;
	_data[9] = m9;
	_data[10] = m10;
	_data[11] = m11;
	_data[12] = m12;
	_data[13] = m13;
	_data[14] = m14;
	_data[15] = m15;
}

template <class _ST>
void mat4<_ST>::set(const vec4<_ST>& row0, const vec4<_ST>& row1, const vec4<_ST>& row2, const vec4<_ST>& row3)
{
	(*this)[0] = row0;
	(*this)[1] = row1;
	(*this)[2] = row2;
	(*this)[3] = row3;
}

template <class _ST>
void mat4<_ST>::set(const mat3<_ST>& m3)
{
	(*this)[0].set(m3.operator[](0), _ST(0));
	(*this)[1].set(m3[1], _ST(0));
	(*this)[2].set(m3[2], _ST(0));
	(*this)[3].set(_ST(0), _ST(0), _ST(0), _ST(1));
}

template <class _ST>
void mat4<_ST>::set3x3(const mat3<_ST>& m3)
{
	(*this)[0].rvec3() = m3[0];
	(*this)[1].rvec3() = m3[1];
	(*this)[2].rvec3() = m3[2];
}

template <class _ST>
void mat4<_ST>::get(vec4<_ST>& row0, vec4<_ST>& row1, vec4<_ST>& row2, vec4<_ST>& row3) const
{
	row0 = (*this)[0];
	row1 = (*this)[1];
	row2 = (*this)[2];
	row3 = (*this)[3];
}

template <class _ST>
void mat4<_ST>::get(mat3<_ST>& m3) const
{
	m3[0] = (*this)[0].rvec3();
	m3[1] = (*this)[1].rvec3();
	m3[2] = (*this)[2].rvec3();
}

template <class _ST>
constexpr
vec4<_ST> mat4<_ST>::get_col(int col) const
{
	return { _data[col], _data[col + 4], _data[col + 8], _data[col + 12] };
}



// operations

template <class _ST>
constexpr
_ST mat4<_ST>::determinant() const
{
	_ST sub_10_15_11_14 = _data[10] * _data[15] - _data[11] * _data[14];
	_ST sub_9_15_11_13 = _data[9] * _data[15] - _data[11] * _data[13];
	_ST sub_9_14_10_13 = _data[9] * _data[14] - _data[10] * _data[13];
	_ST sub_8_15_11_12 = _data[8] * _data[15] - _data[11] * _data[12];
	_ST sub_8_14_10_12 = _data[8] * _data[14] - _data[10] * _data[12];
	_ST sub_8_13_9_12 = _data[8] * _data[13] - _data[9] * _data[12];

	_ST det_0_0 = _data[5] * sub_10_15_11_14 - 
				  _data[6] * sub_9_15_11_13 +
				  _data[7] * sub_9_14_10_13;

	_ST det_0_1 = _data[4] * sub_10_15_11_14 - 
				  _data[6] * sub_8_15_11_12 +
				  _data[7] * sub_8_14_10_12;

	_ST det_0_2 = _data[4] * sub_9_15_11_13 - 
				  _data[5] * sub_8_15_11_12 +
				  _data[7] * sub_8_13_9_12;

	_ST det_0_3 = _data[4] * sub_9_14_10_13 - 
				  _data[5] * sub_8_14_10_12 +
				  _data[6] * sub_8_13_9_12;

	return _data[0] * det_0_0 - _data[1] * det_0_1 + _data[2] * det_0_2 - _data[3] * det_0_3;
}

template <class _ST>
inline
void mat4<_ST>::set_identity()
{
	_copy(mat4_identity<_ST>);
}

template <class _ST>
void mat4<_ST>::transpose()
{
	std::swap(_data[1], _data[4]);
	std::swap(_data[2], _data[8]);
	std::swap(_data[3], _data[12]);
	std::swap(_data[6], _data[9]);
	std::swap(_data[7], _data[13]);
	std::swap(_data[11], _data[14]);
}

// inverse of an arbitrary matrix
// WARNING: does not check if the matrix is invertible!
template <class _ST>
inline
void mat4<_ST>::inverse()
{
	_inverse(_data, _data);
}

template <class _ST>
inline
void mat4<_ST>::inverse_transpose()
{
	_inverse_transpose(_data, _data);
}

template <class _ST>
inline
void mat4<_ST>::adjoint()
{
	_adjoint(_data, _data);
}

template <class _ST>
inline
void mat4<_ST>::get_transpose(mat4& m) const
{
	_transpose(m._data, _data);
}

template <class _ST>
inline
void mat4<_ST>::get_inverse(mat4& m) const
{
	_inverse(m._data, _data);
}

template <class _ST>
inline
void mat4<_ST>::get_inverse_transpose(mat4& m) const
{
	_inverse_transpose(m._data, _data);
}

template <class _ST>
inline
void mat4<_ST>::get_adjoint(mat4& m) const
{
	_adjoint(m._data, _data);
}


template <class _ST>
inline
void mat4<_ST>::set_transpose(const mat4& m)
{
	_transpose(_data, m._data);
}

template <class _ST>
inline
void mat4<_ST>::set_inverse(const mat4& m)
{
	_inverse(_data, m._data);
}

template <class _ST>
inline
void mat4<_ST>::set_inverse_transpose(const mat4& m)
{
	_inverse_transpose(_data, m._data);
}

template <class _ST>
inline
void mat4<_ST>::set_adjoint(const mat4& m)
{
	_adjoint(_data, m._data);
}

// transforms

// assumes that it's an affine matrix
template <class _ST>
inline
void mat4<_ST>::translate(const vec3<_ST>& transl)
{
	translate(transl.x, transl.y, transl.z);
}

// assumes that it's an affine matrix
template <class _ST>
void mat4<_ST>::translate(_ST _x, _ST _y, _ST _z)
{
	_data[12] += _x;
	_data[13] += _y;
	_data[14] += _z;
}

template <class _ST>
void mat4<_ST>::rotate(_ST angle, _ST _x, _ST _y, _ST _z)
{
	_ST cos_a = std::cos(angle);
	_ST inv_cos_a = _ST(1) - cos_a;
	_ST inv_cos_a_xy = inv_cos_a * _x * _y;
	_ST inv_cos_a_xz = inv_cos_a * _x * _z;
	_ST inv_cos_a_yz = inv_cos_a * _y * _z;
	_ST sin_a = std::sin(angle);
	_ST sin_a_x = sin_a * _x;
	_ST sin_a_y = sin_a * _y;
	_ST sin_a_z = sin_a * _z;

	_ST rot00 = cos_a + inv_cos_a * _x * _x;
	_ST rot10 = inv_cos_a_xy - sin_a_z;
	_ST rot20 = inv_cos_a_xz + sin_a_y;
	_ST rot01 = inv_cos_a_xy + sin_a_z;
	_ST rot11 = cos_a + inv_cos_a * _y * _y;
	_ST rot21 = inv_cos_a_yz - sin_a_x;
	_ST rot02 = inv_cos_a_xz - sin_a_y;
	_ST rot12 = inv_cos_a_yz + sin_a_x;
	_ST rot22 = cos_a + inv_cos_a * _z * _z;

	mat4<_ST> temp;

	temp(0) = _data[0] * rot00 + _data[1] * rot10 + _data[2] * rot20;
	temp(1) = _data[0] * rot01 + _data[1] * rot11 + _data[2] * rot21;
	temp(2) = _data[0] * rot02 + _data[1] * rot12 + _data[2] * rot22;
	temp(3) = _data[3];

	temp(4) = _data[4] * rot00 + _data[5] * rot10 + _data[6] * rot20;
	temp(5) = _data[4] * rot01 + _data[5] * rot11 + _data[6] * rot21;
	temp(6) = _data[4] * rot02 + _data[5] * rot12 + _data[6] * rot22;
	temp(7) = _data[7];

	temp(8) = _data[8] * rot00 + _data[9] * rot10 + _data[10] * rot20;
	temp(9) = _data[8] * rot01 + _data[9] * rot11 + _data[10] * rot21;
	temp(10) = _data[8] * rot02 + _data[9] * rot12 + _data[10] * rot22;
	temp(11) = _data[11];

	temp(12) = _data[12] * rot00 + _data[13] * rot10 + _data[14] * rot20;
	temp(13) = _data[12] * rot01 + _data[13] * rot11 + _data[14] * rot21;
	temp(14) = _data[12] * rot02 + _data[13] * rot12 + _data[14] * rot22;
	temp(15) = _data[15];

	*this = temp;
}

template <class _ST>
inline
void mat4<_ST>::rotate(_ST angle, const vec3<_ST>& axis)
{
	rotate(angle, axis.x, axis.y, axis.z);
}

template <class _ST>
void mat4<_ST>::rotate_x(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);
	_ST t;

	t = _data[1] * cos_a - _data[2] * sin_a;
	_data[2] = _data[1] * sin_a + _data[2] * cos_a;
	_data[1] = t;

	t = _data[5] * cos_a - _data[6] * sin_a;
	_data[6] = _data[5] * sin_a + _data[6] * cos_a;
	_data[5] = t;

	t = _data[9] * cos_a - _data[10] * sin_a;
	_data[10] = _data[9] * sin_a + _data[10] * cos_a;
	_data[9] = t;

	t = _data[13] * cos_a - _data[14] * sin_a;
	_data[14] = _data[13] * sin_a + _data[14] * cos_a;
	_data[13] = t;
}

template <class _ST>
void mat4<_ST>::rotate_y(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);
	_ST t;

	t = _data[0] * cos_a + _data[2] * sin_a;
	_data[2] = _data[2] * cos_a - _data[0] * sin_a;
	_data[0] = t;

	t = _data[4] * cos_a + _data[6] * sin_a;
	_data[6] = _data[6] * cos_a - _data[4] * sin_a;
	_data[4] = t;

	t = _data[8] * cos_a + _data[10] * sin_a;
	_data[10] = _data[10] * cos_a - _data[8] * sin_a;
	_data[8] = t;

	t = _data[12] * cos_a + _data[14] * sin_a;
	_data[14] = _data[14] * cos_a - _data[12] * sin_a;
	_data[12] = t;
}

template <class _ST>
void mat4<_ST>::rotate_z(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);
	_ST t;

	t = _data[0] * cos_a - _data[1] * sin_a;
	_data[1] = _data[0] * sin_a + _data[1] * cos_a;
	_data[0] = t;

	t = _data[4] * cos_a - _data[5] * sin_a;
	_data[5] = _data[4] * sin_a + _data[5] * cos_a;
	_data[4] = t;

	t = _data[8] * cos_a - _data[9] * sin_a;
	_data[9] = _data[8] * sin_a + _data[9] * cos_a;
	_data[8] = t;

	t = _data[12] * cos_a - _data[13] * sin_a;
	_data[13] = _data[12] * sin_a + _data[13] * cos_a;
	_data[12] = t;
}

template <class _ST>
void mat4<_ST>::from_euler(_ST x_angle, _ST y_angle, _ST z_angle)
{
	_ST cr = std::cos(x_angle);
	_ST sr = std::sin(x_angle);
	_ST cp = std::cos(y_angle);
	_ST sp = std::sin(y_angle);
	_ST cy = std::cos(z_angle);
	_ST sy = std::sin(z_angle);

	_data[0] = cp * cy;
	_data[1] = cp * sy;
	_data[2] = -sp;
	_data[3] = _ST(0);

	_ST srsp = sr * sp;
	_ST crsp = cr * sp;

	_data[4] = srsp * cy - cr * sy;
	_data[5] = srsp * sy + cr * cy;
	_data[6] = sr * cp;
	_data[7] = _ST(0);

	_data[8] = crsp * cy + sr * sy;
	_data[9] = crsp * sy - sr * cy;
	_data[10] = cr * cp;
	_data[11] = _ST(0);

	_data[12] = _ST(0);
	_data[13] = _ST(0);
	_data[14] = _ST(0);
	_data[15] = _ST(1);
}

template <class _ST>
void mat4<_ST>::to_euler(_ST& x_angle, _ST& y_angle, _ST& z_angle)
{
	y_angle = -std::asin(_data[2]);
	_ST cy = std::cos(y_angle);

	float rotx, roty;

	if(fcmp_gt(std::fabs(cy), _ST(0)))
	{
		_ST inv_c = _ST(1) / cy;
		rotx = _data[10] * inv_c;
		roty = _data[6] * inv_c;
		x_angle = atan2(roty, rotx);
		rotx = _data[0] * inv_c;
		roty = _data[1] * inv_c;
		z_angle = atan2(roty, rotx);
	}
	else
	{
		x_angle = _ST(0);
		rotx = _data[5];
		roty = -_data[4];
		z_angle = atan2(roty, rotx);
	}

	if(x_angle < 0.0) x_angle += _ST(TWO_PI);
	if(y_angle < 0.0) y_angle += _ST(TWO_PI);
	if(z_angle < 0.0) z_angle += _ST(TWO_PI);
}

template <class _ST>
void mat4<_ST>::scale(_ST s)
{
	_data[0] *= s;
	_data[1] *= s;
	_data[2] *= s;

	_data[4] *= s;
	_data[5] *= s;
	_data[6] *= s;

	_data[8] *= s;
	_data[9] *= s;
	_data[10] *= s;

	_data[12] *= s;
	_data[13] *= s;
	_data[14] *= s;
}

template <class _ST>
void mat4<_ST>::scale(_ST _x, _ST _y, _ST _z)
{
	_data[0] *= _x;
	_data[1] *= _y;
	_data[2] *= _z;

	_data[4] *= _x;
	_data[5] *= _y;
	_data[6] *= _z;

	_data[8] *= _x;
	_data[9] *= _y;
	_data[10] *= _z;

	_data[12] *= _x;
	_data[13] *= _y;
	_data[14] *= _z;
}

template <class _ST>
void mat4<_ST>::scale(const vec3<_ST>& s)
{
	_data[0] *= s.x;
	_data[1] *= s.y;
	_data[2] *= s.z;

	_data[4] *= s.x;
	_data[5] *= s.y;
	_data[6] *= s.z;

	_data[8] *= s.x;
	_data[9] *= s.y;
	_data[10] *= s.z;

	_data[12] *= s.x;
	_data[13] *= s.y;
	_data[14] *= s.z;
}

template <class _ST>
void mat4<_ST>::set_translation(const vec3<_ST>& transl)
{
	(*this)[0].set(_ST(1), _ST(0), _ST(0), _ST(0));
	(*this)[1].set(_ST(0), _ST(1), _ST(0), _ST(0));
	(*this)[2].set(_ST(0), _ST(0), _ST(1), _ST(0));
	(*this)[3].set(transl);
}

template <class _ST>
void mat4<_ST>::set_translation(_ST _x, _ST _y, _ST _z)
{
	(*this)[0].set(_ST(1), _ST(0), _ST(0), _ST(0));
	(*this)[1].set(_ST(0), _ST(1), _ST(0), _ST(0));
	(*this)[2].set(_ST(0), _ST(0), _ST(1), _ST(0));
	(*this)[3].set(_x, _y, _z);
}

template <class _ST>
void mat4<_ST>::set_rotation(_ST angle, _ST _x, _ST _y, _ST _z)
{
	_ST cos_a = std::cos(angle);
	_ST inv_cos_a = _ST(1) - cos_a;
	_ST inv_cos_a_xy = inv_cos_a * _x * _y;
	_ST inv_cos_a_xz = inv_cos_a * _x * _z;
	_ST inv_cos_a_yz = inv_cos_a * _y * _z;
	_ST sin_a = std::sin(angle);
	_ST sin_a_x = sin_a * _x;
	_ST sin_a_y = sin_a * _y;
	_ST sin_a_z = sin_a * _z;

	_data[0] = cos_a + inv_cos_a * _x * _x;
	_data[4] = inv_cos_a_xy - sin_a_z;
	_data[8] = inv_cos_a_xz + sin_a_y;
	_data[12] = _ST(0);
	_data[1] = inv_cos_a_xy + sin_a_z;
	_data[5] = cos_a + inv_cos_a * _y * _y;
	_data[9] = inv_cos_a_yz - sin_a_x;
	_data[13] = _ST(0);
	_data[2] = inv_cos_a_xz - sin_a_y;
	_data[6] = inv_cos_a_yz + sin_a_x;
	_data[10] = cos_a + inv_cos_a * _z * _z;
	_data[14] = _ST(0);
	_data[3] = _ST(0);
	_data[7] = _ST(0);
	_data[11] = _ST(0);
	_data[15] = _ST(1);
}

template <class _ST>
inline
void mat4<_ST>::set_rotation(_ST angle, const vec3<_ST>& axis)
{
	set_rotation(angle, axis.x, axis.y, axis.z);
}

template <class _ST>
void mat4<_ST>::set_rotation_x(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);

	(*this)[0].set(_ST(1), _ST(0), _ST(0), _ST(0));
	(*this)[1].set(_ST(0), cos_a, sin_a, _ST(0));
	(*this)[2].set(_ST(0), -sin_a, cos_a, _ST(0));
	(*this)[3].set(_ST(0), _ST(0), _ST(0), _ST(1));
}

template <class _ST>
void mat4<_ST>::set_rotation_y(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);

	(*this)[0].set(cos_a, _ST(0), -sin_a, _ST(0));
	(*this)[1].set(_ST(0), _ST(1), _ST(0), _ST(0));
	(*this)[2].set(sin_a, _ST(0), cos_a, _ST(0));
	(*this)[3].set(_ST(0), _ST(0), _ST(0), _ST(1));
}

template <class _ST>
void mat4<_ST>::set_rotation_z(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);

	(*this)[0].set(cos_a, sin_a, _ST(0), _ST(0));
	(*this)[1].set(-sin_a, cos_a, _ST(0), _ST(0));
	(*this)[2].set(_ST(0), _ST(0), _ST(1), _ST(0));
	(*this)[3].set(_ST(0), _ST(0), _ST(0), _ST(1));
}

template <class _ST>
void mat4<_ST>::set_scale(_ST s)
{
	(*this)[0].set(s, _ST(0), _ST(0), _ST(0));
	(*this)[1].set(_ST(0), s, _ST(0), _ST(0));
	(*this)[2].set(_ST(0), _ST(0), s, _ST(0));
	(*this)[3].set(_ST(0), _ST(0), _ST(0), _ST(1));
}

template <class _ST>
void mat4<_ST>::set_scale(_ST _x, _ST _y, _ST _z)
{
	(*this)[0].set(_x, _ST(0), _ST(0), _ST(0));
	(*this)[1].set(_ST(0), _y, _ST(0), _ST(0));
	(*this)[2].set(_ST(0), _ST(0), _z, _ST(0));
	(*this)[3].set(_ST(0), _ST(0), _ST(0), _ST(1));
}

template <class _ST>
void mat4<_ST>::set_scale(const vec3<_ST>& s)
{
	(*this)[0].set(s.x, _ST(0), _ST(0), _ST(0));
	(*this)[1].set(_ST(0), s.y, _ST(0), _ST(0));
	(*this)[2].set(_ST(0), _ST(0), s.z, _ST(0));
	(*this)[3].set(_ST(0), _ST(0), _ST(0), _ST(1));
}

// projection transformation functions

template <class _ST>
void mat4<_ST>::ortho(_ST left, _ST right, _ST bottom, _ST top, _ST znear, _ST zfar)
{
	_ST inv_width = _ST(1) / (right - left);
	_ST inv_height = _ST(1) / (top - bottom);
	_ST inv_depth = _ST(1) / (zfar - znear);

	(*this)[0].set(_ST(2) * inv_width, _ST(0), _ST(0), _ST(0));
	(*this)[1].set(_ST(0), _ST(2) * inv_height, _ST(0), _ST(0));
	(*this)[2].set(_ST(0), _ST(0), -_ST(2) * inv_depth, _ST(0));
	(*this)[3].set(-(right + left) * inv_width, -(top + bottom) * inv_height, -(zfar + znear) * inv_depth, _ST(1));
}

template <class _ST>
void mat4<_ST>::frustum(_ST left, _ST right, _ST bottom, _ST top, _ST znear, _ST zfar)
{
	_ST inv_width = _ST(1) / (right - left);
	_ST inv_height = _ST(1) / (top - bottom);
	_ST inv_depth = _ST(1) / (zfar - znear);
	_ST two_near = _ST(2) * znear;

	(*this)[0].set(two_near * inv_width, _ST(0), _ST(0), _ST(0));
	(*this)[1].set(_ST(0), two_near * inv_height, _ST(0), _ST(0));
	(*this)[2].set((right + left) * inv_width, (top + bottom) * inv_height, -(zfar + znear) * inv_depth, _ST(-1));
	(*this)[3].set(_ST(0), _ST(0), - two_near * zfar * inv_depth, _ST(0));
}

template <class _ST>
void mat4<_ST>::frustum_inf(_ST left, _ST right, _ST bottom, _ST top, _ST znear)
{
	_ST inv_width = _ST(1) / (right - left);
	_ST inv_height = _ST(1) / (top - bottom);
	_ST two_near = _ST(2) * znear;

	(*this)[0].set(two_near * inv_width, _ST(0), _ST(0), _ST(0));
	(*this)[1].set(_ST(0), two_near * inv_height, _ST(0), _ST(0));
	(*this)[2].set((right + left) * inv_width, (top + bottom) * inv_height, _ST(-1), _ST(-1));
	(*this)[3].set(_ST(0), _ST(0), -two_near, _ST(0));
}

template <class _ST>
void mat4<_ST>::perspective(_ST fovy, _ST aspect, _ST znear, _ST zfar)
{
	fovy *= _ST(0.5);
	_ST y_scale = std::cos(fovy) / std::sin(fovy);
	_ST x_scale = y_scale / aspect;
	_ST inv_delta = _ST(1.0) / (zfar - znear);
	_ST z_scale = -(znear + zfar) * inv_delta;

	(*this)[0].set(x_scale, _ST(0), _ST(0), _ST(0));
	(*this)[1].set(_ST(0), y_scale, _ST(0), _ST(0));
	(*this)[2].set(_ST(0), _ST(0), z_scale, _ST(-1));
	(*this)[3].set(_ST(0), _ST(0), _ST(-2.0) * znear * zfar * inv_delta, _ST(0));
}

template <class _ST>
void mat4<_ST>::look_at(const vec3<_ST>& eye, const vec3<_ST>& at, const vec3<_ST>& up)
{
	vec3<_ST> forward = at - eye;
	forward.normalize();

	vec3<_ST> right = cross(forward, up);
	right.normalize();

	// make sure 3 vectors form orthonormal bases
	vec3<_ST> _up = cross(right, forward);

	// for right-handed coordinate system
	forward.negate();

	// rotation 3x3 submatrix
	_data[0] = right.x;
	_data[4] = right.y;
	_data[8] = right.z;

	_data[1] =  _up.x;
	_data[5] = _up.y;
	_data[9] =  _up.z;

	_data[2] = forward.x;
	_data[6] = forward.y;
	_data[10] = forward.z;

	// translation vector
	_data[12] = - dot(right, eye);
	_data[13] = - dot(_up, eye);
	_data[14] = - dot(forward, eye);

	_data[3] = _data[7] = _data[11] = _ST(0);
	_data[15] = _ST(1);
}

// implementation functions

template <class _ST>
inline
void mat4<_ST>::_copy(const _ST* src)
{
	memcpy(_data, src, sizeof(_ST) * num_elements);
}

template <class _ST>
void mat4<_ST>::_transpose(_ST* dest, const _ST* src)
{
	dest[0] = src[0];
	dest[1] = src[4];
	dest[2] = src[8];
	dest[3] = src[12];

	dest[4] = src[1];
	dest[5] = src[5];
	dest[6] = src[9];
	dest[7] = src[13];

	dest[8] = src[2];
	dest[9] = src[6];
	dest[10] = src[10];
	dest[11] = src[14];

	dest[12] = src[3];
	dest[13] = src[7];
	dest[14] = src[11];
	dest[15] = src[15];
}

template <class _ST>
void mat4<_ST>::_inverse(_ST* dest, const _ST* src)
{
	// co-factors
	_ST sub_10_15_11_14 = src[10] * src[15] - src[11] * src[14];
	_ST sub_9_15_11_13 = src[9] * src[15] - src[11] * src[13];
	_ST sub_9_14_10_13 = src[9] * src[14] - src[10] * src[13];
	_ST sub_8_15_11_12 = src[8] * src[15] - src[11] * src[12];
	_ST sub_8_14_10_12 = src[8] * src[14] - src[10] * src[12];
	_ST sub_8_13_9_12 = src[8] * src[13] - src[9] * src[12];

	_ST sub_2_7_3_6 = src[2] * src[7] - src[3] * src[6];
	_ST sub_1_7_3_5 = src[1] * src[7] - src[3] * src[5];
	_ST sub_1_6_2_5 = src[1] * src[6] - src[2] * src[5];
	_ST sub_0_7_3_4 = src[0] * src[7] - src[3] * src[4];
	_ST sub_0_6_2_4 = src[0] * src[6] - src[2] * src[4];
	_ST sub_0_5_1_4 = src[0] * src[5] - src[1] * src[4];


	_ST cf0 =	src[5] * sub_10_15_11_14 - 
				src[6] * sub_9_15_11_13 + 
				src[7] * sub_9_14_10_13;

	_ST cf1 =	- (src[4] * sub_10_15_11_14 - 
				src[6] * sub_8_15_11_12 + 
				src[7] * sub_8_14_10_12);

	_ST cf2 =	src[4] * sub_9_15_11_13 - 
				src[5] * sub_8_15_11_12 + 
				src[7] * sub_8_13_9_12;

	_ST cf3 =	- (src[4] * sub_9_14_10_13 - 
				src[5] * sub_8_14_10_12 + 
				src[6] * sub_8_13_9_12);

	_ST cf4 =	- (src[1] * sub_10_15_11_14 - 
				src[2] * sub_9_15_11_13 + 
				src[3] * sub_9_14_10_13);

	_ST cf5 =	src[0] * sub_10_15_11_14 - 
				src[2] * sub_8_15_11_12 + 
				src[3] * sub_8_14_10_12;

	_ST cf6 =	- (src[0] * sub_9_15_11_13 - 
				src[1] * sub_8_15_11_12 + 
				src[3] * sub_8_13_9_12);

	_ST cf7 =	src[0] * sub_9_14_10_13 - 
				src[1] * sub_8_14_10_12 + 
				src[2] * sub_8_13_9_12;

	_ST cf8 =	src[13] * sub_2_7_3_6 -
				src[14] * sub_1_7_3_5 +
				src[15] * sub_1_6_2_5;

	_ST cf9 =	- (src[12] * sub_2_7_3_6 -
				src[14] * sub_0_7_3_4 +
				src[15] * sub_0_6_2_4);

	_ST cf10 =	src[12] * sub_1_7_3_5 -
				src[13] * sub_0_7_3_4 +
				src[15] * sub_0_5_1_4;

	_ST cf11 =	- (src[12] * sub_1_6_2_5 -
				src[13] * sub_0_6_2_4 +
				src[14] * sub_0_5_1_4);

	_ST cf12 =	- (src[9] * sub_2_7_3_6 -
				src[10] * sub_1_7_3_5 +
				src[11] * sub_1_6_2_5);

	_ST cf13 =	src[8] * sub_2_7_3_6 -
				src[10] * sub_0_7_3_4 +
				src[11] * sub_0_6_2_4;

	_ST cf14 =	- (src[8] * sub_1_7_3_5 -
				src[9] * sub_0_7_3_4 +
				src[11] * sub_0_5_1_4);

	_ST cf15 =	src[8] * sub_1_6_2_5 -
				src[9] * sub_0_6_2_4 +
				src[10] * sub_0_5_1_4;

	// determinant
	_ST det = src[0] * cf0 + src[1] * cf1 + src[2] * cf2 + src[3] * cf3;
	_ST inv_det = _ST(1) / det;

	// inverse
	dest[0] = cf0 * inv_det;
	dest[1] = cf4 * inv_det;
	dest[2] = cf8 * inv_det;
	dest[3] = cf12 * inv_det;

	dest[4] = cf1 * inv_det;
	dest[5] = cf5 * inv_det;
	dest[6] = cf9 * inv_det;
	dest[7] = cf13 * inv_det;

	dest[8] = cf2 * inv_det;
	dest[9] = cf6 * inv_det;
	dest[10] = cf10 * inv_det;
	dest[11] = cf14 * inv_det;

	dest[12] = cf3 * inv_det;
	dest[13] = cf7 * inv_det;
	dest[14] = cf11 * inv_det;
	dest[15] = cf15 * inv_det;
}

template <class _ST>
void mat4<_ST>::_inverse_transpose(_ST* dest, const _ST* src)
{
	// co-factors
	_ST sub_10_15_11_14 = src[10] * src[15] - src[11] * src[14];
	_ST sub_9_15_11_13 = src[9] * src[15] - src[11] * src[13];
	_ST sub_9_14_10_13 = src[9] * src[14] - src[10] * src[13];
	_ST sub_8_15_11_12 = src[8] * src[15] - src[11] * src[12];
	_ST sub_8_14_10_12 = src[8] * src[14] - src[10] * src[12];
	_ST sub_8_13_9_12 = src[8] * src[13] - src[9] * src[12];

	_ST sub_2_7_3_6 = src[2] * src[7] - src[3] * src[6];
	_ST sub_1_7_3_5 = src[1] * src[7] - src[3] * src[5];
	_ST sub_1_6_2_5 = src[1] * src[6] - src[2] * src[5];
	_ST sub_0_7_3_4 = src[0] * src[7] - src[3] * src[4];
	_ST sub_0_6_2_4 = src[0] * src[6] - src[2] * src[4];
	_ST sub_0_5_1_4 = src[0] * src[5] - src[1] * src[4];


	_ST cf0 =	src[5] * sub_10_15_11_14 - 
				src[6] * sub_9_15_11_13 + 
				src[7] * sub_9_14_10_13;

	_ST cf1 =	- (src[4] * sub_10_15_11_14 - 
				src[6] * sub_8_15_11_12 + 
				src[7] * sub_8_14_10_12);

	_ST cf2 =	src[4] * sub_9_15_11_13 - 
				src[5] * sub_8_15_11_12 + 
				src[7] * sub_8_13_9_12;

	_ST cf3 =	- (src[4] * sub_9_14_10_13 - 
				src[5] * sub_8_14_10_12 + 
				src[6] * sub_8_13_9_12);

	_ST cf4 =	- (src[1] * sub_10_15_11_14 - 
				src[2] * sub_9_15_11_13 + 
				src[3] * sub_9_14_10_13);

	_ST cf5 =	src[0] * sub_10_15_11_14 - 
				src[2] * sub_8_15_11_12 + 
				src[3] * sub_8_14_10_12;

	_ST cf6 =	- (src[0] * sub_9_15_11_13 - 
				src[1] * sub_8_15_11_12 + 
				src[3] * sub_8_13_9_12);

	_ST cf7 =	src[0] * sub_9_14_10_13 - 
				src[1] * sub_8_14_10_12 + 
				src[2] * sub_8_13_9_12;

	_ST cf8 =	src[13] * sub_2_7_3_6 -
				src[14] * sub_1_7_3_5 +
				src[15] * sub_1_6_2_5;

	_ST cf9 =	- (src[12] * sub_2_7_3_6 -
				src[14] * sub_0_7_3_4 +
				src[15] * sub_0_6_2_4);

	_ST cf10 =	src[12] * sub_1_7_3_5 -
				src[13] * sub_0_7_3_4 +
				src[15] * sub_0_5_1_4;

	_ST cf11 =	- (src[12] * sub_1_6_2_5 -
				src[13] * sub_0_6_2_4 +
				src[14] * sub_0_5_1_4);

	_ST cf12 =	- (src[9] * sub_2_7_3_6 -
				src[10] * sub_1_7_3_5 +
				src[11] * sub_1_6_2_5);

	_ST cf13 =	src[8] * sub_2_7_3_6 -
				src[10] * sub_0_7_3_4 +
				src[11] * sub_0_6_2_4;

	_ST cf14 =	- (src[8] * sub_1_7_3_5 -
				src[9] * sub_0_7_3_4 +
				src[11] * sub_0_5_1_4);

	_ST cf15 =	src[8] * sub_1_6_2_5 -
				src[9] * sub_0_6_2_4 +
				src[10] * sub_0_5_1_4;

	// determinant
	_ST det = src[0] * cf0 + src[1] * cf1 + src[2] * cf2 + src[3] * cf3;
	_ST inv_det = _ST(1) / det;

	// transpose of the inverse
	dest[0] = cf0 * inv_det;
	dest[1] = cf1 * inv_det;
	dest[2] = cf2 * inv_det;
	dest[3] = cf3 * inv_det;

	dest[4] = cf4 * inv_det;
	dest[5] = cf5 * inv_det;
	dest[6] = cf6 * inv_det;
	dest[7] = cf7 * inv_det;

	dest[8] = cf8 * inv_det;
	dest[9] = cf9 * inv_det;
	dest[10] = cf10 * inv_det;
	dest[11] = cf11 * inv_det;

	dest[12] = cf12 * inv_det;
	dest[13] = cf13 * inv_det;
	dest[14] = cf14 * inv_det;
	dest[15] = cf15 * inv_det;
}

template <class _ST>
void mat4<_ST>::_adjoint(_ST* dest, const _ST* src)
{
	// co-factors
	_ST sub_10_15_11_14 = src[10] * src[15] - src[11] * src[14];
	_ST sub_9_15_11_13 = src[9] * src[15] - src[11] * src[13];
	_ST sub_9_14_10_13 = src[9] * src[14] - src[10] * src[13];
	_ST sub_8_15_11_12 = src[8] * src[15] - src[11] * src[12];
	_ST sub_8_14_10_12 = src[8] * src[14] - src[10] * src[12];
	_ST sub_8_13_9_12 = src[8] * src[13] - src[9] * src[12];

	_ST sub_2_7_3_6 = src[2] * src[7] - src[3] * src[6];
	_ST sub_1_7_3_5 = src[1] * src[7] - src[3] * src[5];
	_ST sub_1_6_2_5 = src[1] * src[6] - src[2] * src[5];
	_ST sub_0_7_3_4 = src[0] * src[7] - src[3] * src[4];
	_ST sub_0_6_2_4 = src[0] * src[6] - src[2] * src[4];
	_ST sub_0_5_1_4 = src[0] * src[5] - src[1] * src[4];


	dest[0] =	src[5] * sub_10_15_11_14 - 
				src[6] * sub_9_15_11_13 + 
				src[7] * sub_9_14_10_13;

	dest[1] =	- (src[1] * sub_10_15_11_14 - 
				src[2] * sub_9_15_11_13 + 
				src[3] * sub_9_14_10_13);

	dest[2] =	src[13] * sub_2_7_3_6 -
				src[14] * sub_1_7_3_5 +
				src[15] * sub_1_6_2_5;

	dest[3] =	- (src[9] * sub_2_7_3_6 -
				src[10] * sub_1_7_3_5 +
				src[11] * sub_1_6_2_5);

	dest[4] =	- (src[4] * sub_10_15_11_14 - 
				src[6] * sub_8_15_11_12 + 
				src[7] * sub_8_14_10_12);

	dest[5] =	src[0] * sub_10_15_11_14 - 
				src[2] * sub_8_15_11_12 + 
				src[3] * sub_8_14_10_12;

	dest[6] =	- (src[12] * sub_2_7_3_6 -
				src[14] * sub_0_7_3_4 +
				src[15] * sub_0_6_2_4);

	dest[7] =	src[8] * sub_2_7_3_6 -
				src[10] * sub_0_7_3_4 +
				src[11] * sub_0_6_2_4;

	dest[8] =	src[4] * sub_9_15_11_13 - 
				src[5] * sub_8_15_11_12 + 
				src[7] * sub_8_13_9_12;

	dest[9] =	- (src[0] * sub_9_15_11_13 - 
				src[1] * sub_8_15_11_12 + 
				src[3] * sub_8_13_9_12);

	dest[10] =	src[12] * sub_1_7_3_5 -
				src[13] * sub_0_7_3_4 +
				src[15] * sub_0_5_1_4;

	dest[11] =	- (src[8] * sub_1_7_3_5 -
				src[9] * sub_0_7_3_4 +
				src[11] * sub_0_5_1_4);

	dest[12] =	- (src[4] * sub_9_14_10_13 - 
				src[5] * sub_8_14_10_12 + 
				src[6] * sub_8_13_9_12);

	dest[13] =	src[0] * sub_9_14_10_13 - 
				src[1] * sub_8_14_10_12 + 
				src[2] * sub_8_13_9_12;

	dest[14] =	- (src[12] * sub_1_6_2_5 -
				src[13] * sub_0_6_2_4 +
				src[14] * sub_0_5_1_4);

	dest[15] =	src[8] * sub_1_6_2_5 -
				src[9] * sub_0_6_2_4 +
				src[10] * sub_0_5_1_4;
}


// matrix operations; global functions
template <class _ST>
void mul(mat4<_ST>& r, const mat4<_ST>& a, const mat4<_ST>& b)
{
	r(0) = a(0) * b(0) + a(1) * b(4) + a(2) * b(8) + a(3) * b(12);
	r(1) = a(0) * b(1) + a(1) * b(5) + a(2) * b(9) + a(3) * b(13);
	r(2) = a(0) * b(2) + a(1) * b(6) + a(2) * b(10) + a(3) * b(14);
	r(3) = a(0) * b(3) + a(1) * b(7) + a(2) * b(11) + a(3) * b(15);

	r(4) = a(4) * b(0) + a(5) * b(4) + a(6) * b(8) + a(7) * b(12);
	r(5) = a(4) * b(1) + a(5) * b(5) + a(6) * b(9) + a(7) * b(13);
	r(6) = a(4) * b(2) + a(5) * b(6) + a(6) * b(10) + a(7) * b(14);
	r(7) = a(4) * b(3) + a(5) * b(7) + a(6) * b(11) + a(7) * b(15);
	
	r(8) = a(8) * b(0) + a(9) * b(4) + a(10) * b(8) + a(11) * b(12);
	r(9) = a(8) * b(1) + a(9) * b(5) + a(10) * b(9) + a(11) * b(13);
	r(10) = a(8) * b(2) + a(9) * b(6) + a(10) * b(10) + a(11) * b(14);
	r(11) = a(8) * b(3) + a(9) * b(7) + a(10) * b(11) + a(11) * b(15);

	r(12) = a(12) * b(0) + a(13) * b(4) + a(14) * b(8) + a(15) * b(12);
	r(13) = a(12) * b(1) + a(13) * b(5) + a(14) * b(9) + a(15) * b(13);
	r(14) = a(12) * b(2) + a(13) * b(6) + a(14) * b(10) + a(15) * b(14);
	r(15) = a(12) * b(3) + a(13) * b(7) + a(14) * b(11) + a(15) * b(15);
}

template <class _ST>
inline
mat4<_ST> operator * (const mat4<_ST>& a, const mat4<_ST>& b)
{
	mat4<_ST> result;
	mul(result, a, b);
	return result;
}

template <class _ST>
void add(mat4<_ST>& r, const mat4<_ST>& a, const mat4<_ST>& b)
{
	r(0) = a(0) + b(0);
	r(1) = a(1) + b(1);
	r(2) = a(2) + b(2);
	r(3) = a(3) + b(3);

	r(4) = a(4) + b(4);
	r(5) = a(5) + b(5);
	r(6) = a(6) + b(6);
	r(7) = a(7) + b(7);

	r(8) = a(8) + b(8);
	r(9) = a(9) + b(9);
	r(10) = a(10) + b(10);
	r(11) = a(11) + b(11);

	r(12) = a(12) + b(12);
	r(13) = a(13) + b(13);
	r(14) = a(14) + b(14);
	r(15) = a(15) + b(15);
}

template <class _ST>
inline
mat4<_ST> operator + (const mat4<_ST>& a, const mat4<_ST>& b)
{
	mat4<_ST> result;
	add(result, a, b);
	return result;
}


constexpr mat4f mat4f_identity = mat4_identity<float>;
constexpr mat4d mat4d_identity = mat4_identity<double>;
constexpr mat4ld mat4ld_identity = mat4_identity<long double>;
constexpr mat4i mat4i_identity = mat4_identity<int>;
constexpr mat4ui mat4ui_identity = mat4_identity<unsigned int>;


} // namespace math3d



#endif //_MATH3D_MAT4_H_


/* ----------------------------------------
	File: mat3.h
	Purpose: 3x3 matrix class
	Author: Milan D.
	Date: 01.05.2005
   ---------------------------------------- */



#ifndef _MATH3D_MAT3_H_
#define _MATH3D_MAT3_H_

#include <type_traits>
#include <utility>
#include <cstring>


/*

Elements are ordered in memory like this:

    0    1    2

    3    4    5

    6    7    8

What matrix elements represent:

                 +------------+
    x axes ->    | Xx  Xy  Xz |
                 +------------+
    y axes ->    | Yx  Yy  Yz |
                 +------------+
    z axes ->    | Zx  Zy  Zz |
                 +------------+
*/


namespace math3d
{

template <class _ST> class vec2;
template <class _ST> class vec3;
template <class _ST> class mat4;


template <class _ST>
class mat3
{
public:
	static_assert(std::is_arithmetic_v<_ST>, "Scalar type _ST must be floating point or integral type.");

	typedef _ST scalar_t;

	static constexpr size_t num_elements = 9;

	constexpr mat3() = default;

	constexpr mat3(	_ST m0, _ST m1, _ST m2,
					_ST m3, _ST m4, _ST m5,
					_ST m6, _ST m7, _ST m8);

	constexpr mat3(const vec3<_ST>& row0, const vec3<_ST>& row1, const vec3<_ST>& row2);

	// conversion constructors
	explicit constexpr mat3(const mat4<_ST>& m4);
	explicit constexpr mat3(const _ST* m);

	template <class _T>
	explicit constexpr mat3(const mat3<_T>& m);

	// access
	vec3<_ST>& operator [] (int row_index);
	constexpr const vec3<_ST>& operator [] (int row_index) const;

	_ST& operator () (int index);
	constexpr _ST operator () (int index) const;

	operator _ST* () { return _data; }
	constexpr operator const _ST* () const { return _data; }

	void set(_ST m0, _ST m1, _ST m2,
			 _ST m3, _ST m4, _ST m5,
			 _ST m6, _ST m7, _ST m8);

	void set(const vec3<_ST>& row0, const vec3<_ST>& row1, const vec3<_ST>& row2);
	void set(const mat4<_ST>& m4);
	void get(vec3<_ST>& row0, vec3<_ST>& row1, vec3<_ST>& row2) const;
	constexpr vec3<_ST> get_col(int col) const;

	// operations
	constexpr _ST determinant() const;
	void transpose();
	void inverse();
	void inverse_transpose();
	void adjoint();
	void get_transpose(mat3& m) const;
	void get_inverse(mat3& m) const;
	void get_inverse_transpose(mat3& m) const;
	void get_adjoint(mat3& m) const;
	void set_identity();
	void set_transpose(const mat3& m);
	void set_inverse(const mat3& m);
	void set_inverse_transpose(const mat3& m);
	void set_adjoint(const mat3& m);

	// transformations
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
	void set_rotation(_ST angle, _ST _x, _ST _y, _ST _z);
	void set_rotation(_ST angle, const vec3<_ST>& axis);
	void set_rotation_x(_ST angle);
	void set_rotation_y(_ST angle);
	void set_rotation_z(_ST angle);
	void set_scale(_ST s);
	void set_scale(_ST _x, _ST _y, _ST _z);
	void set_scale(const vec3<_ST>& s);

	// 2D transformations.
	void translate_2d(_ST x, _ST y);
	void translate_2d(const vec2<_ST>& transl);
	void rotate_2d(_ST angle);
	void scale_2d(_ST sx, _ST sy);
	void scale_2d(const vec2<_ST>& scale);
	void set_translation_2d(_ST x, _ST y);
	void set_translation_2d(const vec2<_ST>& transl);
	void set_rotation_2d(_ST angle);
	void set_scale_2d(_ST sx, _ST sy);
	void set_scale_2d(const vec2<_ST>& scale);

private:
	void _copy(const _ST* src);
	static void _inverse(_ST* dest, const _ST* src);
	static void _transpose(_ST* dest, const _ST* src);
	static void _inverse_transpose(_ST* dest, const _ST* src);
	static void _adjoint(_ST* dest, const _ST* src);
	static void _swap(_ST& a, _ST& b);

	_ST _data[num_elements];
};


typedef mat3<float> mat3f;
typedef mat3<double> mat3d;
typedef mat3<long double> mat3ld;
typedef mat3<int> mat3i;
typedef mat3<unsigned int> mat3ui;


} // namespace math3d

// ------------------------------------

#include "vec3.h"
#include "mat4.h"


namespace math3d
{

template <class _ST>
constexpr mat3<_ST> mat3_identity {
	_ST(1), _ST(0), _ST(0),
	_ST(0), _ST(1), _ST(0),
	_ST(0), _ST(0), _ST(1)
};


template <class _ST>
constexpr
mat3<_ST>::mat3(_ST m0, _ST m1, _ST m2,
				_ST m3, _ST m4, _ST m5,
				_ST m6, _ST m7, _ST m8)
	: _data { m0, m1, m2, m3, m4, m5, m6, m7, m8 }
{
}

template <class _ST>
constexpr
mat3<_ST>::mat3(const vec3<_ST>& row0, const vec3<_ST>& row1, const vec3<_ST>& row2)
	: _data {
		row0.x, row0.y, row0.z,
		row1.x, row1.y, row1.z,
		row2.x, row2.y, row2.z,
	}
{
}

// conversion constructors
template <class _ST>
constexpr
mat3<_ST>::mat3(const mat4<_ST>& m4)
	: _data {
		m4(0), m4(1), m4(2),
		m4(4), m4(5), m4(6),
		m4(8), m4(9), m4(10)
	}
{
}

template <class _ST>
constexpr
mat3<_ST>::mat3(const _ST* m)
	: _data {
		m[0], m[1], m[2],
		m[3], m[4], m[5],
		m[6], m[7], m[8],
	}
{
}

template <class _ST>
template <class _T>
constexpr
mat3<_ST>::mat3(const mat3<_T>& m)
	: _data {
		_ST(m(0)), _ST(m(1)), _ST(m(2)),
		_ST(m(3)), _ST(m(4)), _ST(m(5)),
		_ST(m(6)), _ST(m(7)), _ST(m(8))
	}
{
}


// access

template <class _ST>
inline
vec3<_ST>& mat3<_ST>::operator [] (int row_index)
{
	return *(vec3<_ST>*) (_data + (row_index << 1) + row_index);
}

template <class _ST>
constexpr
const vec3<_ST>& mat3<_ST>::operator [] (int row_index) const
{
	return *(vec3<_ST>*) (_data + (row_index << 1) + row_index);
}

template <class _ST>
inline
_ST& mat3<_ST>::operator () (int index)
{
	return _data[index];
}

template <class _ST>
constexpr
_ST mat3<_ST>::operator () (int index) const
{
	return _data[index];
}

template <class _ST>
inline
void mat3<_ST>::set(_ST m0, _ST m1, _ST m2,
					_ST m3, _ST m4, _ST m5,
					_ST m6, _ST m7, _ST m8)
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
}

template <class _ST>
void mat3<_ST>::set(const vec3<_ST>& row0, const vec3<_ST>& row1, const vec3<_ST>& row2)
{
	(*this)[0] = row0;
	(*this)[1] = row1;
	(*this)[2] = row2;
}

template <class _ST>
void mat3<_ST>::set(const mat4<_ST>& m4)
{
	(*this)[0] = m4[0].rvec3();
	(*this)[1] = m4[1].rvec3();
	(*this)[2] = m4[2].rvec3();
}

template <class _ST>
void mat3<_ST>::get(vec3<_ST>& row0, vec3<_ST>& row1, vec3<_ST>& row2) const
{
	row0 = (*this)[0];
	row1 = (*this)[1];
	row2 = (*this)[2];
}

template <class _ST>
constexpr
vec3<_ST> mat3<_ST>::get_col(int col) const
{
	return { _data[col], _data[col + 3], _data[col + 6] };
}


// operations

template <class _ST>
constexpr
_ST mat3<_ST>::determinant() const
{
	// co-factors
	_ST cf0 = _data[4] * _data[8] - _data[5] * _data[7];
	_ST cf1 = _data[3] * _data[8] - _data[5] * _data[6];
	_ST cf2 = _data[3] * _data[7] - _data[4] * _data[6];

	return _data[0] * cf0 - _data[1] * cf1 + _data[2] * cf2;
}

template <class _ST>
inline
void mat3<_ST>::set_identity()
{
	_copy(mat3_identity<_ST>);
}

template <class _ST>
void mat3<_ST>::transpose()
{
	_swap(_data[1], _data[3]);
	_swap(_data[2], _data[6]);
	_swap(_data[5], _data[7]);
}

// inverse of an arbitrary matrix
// WARNING: does not check if the matrix is invertible!
template <class _ST>
inline
void mat3<_ST>::inverse()
{
	_inverse(_data, _data);
}

template <class _ST>
inline
void mat3<_ST>::inverse_transpose()
{
	_inverse_transpose(_data, _data);
}

template <class _ST>
inline
void mat3<_ST>::adjoint()
{
	_adjoint(_data, _data);
}

template <class _ST>
inline
void mat3<_ST>::get_transpose(mat3& m) const
{
	_transpose(m._data, _data);
}

template <class _ST>
inline
void mat3<_ST>::get_inverse(mat3& m) const
{
	_inverse(m._data, _data);
}

template <class _ST>
inline
void mat3<_ST>::get_inverse_transpose(mat3& m) const
{
	_inverse_transpose(m._data, _data);
}

template <class _ST>
inline
void mat3<_ST>::get_adjoint(mat3& m) const
{
	_adjoint(m._data, _data);
}

template <class _ST>
inline
void mat3<_ST>::set_transpose(const mat3& m)
{
	_transpose(_data, m._data);
}

template <class _ST>
inline
void mat3<_ST>::set_inverse(const mat3& m)
{
	_inverse(_data, m._data);
}

template <class _ST>
inline
void mat3<_ST>::set_inverse_transpose(const mat3& m)
{
	_inverse_transpose(_data, m._data);
}

template <class _ST>
inline
void mat3<_ST>::set_adjoint(const mat3& m)
{
	_adjoint(_data, m._data);
}

// transformations

template <class _ST>
void mat3<_ST>::rotate(_ST angle, _ST _x, _ST _y, _ST _z)
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

	mat3<_ST> temp;

	temp(0) = _data[0] * rot00 + _data[1] * rot10 + _data[2] * rot20;
	temp(1) = _data[0] * rot01 + _data[1] * rot11 + _data[2] * rot21;
	temp(2) = _data[0] * rot02 + _data[1] * rot12 + _data[2] * rot22;

	temp(3) = _data[3] * rot00 + _data[4] * rot10 + _data[5] * rot20;
	temp(4) = _data[3] * rot01 + _data[4] * rot11 + _data[5] * rot21;
	temp(5) = _data[3] * rot02 + _data[4] * rot12 + _data[5] * rot22;

	temp(6) = _data[6] * rot00 + _data[7] * rot10 + _data[8] * rot20;
	temp(7) = _data[6] * rot01 + _data[7] * rot11 + _data[8] * rot21;
	temp(8) = _data[6] * rot02 + _data[7] * rot12 + _data[8] * rot22;

	*this = temp;
}

template <class _ST>
inline
void mat3<_ST>::rotate(_ST angle, const vec3<_ST>& axis)
{
	rotate(angle, axis.x, axis.y, axis.z);
}

template <class _ST>
void mat3<_ST>::rotate_x(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);
	_ST t;

	t = _data[1] * cos_a - _data[2] * sin_a;
	_data[2] = _data[1] * sin_a + _data[2] * cos_a;
	_data[1] = t;

	t = _data[4] * cos_a - _data[5] * sin_a;
	_data[5] = _data[4] * sin_a + _data[5] * cos_a;
	_data[4] = t;

	t = _data[7] * cos_a - _data[8] * sin_a;
	_data[8] = _data[7] * sin_a + _data[8] * cos_a;
	_data[7] = t;
}

template <class _ST>
void mat3<_ST>::rotate_y(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);
	_ST t;

	t = _data[0] * cos_a + _data[2] * sin_a;
	_data[2] = _data[2] * cos_a - _data[0] * sin_a;
	_data[0] = t;

	t = _data[3] * cos_a + _data[5] * sin_a;
	_data[5] = _data[5] * cos_a - _data[3] * sin_a;
	_data[3] = t;

	t = _data[6] * cos_a + _data[8] * sin_a;
	_data[8] = _data[8] * cos_a - _data[6] * sin_a;
	_data[6] = t;
}

template <class _ST>
void mat3<_ST>::rotate_z(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);
	_ST t;

	t = _data[0] * cos_a - _data[1] * sin_a;
	_data[1] = _data[0] * sin_a + _data[1] * cos_a;
	_data[0] = t;

	t = _data[3] * cos_a - _data[4] * sin_a;
	_data[4] = _data[3] * sin_a + _data[4] * cos_a;
	_data[3] = t;

	t = _data[7] * cos_a - _data[8] * sin_a;
	_data[8] = _data[7] * sin_a + _data[8] * cos_a;
	_data[7] = t;
}

template <class _ST>
void mat3<_ST>::from_euler(_ST x_angle, _ST y_angle, _ST z_angle)
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

	_ST srsp = sr * sp;
	_ST crsp = cr * sp;

	_data[3] = srsp * cy - cr * sy;
	_data[4] = srsp * sy + cr * cy;
	_data[5] = sr * cp;

	_data[6] = crsp * cy + sr * sy;
	_data[7] = crsp * sy - sr * cy;
	_data[8] = cr * cp;
}

template <class _ST>
void mat3<_ST>::to_euler(_ST& x_angle, _ST& y_angle, _ST& z_angle)
{
	y_angle = -std::asin(_data[2]);
	_ST cy = std::cos(y_angle);

	float rotx, roty;

	if(fcmp_gt(fabs(cy), _ST(0)))
	{
		_ST inv_c = _ST(1) / cy;
		rotx = _data[8] * inv_c;
		roty = _data[5] * inv_c;
		x_angle = atan2(roty, rotx);
		rotx = _data[0] * inv_c;
		roty = _data[1] * inv_c;
		z_angle = atan2(roty, rotx);
	}
	else
	{
		x_angle = _ST(0);
		rotx = _data[4];
		roty = -_data[3];
		z_angle = atan2(roty, rotx);
	}

	if(x_angle < 0.0) x_angle += _ST(TWO_PI);
	if(y_angle < 0.0) y_angle += _ST(TWO_PI);
	if(z_angle < 0.0) z_angle += _ST(TWO_PI);
}

template <class _ST>
void mat3<_ST>::scale(_ST s)
{
	_data[0] *= s;
	_data[1] *= s;
	_data[2] *= s;

	_data[3] *= s;
	_data[4] *= s;
	_data[5] *= s;

	_data[6] *= s;
	_data[7] *= s;
	_data[8] *= s;
}

template <class _ST>
void mat3<_ST>::scale(_ST _x, _ST _y, _ST _z)
{
	_data[0] *= _x;
	_data[1] *= _y;
	_data[2] *= _z;

	_data[3] *= _x;
	_data[4] *= _y;
	_data[5] *= _z;

	_data[6] *= _x;
	_data[7] *= _y;
	_data[8] *= _z;
}

template <class _ST>
void mat3<_ST>::scale(const vec3<_ST>& s)
{
	_data[0] *= s.x;
	_data[1] *= s.y;
	_data[2] *= s.z;

	_data[3] *= s.x;
	_data[4] *= s.y;
	_data[5] *= s.z;

	_data[6] *= s.x;
	_data[7] *= s.y;
	_data[8] *= s.z;
}

template <class _ST>
void mat3<_ST>::set_rotation(_ST angle, _ST _x, _ST _y, _ST _z)
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
	_data[3] = inv_cos_a_xy - sin_a_z;
	_data[6] = inv_cos_a_xz + sin_a_y;
	_data[1] = inv_cos_a_xy + sin_a_z;
	_data[4] = cos_a + inv_cos_a * _y * _y;
	_data[7] = inv_cos_a_yz - sin_a_x;
	_data[2] = inv_cos_a_xz - sin_a_y;
	_data[5] = inv_cos_a_yz + sin_a_x;
	_data[8] = cos_a + inv_cos_a * _z * _z;
}

template <class _ST>
inline
void mat3<_ST>::set_rotation(_ST angle, const vec3<_ST>& axis)
{
	set_rotation(angle, axis.x, axis.y, axis.z);
}

template <class _ST>
void mat3<_ST>::set_rotation_x(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);

	(*this)[0].set(_ST(1), _ST(0), _ST(0));
	(*this)[1].set(_ST(0), cos_a, sin_a);
	(*this)[2].set(_ST(0), -sin_a, cos_a);
}

template <class _ST>
void mat3<_ST>::set_rotation_y(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);

	(*this)[0].set(cos_a, _ST(0), -sin_a);
	(*this)[1].set(_ST(0), _ST(1), _ST(0));
	(*this)[2].set(sin_a, _ST(0), cos_a);
}

template <class _ST>
void mat3<_ST>::set_rotation_z(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);

	(*this)[0].set(cos_a, sin_a, _ST(0));
	(*this)[1].set(-sin_a, cos_a, _ST(0));
	(*this)[2].set(_ST(0), _ST(0), _ST(1));
}

template <class _ST>
void mat3<_ST>::set_scale(_ST s)
{
	(*this)[0].set(s, _ST(0), _ST(0));
	(*this)[1].set(_ST(0), s, _ST(0));
	(*this)[2].set(_ST(0), _ST(0), s);
}

template <class _ST>
void mat3<_ST>::set_scale(_ST _x, _ST _y, _ST _z)
{
	(*this)[0].set(_x, _ST(0), _ST(0));
	(*this)[1].set(_ST(0), _y, _ST(0));
	(*this)[2].set(_ST(0), _ST(0), _z);
}

template <class _ST>
void mat3<_ST>::set_scale(const vec3<_ST>& s)
{
	(*this)[0].set(s.x, _ST(0), _ST(0));
	(*this)[1].set(_ST(0), s.y, _ST(0));
	(*this)[2].set(_ST(0), _ST(0), s.z);
}

template<class _ST>
inline void mat3<_ST>::translate_2d(_ST x, _ST y)
{
	_data[6] += x;
	_data[7] += y;
}

template<class _ST>
inline void mat3<_ST>::translate_2d(const vec2<_ST>& transl)
{
	_data[6] += transl.x;
	_data[7] += transl.y;
}

template<class _ST>
inline void mat3<_ST>::rotate_2d(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);

	_ST m0 = _data[0] * cos_a - _data[1] * sin_a;
	_ST m1 = _data[0] * sin_a + _data[1] * cos_a;
	_data[0] = m0;
	_data[1] = m1;

	_ST m3 = _data[3] * cos_a - _data[4] * sin_a;
	_ST m4 = _data[3] * sin_a + _data[4] * cos_a;
	_data[3] = m3;
	_data[4] = m4;

	_ST m6 = _data[6] * cos_a - _data[7] * sin_a;
	_ST m7 = _data[6] * sin_a + _data[7] * cos_a;
	_data[6] = m6;
	_data[7] = m7;
}

template<class _ST>
inline void mat3<_ST>::scale_2d(_ST sx, _ST sy)
{
	_data[0] *= sx;
	_data[1] *= sy;

	_data[3] *= sx;
	_data[4] *= sy;

	_data[6] *= sx;
	_data[7] *= sy;
}

template<class _ST>
inline void mat3<_ST>::scale_2d(const vec2<_ST>& scale)
{
	scale_2d(scale.x, scale.y);
}

template<class _ST>
inline void mat3<_ST>::set_translation_2d(_ST x, _ST y)
{
	set(
		_ST(1), _ST(0), _ST(0),
		_ST(0), _ST(1), _ST(0),
		x,      y,      _ST(1)
	);
}

template<class _ST>
inline void mat3<_ST>::set_translation_2d(const vec2<_ST>& transl)
{
	set_translation_2d(transl.x, transl.y);
}

template<class _ST>
inline void mat3<_ST>::set_rotation_2d(_ST angle)
{
	_ST cos_a = std::cos(angle);
	_ST sin_a = std::sin(angle);

	set(
		cos_a,  sin_a,  _ST(0),
		-sin_a, cos_a,  _ST(0),
		_ST(0), _ST(0), _ST(1)
	);
}

template<class _ST>
inline void mat3<_ST>::set_scale_2d(_ST sx, _ST sy)
{
	set(
		sx,     _ST(0), _ST(0),
		_ST(0), sy,     _ST(0),
		_ST(0), _ST(0), _ST(1)
	);
}

template<class _ST>
inline void mat3<_ST>::set_scale_2d(const vec2<_ST>& scale)
{
	set_scale_2d(scale.x, scale.y);
}

// implementation functions

template <class _ST>
inline
void mat3<_ST>::_copy(const _ST* src)
{
	memcpy(this, src, sizeof(_ST) * num_elements);
}

template <class _ST>
void mat3<_ST>::_transpose(_ST* dest, const _ST* src)
{
	dest[0] = src[0];
	dest[1] = src[3];
	dest[2] = src[6];

	dest[3] = src[1];
	dest[4] = src[4];
	dest[5] = src[7];

	dest[6] = src[2];
	dest[7] = src[5];
	dest[8] = src[8];
}

template <class _ST>
void mat3<_ST>::_inverse(_ST* dest, const _ST* src)
{
	// co-factors
	_ST cf0 = src[4] * src[8] - src[5] * src[7];
	_ST cf1 = - (src[3] * src[8] - src[5] * src[6]);
	_ST cf2 = src[3] * src[7] - src[4] * src[6];
	_ST cf3 = - (src[1] * src[8] - src[2] * src[7]);
	_ST cf4 = src[0] * src[8] - src[2] * src[6];
	_ST cf5 = - (src[0] * src[7] - src[1] * src[6]);
	_ST cf6 = src[1] * src[5] - src[2] * src[4];
	_ST cf7 = - (src[0] * src[5] - src[2] * src[3]);
	_ST cf8 = src[0] * src[4] - src[1] * src[3];

	// determinant
	_ST det = src[0] * cf0 + src[1] * cf1 + src[2] * cf2;
	_ST inv_det = _ST(1) / det;

	// inverse
	dest[0] = cf0 * inv_det;
	dest[1] = cf3 * inv_det;
	dest[2] = cf6 * inv_det;

	dest[3] = cf1 * inv_det;
	dest[4] = cf4 * inv_det;
	dest[5] = cf7 * inv_det;

	dest[6] = cf2 * inv_det;
	dest[7] = cf5 * inv_det;
	dest[8] = cf8 * inv_det;
}

template <class _ST>
void mat3<_ST>::_inverse_transpose(_ST* dest, const _ST* src)
{
	// co-factors
	_ST cf0 = src[4] * src[8] - src[5] * src[7];
	_ST cf1 = - (src[3] * src[8] - src[5] * src[6]);
	_ST cf2 = src[3] * src[7] - src[4] * src[6];
	_ST cf3 = - (src[1] * src[8] - src[2] * src[7]);
	_ST cf4 = src[0] * src[8] - src[2] * src[6];
	_ST cf5 = - (src[0] * src[7] - src[1] * src[6]);
	_ST cf6 = src[1] * src[5] - src[2] * src[4];
	_ST cf7 = - (src[0] * src[5] - src[2] * src[3]);
	_ST cf8 = src[0] * src[4] - src[1] * src[3];

	// determinant
	_ST det = src[0] * cf0 + src[1] * cf1 + src[2] * cf2;
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
}

template <class _ST>
void mat3<_ST>::_adjoint(_ST* dest, const _ST* src)
{
	// co-factors
	dest[0] = src[4] * src[8] - src[5] * src[7];
	dest[1] = - (src[1] * src[8] - src[2] * src[7]);
	dest[2] = src[1] * src[5] - src[2] * src[4];
	dest[3] = - (src[3] * src[8] - src[5] * src[6]);
	dest[4] = src[0] * src[8] - src[2] * src[6];
	dest[5] = - (src[0] * src[5] - src[2] * src[3]);
	dest[6] = src[3] * src[7] - src[4] * src[6];
	dest[7] = - (src[0] * src[7] - src[1] * src[6]);
	dest[8] = src[0] * src[4] - src[1] * src[3];
}

template <class _ST>
inline
void mat3<_ST>::_swap(_ST& a, _ST& b)
{
	_ST t = a;
	a = b;
	b = t;
}


// matrix operations; global functions

// WARNING: result must not be the same object as one of the operands
template <class _ST>
void mul(mat3<_ST>& r, const mat3<_ST>& a, const mat3<_ST>& b)
{
	r(0) = a(0) * b(0) + a(1) * b(3) + a(2) * b(6);
	r(1) = a(0) * b(1) + a(1) * b(4) + a(2) * b(7);
	r(2) = a(0) * b(2) + a(1) * b(5) + a(2) * b(8);

	r(3) = a(3) * b(0) + a(4) * b(3) + a(5) * b(6);
	r(4) = a(3) * b(1) + a(4) * b(4) + a(5) * b(7);
	r(5) = a(3) * b(2) + a(4) * b(5) + a(5) * b(8);

	r(6) = a(6) * b(0) + a(7) * b(3) + a(8) * b(6);
	r(7) = a(6) * b(1) + a(7) * b(4) + a(8) * b(7);
	r(8) = a(6) * b(2) + a(7) * b(5) + a(8) * b(8);
}

template <class _ST>
inline
mat3<_ST> operator * (const mat3<_ST>& a, const mat3<_ST>& b)
{
	mat3<_ST> result;
	mul(result, a, b);
	return result;
}

template <class _ST>
void add(mat3<_ST>& r, const mat3<_ST>& a, const mat3<_ST>& b)
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
}

template <class _ST>
inline
mat3<_ST> operator + (const mat3<_ST>& a, const mat3<_ST>& b)
{
	mat3<_ST> result;
	add(result, a, b);
	return result;
}


constexpr mat3f mat3f_identity = mat3_identity<float>;
constexpr mat3d mat3d_identity = mat3_identity<double>;
constexpr mat3ld mat3ld_identity = mat3_identity<long double>;
constexpr mat3i mat3i_identity = mat3_identity<int>;
constexpr mat3ui mat3ui_identity = mat3_identity<unsigned int>;


} // namespace math3d


#endif //_MATH3D_MAT3_H_


/* ----------------------------------------
	File: vec3.h
	Purpose: 3 component vector class
	Author: Milan D.
	Date: 01.05.2005
   ---------------------------------------- */


#ifndef _MATH3D_VEC3_H_
#define _MATH3D_VEC3_H_

#include <type_traits>
#include <algorithm>
#include "mathutil.h"


namespace math3d
{

template <typename _ST> class vec2;
template <typename _ST> class vec4;



template <typename _ST>
class vec3
{
public:
	static_assert(std::is_arithmetic_v<_ST>, "Scalar type _ST must be floating point or integral type.");

	typedef _ST scalar_t;

	static constexpr size_t num_elements = 3;

	constexpr vec3() = default;
	constexpr vec3(_ST _x, _ST _y, _ST _z);

	// conversion constructors
	explicit constexpr vec3(const vec2<_ST>& v2, _ST z = _ST(0));
	explicit constexpr vec3(const vec4<_ST>& v4);
	explicit constexpr vec3(const _ST* v);

	template <typename _T>
	explicit constexpr vec3(const vec3<_T>& v);

	// access
	operator _ST* ();
	operator const _ST* () const;
	_ST& operator [] (int index);
	constexpr _ST operator [] (int index) const;
	void set(_ST x, _ST y, _ST z);
	void set(const _ST* v);
	void set(const vec2<_ST>& v2, _ST _z = _ST(0));
	constexpr void get(_ST* v) const;

	vec2<_ST>& rvec2();
	constexpr const vec2<_ST>& rvec2() const;

	// operations
	_ST length() const;
	constexpr _ST length_sq() const;
	void normalize();
	void negate();
	vec3 operator - () const;
	void set_null();

	vec3& operator += (const vec3& v);
	vec3& operator -= (const vec3& v);
	vec3& operator *= (const vec3& v);
	vec3& operator /= (const vec3& v);

	vec3& operator += (_ST s);
	vec3& operator -= (_ST s);
	vec3& operator *= (_ST s);
	vec3& operator /= (_ST s);

	// data
	_ST x, y, z;
};

typedef vec3<float> vec3f;
typedef vec3<double> vec3d;
typedef vec3<long double> vec3ld;
typedef vec3<int> vec3i;
typedef vec3<unsigned int> vec3ui;
typedef vec3<short> vec3s;
typedef vec3<unsigned short> vec3us;
typedef vec3<char> vec3b;
typedef vec3<unsigned char> vec3ub;


} // namespace math3d

// -------------------------------------------------------------------


#include "vec2.h"
#include "vec4.h"


namespace math3d
{

template <typename _ST>
constexpr vec3<_ST> vec3_x_axis { _ST(1), _ST(0), _ST(0) };

template <typename _ST>
constexpr vec3<_ST> vec3_y_axis { _ST(0), _ST(1), _ST(0) };

template <typename _ST>
constexpr vec3<_ST> vec3_z_axis { _ST(0), _ST(0), _ST(1) };

template <typename _ST>
constexpr vec3<_ST> vec3_null { _ST(0), _ST(0), _ST(0) };



template <typename _ST>
constexpr
vec3<_ST>::vec3(_ST _x, _ST _y, _ST _z)
	: x { _x }
	, y { _y }
	, z { _z }
{
}

template <typename _ST>
constexpr
vec3<_ST>::vec3(const vec2<_ST>& v2, _ST _z)
	: x { v2.x }
	, y { v2.y }
	, z { _z }
{
}

template <typename _ST>
constexpr
vec3<_ST>::vec3(const vec4<_ST>& v4)
	: x { v4.x }
	, y { v4.y }
	, z { v4.z }
{
}

template <typename _ST>
constexpr
vec3<_ST>::vec3(const _ST* v)
	: x { v[0] }
	, y { v[1] }
	, z { v[2] }
{
}

template <typename _ST>
template <typename _T>
constexpr
vec3<_ST>::vec3(const vec3<_T>& v)
	: x { _ST(v.x) }
	, y { _ST(v.y) }
	, z { _ST(v.z) }
{
}

template<typename _ST>
inline
vec3<_ST>::operator _ST*()
{
	return &x;
}

template<typename _ST>
inline
vec3<_ST>::operator const _ST*() const
{
	return &x;
}

template<typename _ST>
inline
_ST& vec3<_ST>::operator[] (int index)
{
	return (&x)[index];
}

template<typename _ST>
constexpr
_ST vec3<_ST>::operator[] (int index) const
{
	return (&x)[index];
}

template <typename _ST>
inline
void vec3<_ST>::set(_ST _x, _ST _y, _ST _z)
{
	x = _x;
	y = _y;
	z = _z;
}

template <typename _ST>
inline
void vec3<_ST>::set(const _ST* v)
{
	x = v[0];
	y = v[1];
	z = v[2];
}

template <typename _ST>
inline
void vec3<_ST>::set(const vec2<_ST>& v2, _ST _z)
{
	x = v2.x;
	y = v2.y;
	z = _z;
}

template <typename _ST>
constexpr
void vec3<_ST>::get(_ST* v) const
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

template <typename _ST>
vec2<_ST>& vec3<_ST>::rvec2()
{
	return *(vec2<_ST>*)this;
}

template <typename _ST>
constexpr const vec2<_ST>& vec3<_ST>::rvec2() const
{
	return *(const vec2<_ST>*)this;
}

template <typename _ST>
inline
_ST vec3<_ST>::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}

template <typename _ST>
constexpr
_ST vec3<_ST>::length_sq() const
{
	return (x * x + y * y + z * z);
}


template <typename _ST>
inline
void vec3<_ST>::normalize()
{
	_ST len = length();

	if (len >= std::numeric_limits<_ST>::epsilon())
	{
		_ST i_len = _ST(1) / len;
		x *= i_len;
		y *= i_len;
		z *= i_len;
	}
}


template <typename _ST>
inline
void vec3<_ST>::negate()
{
	set(-x, -y, -z);
}

template <typename _ST>
inline
vec3<_ST> vec3<_ST>::operator - () const
{
	return { -x, -y, -z };
}


template <typename _ST>
inline
void vec3<_ST>::set_null()
{
	x = y = z = _ST(0);
}


//---------------------------------------------------------------------


template <typename _ST>
constexpr
_ST dot(const vec3<_ST>& a, const vec3<_ST>& b)
{
	return  a.x * b.x +
			a.y * b.y +
			a.z * b.z;
}

// Returns cross product of vectors a and b.
// r = a X b
template <typename _ST>
constexpr
vec3<_ST> cross(const vec3<_ST>& a, const vec3<_ST>& b)
{
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

template <typename _ST>
inline
vec3<_ST> normalize(const vec3<_ST>& a)
{
	_ST len = a.length();

	if (len >= std::numeric_limits<_ST>::epsilon())
	{
		_ST rlen = _ST(1) / len;
		return a * rlen;
	}

	return a;
}

template <typename _ST>
constexpr
vec3<_ST> negate(const vec3<_ST>& a)
{
	return { -a.x, -a.y, -a.z };
}

template <typename _ST>
constexpr
vec3<_ST> operator + (const vec3<_ST>& a, const vec3<_ST>& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

template <typename _ST>
constexpr
vec3<_ST> operator - (const vec3<_ST>& a, const vec3<_ST>& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

template <typename _ST>
constexpr
vec3<_ST> operator * (const vec3<_ST>& a, const vec3<_ST>& b)
{
	return { a.x * b.x, a.y * b.y, a.z * b.z };
}

template <typename _ST>
constexpr
vec3<_ST> operator / (const vec3<_ST>& a, const vec3<_ST>& b)
{
	return { a.x / b.x, a.y / b.y, a.z / b.z };
}


template <typename _ST>
constexpr
vec3<_ST> operator + (const vec3<_ST>& a, _ST s)
{
	return { a.x + s, a.y + s, a.z + s };
}

template <typename _ST>
constexpr
vec3<_ST> operator - (const vec3<_ST>& a, _ST s)
{
	return { a.x - s, a.y - s, a.z - s };
}

template <typename _ST>
constexpr
vec3<_ST> operator * (const vec3<_ST>& a, _ST s)
{
	return { a.x * s, a.y * s, a.z * s };
}

template <typename _ST>
constexpr
vec3<_ST> operator * (_ST s, const vec3<_ST>& a)
{
	return { a.x * s, a.y * s, a.z * s };
}

template <typename _ST>
constexpr
vec3<_ST> operator / (const vec3<_ST>& a, _ST s)
{
	_ST inv_s = _ST(1) / s;
	return { a.x * inv_s, a.y * inv_s, a.z * inv_s };
}

template <typename _ST>
inline
vec3<_ST>& vec3<_ST>::operator += (const vec3<_ST>& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}


template <typename _ST>
inline
vec3<_ST>& vec3<_ST>::operator -= (const vec3<_ST>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}


template <typename _ST>
inline
vec3<_ST>& vec3<_ST>::operator *= (const vec3<_ST>& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}


template <typename _ST>
inline
vec3<_ST>& vec3<_ST>::operator /= (const vec3<_ST>& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}


template <typename _ST>
inline
vec3<_ST>& vec3<_ST>::operator += (_ST s)
{
	x += s;
	y += s;
	z += s;

	return *this;
}


template <typename _ST>
inline
vec3<_ST>& vec3<_ST>::operator -= (_ST s)
{
	x -= s;
	y -= s;
	z -= s;

	return *this;
}


template <typename _ST>
inline
vec3<_ST>& vec3<_ST>::operator *= (_ST s)
{
	x *= s;
	y *= s;
	z *= s;

	return *this;
}


template <typename _ST>
inline
vec3<_ST>& vec3<_ST>::operator /= (_ST s)
{
	_ST inv_s = _ST(1) / s;
	x *= inv_s;
	y *= inv_s;
	z *= inv_s;

	return *this;
}

template <class _ST>
constexpr vec3<_ST> min(const vec3<_ST>& v1, const vec3<_ST>& v2)
{
	return { std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z) };
}

template <class _ST>
constexpr vec3<_ST> max(const vec3<_ST>& v1, const vec3<_ST>& v2)
{
	return { std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z) };
}

template <typename _ST>
constexpr
bool operator == (const vec3<_ST>& v1, const vec3<_ST>& v2)
{
	return (v1.x == v2.x &&
			v1.y == v2.y &&
			v1.z == v2.z);
}

template <typename _ST>
constexpr
bool operator != (const vec3<_ST>& v1, const vec3<_ST>& v2)
{
	return !(v1 == v2);
}


constexpr vec3f vec3f_null = vec3_null<float>;
constexpr vec3d vec3d_null = vec3_null<double>;
constexpr vec3ld vec3ld_null = vec3_null<long double>;


} // namespace math3d



#endif // _MATH3D_VEC3_H_

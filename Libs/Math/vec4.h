
/* ----------------------------------------
	File: vec4.h
	Purpose: 4 component vector class
	Author: Milan D.
	Date: 01.05.2005
   ---------------------------------------- */


#ifndef _MATH3D_VEC4_H_
#define _MATH3D_VEC4_H_

#include <type_traits>
#include "mathutil.h"


namespace math3d
{

template <typename _ST> class vec2;
template <typename _ST> class vec3;



template <typename _ST>
class vec4
{
public:
	static_assert(std::is_arithmetic_v<_ST>, "Scalar type _ST must be floating point or integral type.");

	typedef _ST scalar_t;

	static constexpr size_t num_elements = 4;

	constexpr vec4() = default;
	constexpr vec4(_ST _x, _ST _y, _ST _z, _ST _w = _ST(1));

	// conversion constructors
	explicit constexpr vec4(const vec2<_ST>& v2, _ST z = _ST(0), _ST w = _ST(1));
	explicit constexpr vec4(const vec3<_ST>& v3, _ST w = _ST(1));
	explicit constexpr vec4(const _ST* v);

	template <typename _T>
	explicit constexpr vec4(const vec4<_T>& v);

	// access
	operator _ST* ();
	operator const _ST* () const;
	_ST& operator [] (int index);
	constexpr _ST operator [] (int index) const;
	void set(_ST x, _ST y, _ST z, _ST w = _ST(1));
	void set(const _ST* v);
	void set(const vec2<_ST>& v2, _ST _z = _ST(0), _ST _w = _ST(1));
	void set(const vec3<_ST>& v3, _ST _w = _ST(1));
	constexpr void get(_ST* v) const;

	vec3<_ST>& rvec3();
	constexpr const vec3<_ST>& rvec3() const;

	vec2<_ST>& rvec2();
	constexpr const vec2<_ST>& rvec2() const;

	// operations
	_ST length() const;
	constexpr _ST length_sq() const;
	void normalize();
	void negate();
	vec4 operator - () const;
	void set_null();

	vec4& operator += (const vec4& v);
	vec4& operator -= (const vec4& v);
	vec4& operator *= (const vec4& v);
	vec4& operator /= (const vec4& v);

	vec4& operator += (_ST s);
	vec4& operator -= (_ST s);
	vec4& operator *= (_ST s);
	vec4& operator /= (_ST s);

	// data
	_ST x, y, z, w;
};


typedef vec4<float> vec4f;
typedef vec4<double> vec4d;
typedef vec4<long double> vec4ld;
typedef vec4<int> vec4i;
typedef vec4<unsigned int> vec4ui;
typedef vec4<short> vec4s;
typedef vec4<unsigned short> vec4us;
typedef vec4<char> vec4b;
typedef vec4<unsigned char> vec4ub;


} // namespace math3d

// -------------------------------------------------------------------


#include "vec2.h"
#include "vec3.h"


namespace math3d
{

template <typename _ST>
constexpr vec4<_ST> vec4_null { _ST(0), _ST(0), _ST(0), _ST(0) };


template <typename _ST>
constexpr
vec4<_ST>::vec4(_ST _x, _ST _y, _ST _z, _ST _w)
	: x { _x }
	, y { _y }
	, z { _z }
	, w { _w }
{
}

template <typename _ST>
constexpr
vec4<_ST>::vec4(const vec2<_ST>& v2, _ST _z, _ST _w)
	: x { v2.x }
	, y { v2.y }
	, z { _z }
	, w { _w }
{
}

template <typename _ST>
constexpr
vec4<_ST>::vec4(const vec3<_ST>& v3, _ST _w)
	: x { v3.x }
	, y { v3.y }
	, z { v3.z }
	, w { _w }
{
}

template <typename _ST>
constexpr
vec4<_ST>::vec4(const _ST* v)
	: x { v[0] }
	, y { v[1] }
	, z { v[2] }
	, w { v[3] }
{
}

template <typename _ST>
template <typename _T>
constexpr
vec4<_ST>::vec4(const vec4<_T>& v)
	: x { _ST(v.x) }
	, y { _ST(v.y) }
	, z { _ST(v.z) }
	, w { _ST(v.w) }
{
}

template<typename _ST>
inline
vec4<_ST>::operator _ST*()
{
	return &x;
}

template<typename _ST>
inline
vec4<_ST>::operator const _ST*() const
{
	return &x;
}

template<typename _ST>
inline
_ST& vec4<_ST>::operator[] (int index)
{
	return (&x)[index];
}

template<typename _ST>
constexpr
_ST vec4<_ST>::operator[] (int index) const
{
	return (&x)[index];
}

template <typename _ST>
inline
void vec4<_ST>::set(_ST _x, _ST _y, _ST _z, _ST _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

template <typename _ST>
inline
void vec4<_ST>::set(const _ST* v)
{
	x = v[0];
	y = v[1];
	z = v[2];
	w = v[3];
}

template <typename _ST>
inline
void vec4<_ST>::set(const vec2<_ST>& v2, _ST _z, _ST _w)
{
	x = v2.x;
	y = v2.y;
	z = _z;
	w = _w;
}

template <typename _ST>
inline
void vec4<_ST>::set(const vec3<_ST>& v3, _ST _w)
{
	x = v3.x;
	y = v3.y;
	z = v3.z;
	w = _w;
}

template <typename _ST>
constexpr
void vec4<_ST>::get(_ST* v) const
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}

template <typename _ST>
vec3<_ST>& vec4<_ST>::rvec3()
{
	return *(vec3<_ST>*)this;
}

template <typename _ST>
constexpr const vec3<_ST>& vec4<_ST>::rvec3() const
{
	return *(const vec3<_ST>*)this;
}

template <typename _ST>
vec2<_ST>& vec4<_ST>::rvec2()
{
	return *(vec2<_ST>*)this;
}

template <typename _ST>
constexpr const vec2<_ST>& vec4<_ST>::rvec2() const
{
	return *(const vec2<_ST>*)this;
}

template <typename _ST>
inline
_ST vec4<_ST>::length() const
{
	return std::sqrt(x * x + y * y + z * z + w * w);
}

template <typename _ST>
constexpr
_ST vec4<_ST>::length_sq() const
{
	return (x * x + y * y + z * z + w * w);
}


template <typename _ST>
inline
void vec4<_ST>::normalize()
{
	_ST len = length();

	if (len >= std::numeric_limits<_ST>::epsilon())
	{
		_ST i_len = _ST(1) / len;
		x *= i_len;
		y *= i_len;
		z *= i_len;
		w *= i_len;
	}
}


template <typename _ST>
inline
void vec4<_ST>::negate()
{
	set(-x, -y, -z, -w);
}

template <typename _ST>
inline
vec4<_ST> vec4<_ST>::operator - () const
{
	return { -x, -y, -z, -w };
}


template <typename _ST>
inline
void vec4<_ST>::set_null()
{
	x = y = z = w = _ST(0);
}


//---------------------------------------------------------------------


template <typename _ST>
constexpr
_ST dot(const vec4<_ST>& a, const vec4<_ST>& b)
{
	return  a.x * b.x +
			a.y * b.y +
			a.z * b.z +
			a.w * b.w;
}

// calculates cross product of a and b and stores result in r
// r = a X b
template <typename _ST>
constexpr
vec4<_ST> cross(const vec4<_ST>& a, const vec4<_ST>& b)
{
	vec4<_ST> r;
	r.x = a.y * b.z - a.z * b.y;
	r.y = a.z * b.x - a.x * b.z;
	r.z = a.x * b.y - a.y * b.x;
	return r;
}

template <typename _ST>
inline
vec4<_ST> normalize(const vec4<_ST>& a)
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
vec4<_ST> negate(const vec4<_ST>& a)
{
	return { -a.x, -a.y, -a.z, -a.w };
}

template <typename _ST>
constexpr
vec4<_ST> operator + (const vec4<_ST>& a, const vec4<_ST>& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

template <typename _ST>
constexpr
vec4<_ST> operator - (const vec4<_ST>& a, const vec4<_ST>& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

template <typename _ST>
constexpr
vec4<_ST> operator * (const vec4<_ST>& a, const vec4<_ST>& b)
{
	return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

template <typename _ST>
constexpr
vec4<_ST> operator / (const vec4<_ST>& a, const vec4<_ST>& b)
{
	return { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}


template <typename _ST>
constexpr
vec4<_ST> operator + (const vec4<_ST>& a, _ST s)
{
	return { a.x + s, a.y + s, a.z + s, a.w + s };
}

template <typename _ST>
constexpr
vec4<_ST> operator - (const vec4<_ST>& a, _ST s)
{
	return { a.x - s, a.y - s, a.z - s, a.w - s };
}

template <typename _ST>
constexpr
vec4<_ST> operator * (const vec4<_ST>& a, _ST s)
{
	return { a.x * s, a.y * s, a.z * s, a.w * s };
}

template <typename _ST>
constexpr
vec4<_ST> operator * (_ST s, const vec4<_ST>& a)
{
	return { a.x * s, a.y * s, a.z * s, a.w * s };
}

template <typename _ST>
constexpr
vec4<_ST> operator / (const vec4<_ST>& a, _ST s)
{
	_ST inv_s = _ST(1) / s;
	return { a.x * inv_s, a.y * inv_s, a.z * inv_s, a.w * inv_s };
}


template <typename _ST>
inline
vec4<_ST>& vec4<_ST>::operator += (const vec4<_ST>& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}


template <typename _ST>
inline
vec4<_ST>& vec4<_ST>::operator -= (const vec4<_ST>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}


template <typename _ST>
inline
vec4<_ST>& vec4<_ST>::operator *= (const vec4<_ST>& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}


template <typename _ST>
inline
vec4<_ST>& vec4<_ST>::operator /= (const vec4<_ST>& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}


template <typename _ST>
inline
vec4<_ST>& vec4<_ST>::operator += (_ST s)
{
	x += s;
	y += s;
	z += s;
	w += s;

	return *this;
}


template <typename _ST>
inline
vec4<_ST>& vec4<_ST>::operator -= (_ST s)
{
	x -= s;
	y -= s;
	z -= s;
	w -= s;

	return *this;
}


template <typename _ST>
inline
vec4<_ST>& vec4<_ST>::operator *= (_ST s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;

	return *this;
}


template <typename _ST>
inline
vec4<_ST>& vec4<_ST>::operator /= (_ST s)
{
	_ST inv_s = _ST(1) / s;
	x *= inv_s;
	y *= inv_s;
	z *= inv_s;
	w *= inv_s;

	return *this;
}


template <typename _ST>
constexpr
bool operator == (const vec4<_ST>& v1, const vec4<_ST>& v2)
{
	return (v1.x == v2.x &&
			v1.y == v2.y &&
			v1.z == v2.z &&
			v1.w == v2.w);
}

template <typename _ST>
constexpr
bool operator != (const vec4<_ST>& v1, const vec4<_ST>& v2)
{
	return !(v1 == v2);
}


constexpr vec4f vec4f_null = vec4_null<float>;
constexpr vec4d vec4d_null = vec4_null<double>;
constexpr vec4ld vec4ld_null = vec4_null<long double>;


} // namespace math3d



#endif // _MATH3D_VEC4_H_

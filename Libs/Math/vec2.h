
/* ----------------------------------------
	File: vec2.h
	Purpose: 2 component vector class
	Author: Milan D.
	Date: 01.05.2005
   ---------------------------------------- */


#ifndef _MATH3D_VEC2_H_
#define _MATH3D_VEC2_H_

#include <type_traits>
#include <algorithm>
#include <limits>
#include "mathutil.h"


namespace math3d
{

template <typename _ST> class vec3;
template <typename _ST> class vec4;



template <typename _ST>
class vec2
{
public:
	static_assert(std::is_arithmetic_v<_ST>, "Scalar type _ST must be floating point or integral type.");

	typedef _ST scalar_t;

	static constexpr size_t num_elements = 2;

	constexpr vec2() = default;
	constexpr vec2(_ST _x, _ST _y);

	// conversion constructors
	explicit constexpr vec2(const vec3<_ST>& v3);
	explicit constexpr vec2(const vec4<_ST>& v4);
	explicit constexpr vec2(const _ST* v);

	template <typename _T>
	explicit constexpr vec2(const vec2<_T>& v);

	// access
	operator _ST* ();
	operator const _ST* () const;
	_ST& operator [] (int index);
	constexpr _ST operator [] (int index) const;
	void set(_ST x, _ST y);
	void set(const _ST* v);
	constexpr void get(_ST* v) const;

	// operations
	_ST length() const;
	constexpr _ST length_sq() const;
	void normalize();
	void negate();
	vec2 operator - () const;
	void set_null();

	vec2& operator += (const vec2& v);
	vec2& operator -= (const vec2& v);
	vec2& operator *= (const vec2& v);
	vec2& operator /= (const vec2& v);

	vec2& operator += (_ST s);
	vec2& operator -= (_ST s);
	vec2& operator *= (_ST s);
	vec2& operator /= (_ST s);

	// data
	_ST x, y;
};

typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<long double> vec2ld;
typedef vec2<int> vec2i;
typedef vec2<unsigned int> vec2ui;
typedef vec2<short> vec2s;
typedef vec2<unsigned short> vec2us;
typedef vec2<char> vec2b;
typedef vec2<unsigned char> vec2ub;

} // namespace math3d

// -------------------------------------------------------------------


#include "vec3.h"
#include "vec4.h"


namespace math3d
{

template <typename _ST>
constexpr vec2<_ST> vec2_x_axis { _ST(1), _ST(0) };

template <typename _ST>
constexpr vec2<_ST> vec2_y_axis { _ST(0), _ST(1) };

template <typename _ST>
constexpr vec2<_ST> vec2_null { _ST(0), _ST(0) };


template <typename _ST>
constexpr
vec2<_ST>::vec2(_ST _x, _ST _y)
	: x { _x }
	, y { _y }
{
}

template <typename _ST>
constexpr
vec2<_ST>::vec2(const vec3<_ST>& v3)
	: x { v3.x }
	, y { v3.y }
{
}

template <typename _ST>
constexpr
vec2<_ST>::vec2(const vec4<_ST>& v4)
	: x { v4.x }
	, y { v4.y }
{
}

template <typename _ST>
constexpr
vec2<_ST>::vec2(const _ST* v)
	: x { v[0] }
	, y { v[1] }
{
}

template <typename _ST>
template <typename _T>
constexpr
vec2<_ST>::vec2(const vec2<_T>& v)
	: x { _ST(v.x) }
	, y { _ST(v.y) }
{
}

template<typename _ST>
inline
vec2<_ST>::operator _ST*()
{
	return &x;
}

template<typename _ST>
inline
vec2<_ST>::operator const _ST*() const
{
	return &x;
}

template<typename _ST>
inline
_ST& vec2<_ST>::operator[] (int index)
{
	return (&x)[index];
}

template<typename _ST>
constexpr
_ST vec2<_ST>::operator[] (int index) const
{
	return (&x)[index];
}

template <typename _ST>
inline
void vec2<_ST>::set(_ST _x, _ST _y)
{
	x = _x;
	y = _y;
}

template <typename _ST>
inline
void vec2<_ST>::set(const _ST* v)
{
	x = v[0];
	y = v[1];
}

template <typename _ST>
constexpr
void vec2<_ST>::get(_ST* v) const
{
	v[0] = x;
	v[1] = y;
}

template <typename _ST>
inline
_ST vec2<_ST>::length() const
{
	return std::sqrt(x * x + y * y);
}

template <typename _ST>
constexpr
_ST vec2<_ST>::length_sq() const
{
	return (x * x + y * y);
}


template <typename _ST>
inline
void vec2<_ST>::normalize()
{
	_ST len = length();

	if (len >= std::numeric_limits<_ST>::epsilon())
	{
		_ST i_len = _ST(1) / len;
		x *= i_len;
		y *= i_len;
	}
}


template <typename _ST>
inline
void vec2<_ST>::negate()
{
	set(-x, -y);
}

template <typename _ST>
inline
vec2<_ST> vec2<_ST>::operator - () const
{
	return { -x, -y };
}

template <typename _ST>
inline
void vec2<_ST>::set_null()
{
	x = y = _ST(0);
}


//---------------------------------------------------------------------
// operations

template <typename _ST>
constexpr
_ST dot(const vec2<_ST>& a, const vec2<_ST>& b)
{
	return  a.x * b.x +
			a.y * b.y;
}

template <typename _ST>
inline
vec2<_ST> normalize(const vec2<_ST>& a)
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
vec2<_ST> negate(const vec2<_ST>& a)
{
	return { -a.x, -a.y };
}

template <typename _ST>
constexpr
vec2<_ST> operator + (const vec2<_ST>& a, const vec2<_ST>& b)
{
	return { a.x + b.x, a.y + b.y };
}

template <typename _ST>
constexpr
vec2<_ST> operator - (const vec2<_ST>& a, const vec2<_ST>& b)
{
	return { a.x - b.x, a.y - b.y };
}

template <typename _ST>
constexpr
vec2<_ST> operator * (const vec2<_ST>& a, const vec2<_ST>& b)
{
	return { a.x * b.x, a.y * b.y };
}

template <typename _ST>
constexpr
vec2<_ST> operator / (const vec2<_ST>& a, const vec2<_ST>& b)
{
	return { a.x / b.x, a.y / b.y };
}


template <typename _ST>
constexpr
vec2<_ST> operator + (const vec2<_ST>& a, _ST s)
{
	return { a.x + s, a.y + s };
}

template <typename _ST>
constexpr
vec2<_ST> operator - (const vec2<_ST>& a, _ST s)
{
	return { a.x - s, a.y - s };
}

template <typename _ST>
constexpr
vec2<_ST> operator * (const vec2<_ST>& a, _ST s)
{
	return { a.x * s, a.y * s };
}

template <typename _ST>
constexpr
vec2<_ST> operator * (_ST s, const vec2<_ST>& a)
{
	return { a.x * s, a.y * s };
}


template <typename _ST>
constexpr
vec2<_ST> operator / (const vec2<_ST>& a, _ST s)
{
	_ST inv_s = _ST(1) / s;
	return { a.x * inv_s, a.y * inv_s };
}

template <typename _ST>
inline
vec2<_ST>& vec2<_ST>::operator += (const vec2<_ST>& v)
{
	x += v.x;
	y += v.y;

	return *this;
}


template <typename _ST>
inline
vec2<_ST>& vec2<_ST>::operator -= (const vec2<_ST>& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}


template <typename _ST>
inline
vec2<_ST>& vec2<_ST>::operator *= (const vec2<_ST>& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}


template <typename _ST>
inline
vec2<_ST>& vec2<_ST>::operator /= (const vec2<_ST>& v)
{
	x /= v.x;
	y /= v.y;

	return *this;
}


template <typename _ST>
inline
vec2<_ST>& vec2<_ST>::operator += (_ST s)
{
	x += s;
	y += s;

	return *this;
}


template <typename _ST>
inline
vec2<_ST>& vec2<_ST>::operator -= (_ST s)
{
	x -= s;
	y -= s;

	return *this;
}


template <typename _ST>
inline
vec2<_ST>& vec2<_ST>::operator *= (_ST s)
{
	x *= s;
	y *= s;

	return *this;
}


template <typename _ST>
inline
vec2<_ST>& vec2<_ST>::operator /= (_ST s)
{
	_ST inv_s = _ST(1) / s;
	x *= inv_s;
	y *= inv_s;

	return *this;
}

template <class _ST>
constexpr vec2<_ST> min(const vec2<_ST>& v1, const vec2<_ST>& v2)
{
	return { std::min(v1.x, v2.x), std::min(v1.y, v2.y) };
}

template <class _ST>
constexpr vec2<_ST> max(const vec2<_ST>& v1, const vec2<_ST>& v2)
{
	return { std::max(v1.x, v2.x), std::max(v1.y, v2.y) };
}

template <typename _ST>
constexpr
bool operator == (const vec2<_ST>& v1, const vec2<_ST>& v2)
{
	return (v1.x == v2.x &&
			v1.y == v2.y);
}

template <typename _ST>
constexpr
bool operator != (const vec2<_ST>& v1, const vec2<_ST>& v2)
{
	return !(v1 == v2);
}


constexpr vec2f vec2f_null = vec2_null<float>;
constexpr vec2d vec2d_null = vec2_null<double>;
constexpr vec2ld vec2ld_null = vec2_null<long double>;

constexpr vec2f vec2f_x_axis = vec2_x_axis<float>;
constexpr vec2f vec2f_y_axis = vec2_y_axis<float>;
constexpr vec2d vec2d_x_axis = vec2_x_axis<double>;
constexpr vec2d vec2d_y_axis = vec2_y_axis<double>;
constexpr vec2ld vec2ld_x_axis = vec2_x_axis<long double>;
constexpr vec2ld vec2ld_y_axis = vec2_y_axis<long double>;


} // namespace math3d



#endif // _MATH3D_VEC2_H_

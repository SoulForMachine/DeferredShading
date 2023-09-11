
/* ----------------------------------------
	File: quat.h
	Purpose: quaternion class
	Author: Milan D.
	Date: 01.05.2005
   ---------------------------------------- */


#ifndef _MATH3D_QUAT_H_
#define _MATH3D_QUAT_H_


namespace math3d
{

template <class _ST> class vec3;
template <class _ST> class mat3;
template <class _ST> class mat4;


template <class _ST>
class quat
{
public:
	typedef _ST scalar_t;

	constexpr quat() = default;
	
	// construct from Euler angles
	quat(_ST euler_x, _ST euler_y, _ST euler_z);

	// explicitly set quaternion values
	constexpr quat(_ST _x, _ST _y, _ST _z, _ST _w);

	quat(const vec3<_ST>& axis, _ST angle);

	// conversion constructors
	explicit constexpr quat(const _ST* q);
	explicit quat(const mat3<_ST>& mat);
	explicit quat(const mat4<_ST>& mat);

	template <class _T>
	explicit constexpr quat(const quat<_T>& q);

	void from_euler(_ST _x, _ST _y, _ST _z);
	void from_axisangle(const vec3<_ST>& axis, _ST angle);
	void from_matrix(const mat3<_ST>& mat);
	void from_matrix(const mat4<_ST>& mat);

	void to_axisangle(vec3<_ST>& axis, _ST& angle) const;
	void to_matrix(mat3<_ST>& mat) const;
	void to_matrix(mat4<_ST>& mat) const;

	vec3<_ST>& imag();
	constexpr const vec3<_ST>& imag() const;
	constexpr _ST real() const;

	// operations
	void conjugate();
	constexpr _ST norm() const;
	void inverse();
	void set_identity();
	void normalize();

	_ST x, y, z, w;
};


typedef quat<float> quatf;
typedef quat<double> quatd;
typedef quat<long double> quatld;


} // namespace math3d


#include "vec3.h"
#include "mat3.h"
#include "mat4.h"


namespace math3d
{


template <class _ST>
constexpr quat<_ST> quat_identity {
	_ST(0), _ST(0), _ST(0), _ST(1)
};


// construction

template <class _ST>
constexpr
quat<_ST>::quat(const _ST* q)
	: x { q[0] }
	, y { q[1] }
	, z { q[2] }
	, w { q[3] }
{
}

template <class _ST>
inline
quat<_ST>::quat(_ST euler_x, _ST euler_y, _ST euler_z)
{
	from_euler(euler_x, euler_y, euler_z);
}

template <class _ST>
constexpr
quat<_ST>::quat(_ST _x, _ST _y, _ST _z, _ST _w)
	: x { _x }
	, y { _y }
	, z { _z }
	, w { _w }
{
}


template <class _ST>
inline
quat<_ST>::quat(const vec3<_ST>& axis, _ST angle)
{
	from_axisangle(axis, angle);
}

template <class _ST>
inline
quat<_ST>::quat(const mat3<_ST>& mat)
{
	from_matrix(mat);
}

template <class _ST>
inline
quat<_ST>::quat(const mat4<_ST>& mat)
{
	from_matrix(mat);
}

template <class _ST>
template <class _T>
constexpr
quat<_ST>::quat(const quat<_T>& q)
	: x { _ST(q.x) }
	, y { _ST(q.y) }
	, z { _ST(q.z) }
	, w { _ST(q.w) }
{
}


template <class _ST>
void quat<_ST>::from_euler(_ST _x, _ST _y, _ST _z)
{
	float angle;

    angle = _x * 0.5f;
    const float sr = sin(angle);
    const float cr = cos(angle);

    angle = _y * 0.5f;
    const float sp = sin(angle);
    const float cp = cos(angle);

    angle = _z * 0.5f;
    const float sy = sin(angle);
    const float cy = cos(angle);

    const float cpcy = cp * cy;
    const float spcy = sp * cy;
    const float cpsy = cp * sy;
    const float spsy = sp * sy;

    x = sr * cpcy - cr * spsy;
    y = cr * spcy + sr * cpsy;
    z = cr * cpsy - sr * spcy;
    w = cr * cpcy + sr * spsy;

	normalize();
}

template <class _ST>
void quat<_ST>::from_axisangle(const vec3<_ST>& axis, _ST angle)
{
	_ST half_angle = angle * _ST(0.5);
	_ST sin_a = sin(half_angle);

	x = axis.x * sin_a;
	y = axis.y * sin_a;
	z = axis.z * sin_a;
	w = cos(half_angle);
}

template <class _ST>
void quat<_ST>::from_matrix(const mat3<_ST>& mat)
{
	_ST trace = _ST(1) + mat(0) + mat(4) + mat(8);

	if(fcmp_gt(trace, _ST(0)))
	{
		_ST s = sqrt(trace) * _ST(2);
		_ST inv_s = _ST(1) / s;
		x = (mat(7) - mat(5)) * inv_s;
		y = (mat(2) - mat(6)) * inv_s;
		z = (mat(3) - mat(1)) * inv_s;
		w = _ST(0.25) * s;
	}
	else
	{
		if(mat(0) > mat(4) && mat(0) > mat(8))
		{
			_ST s = sqrt(_ST(1) + mat(0) - mat(4) - mat(8)) * _ST(2);
			_ST inv_s = _ST(1) / s;
			x = _ST(0.25) * s;
			y = (mat(3) + mat(1)) * inv_s;
			z = (mat(2) + mat(6)) * inv_s;
			w = (mat(7) - mat(5)) * inv_s;
		}
		else if(mat(4) > mat(8))
		{
			_ST s = sqrt(_ST(1) + mat(4) - mat(0) - mat(8)) * _ST(2);
			_ST inv_s = _ST(1) / s;
			x = (mat(3) + mat(1)) * inv_s;
			y = _ST(0.25) * s;
			z = (mat(7) + mat(5)) * inv_s;
			w = (mat(2) - mat(6)) * inv_s;
		}
		else
		{
			_ST s = sqrt(_ST(1) + mat(8) - mat(0) - mat(4)) * _ST(2);
			_ST inv_s = _ST(1) / s;
			x = (mat(2) + mat(6) ) * inv_s;
			y = (mat(7) + mat(5) ) * inv_s;
			z = _ST(0.25) * s;
			w = (mat(3) - mat(1)) * inv_s;
		}
	}
}

template <class _ST>
void quat<_ST>::from_matrix(const mat4<_ST>& mat)
{
	_ST trace = 1 + mat(0) + mat(5) + mat(10);

	if(fcmp_gt(trace, _ST(0)))
	{
		_ST s = sqrt(trace) * _ST(2);
		_ST inv_s = _ST(1) / s;
		x = (mat(9) - mat(6)) * inv_s;
		y = (mat(2) - mat(8)) * inv_s;
		z = (mat(4) - mat(1)) * inv_s;
		w = _ST(0.25) * s;
	}
	else
	{
		if(mat(0) > mat(5) && mat(0) > mat(10))
		{
			_ST s = sqrt(_ST(1) + mat(0) - mat(5) - mat(10)) * 2;
			_ST inv_s = _ST(1) / s;
			x = _ST(0.25) * s;
			y = (mat(4) + mat(1)) * inv_s;
			z = (mat(2) + mat(8)) * inv_s;
			w = (mat(9) - mat(6)) * inv_s;
		}
		else if(mat(5) > mat(10))
		{
			_ST s = sqrt(_ST(1) + mat(5) - mat(0) - mat(10)) * 2;
			_ST inv_s = _ST(1) / s;
			x = (mat(4) + mat(1)) * inv_s;
			y = _ST(0.25) * s;
			z = (mat(9) + mat(6)) * inv_s;
			w = (mat(2) - mat(8)) * inv_s;
		}
		else
		{
			_ST s = sqrt(_ST(1) + mat(10) - mat(0) - mat(5)) * 2;
			_ST inv_s = _ST(1) / s;
			x = (mat(2) + mat(8) ) * inv_s;
			y = (mat(9) + mat(6) ) * inv_s;
			z = _ST(0.25) * s;
			w = (mat(4) - mat(1)) * inv_s;
		}
	}
}


template <class _ST>
void quat<_ST>::to_axisangle(vec3<_ST>& axis, _ST& angle) const
{
	angle = acos(w) * _ST(2);

	_ST inv_sin_a = _ST(1) / sin(angle);
	axis.x = x * inv_sin_a;
	axis.y = y * inv_sin_a;
	axis.z = z * inv_sin_a;
}

template <class _ST>
void quat<_ST>::to_matrix(mat3<_ST>& mat) const
{
	_ST xx = x * x;
	_ST yy = y * y;
	_ST zz = z * z;
	_ST xy = x * y;
	_ST xz = x * z;
	_ST wx = w * x;
	_ST wy = w * y;
	_ST wz = w * z;
	_ST yz = y * z;

	mat(0) = _ST(1) - _ST(2) * (yy + zz);
	mat(3) = _ST(2) * (xy - wz);
	mat(6) = _ST(2) * (xz + wy);

	mat(1) = _ST(2) * (xy + wz);
	mat(4) = _ST(1) - _ST(2) * (xx + zz);
	mat(7) = _ST(2) * (yz - wx);

	mat(2) = _ST(2) * (xz - wy);
	mat(5) = _ST(2) * (yz + wx);
	mat(8) = _ST(1) - _ST(2) * (xx + yy);
}

template <class _ST>
void quat<_ST>::to_matrix(mat4<_ST>& mat) const
{
	_ST xx = x * x;
	_ST yy = y * y;
	_ST zz = z * z;
	_ST xy = x * y;
	_ST xz = x * z;
	_ST wx = w * x;
	_ST wy = w * y;
	_ST wz = w * z;
	_ST yz = y * z;

	mat(0) = _ST(1) - _ST(2) * (yy + zz);
	mat(4) = _ST(2) * (xy - wz);
	mat(8) = _ST(2) * (xz + wy);
	mat(12) = _ST(0);

	mat(1) = _ST(2) * (xy + wz);
	mat(5) = _ST(1) - _ST(2) * (xx + zz);
	mat(9) = _ST(2) * (yz - wx);
	mat(13) = _ST(0);

	mat(2) = _ST(2) * (xz - wy);
	mat(6) = _ST(2) * (yz + wx);
	mat(10) = _ST(1) - _ST(2) * (xx + yy);
	mat(14) = _ST(0);

	mat(3) = _ST(0);
	mat(7) = _ST(0);
	mat(11) = _ST(0);
	mat(15) = _ST(1);
}

template<class _ST>
inline vec3<_ST>& quat<_ST>::imag()
{
	return *(vec3<_ST>*)this;
}

template<class _ST>
constexpr const vec3<_ST>& quat<_ST>::imag() const
{
	return *(const vec3<_ST>*)this;
}

template<class _ST>
constexpr _ST quat<_ST>::real() const
{
	return w;
}


// operations
template <class _ST>
inline
void quat<_ST>::conjugate()
{
	x = -x; y = -y; z = -z;
}

template <class _ST>
constexpr
_ST quat<_ST>::norm() const
{
	return x * x + y * y + z * z + w * w;
}

template <class _ST>
void quat<_ST>::inverse()
{
	_ST inv_norm = _ST(1) / norm();
	x = -x * inv_norm;
	y = -y * inv_norm;
	z = -z * inv_norm;
	w = w * inv_norm;
}

template <class _ST>
inline
void quat<_ST>::set_identity()
{
	*this = quat_identity<_ST>;
}

template <class _ST>
inline
void quat<_ST>::normalize()
{
	_ST n = norm();

	if (n >= std::numeric_limits<_ST>::epsilon())
	{
		_ST inv_len = _ST(1) / std::sqrt(n);

		x *= inv_len;
		y *= inv_len;
		z *= inv_len;
		w *= inv_len;
	}
}

// operations

template <class _ST>
constexpr
_ST dot(const quat<_ST>& a, const quat<_ST>& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <class _ST>
constexpr
quat<_ST> operator * (const quat<_ST>& a, const quat<_ST>& b)
{
	return {
		a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
		a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
		a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
		a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
	};
}

template <class _ST>
quat<_ST> slerp(const quat<_ST>& a, const quat<_ST>& b, _ST t)
{
	quat<_ST> result;
	_ST theta = acos(dot(a, b));
	_ST inv_sin_theta = _ST(1) / sin(theta);
	_ST f1 = sin(theta * (_ST(1) - t)) * inv_sin_theta;
	_ST f2 = sin(theta * t) * inv_sin_theta;

	result.x = f1 * a.x + f2 * b.x;
	result.y = f1 * a.y + f2 * b.y;
	result.z = f1 * a.z + f2 * b.z;
	result.w = f1 * a.w + f2 * b.w;
	return result;
}

template <class _ST>
quat<_ST> pow(const quat<_ST>& q)
{
	quat<_ST> result;
	_ST angle = acos(q.w) * _ST(2);
	_ST angle_exp = angle * exp;
	_ST sin_angle_exp = sin(angle_exp);

	normalize(result.imag, q.imag);
	result.imag *= sin_angle_exp;
	result.real = cos(angle_exp);
	return result;
}

template <class _ST>
quat<_ST> log(const quat<_ST>& q)
{
	quat<_ST> result;
	normalize(result, q.imag);
	result *= acos(q.w);
	return result;
}

template <class _ST>
quat<_ST> conjugate(const quat<_ST>& q)
{
	quat<_ST> result(q);
	result.conjugate();
	return result;
}

template <class _ST>
quat<_ST> inverse(const quat<_ST>& q)
{
	quat<_ST> result(q);
	result.inverse();
	return result;
}


constexpr quat<float> quat_identity_f = quat_identity<float>;
constexpr quat<double> quat_identity_d = quat_identity<double>;
constexpr quat<long double> quat_identity_ld = quat_identity<long double>;

} // namespace math3d



#endif //_MATH3D_QUAT_H_

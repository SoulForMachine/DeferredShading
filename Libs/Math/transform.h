

#ifndef _MATH3D_TRANSFORM_H_
#define _MATH3D_TRANSFORM_H_

/*
	Functions for transforming vectors with matrices and quaternions
*/

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat3.h"
#include "mat4.h"
#include "quat.h"
#include "mathutil.h"



namespace math3d
{

// ----------- vector transform by matrix ------------

template <class _ST>
vec3<_ST> operator * (const vec3<_ST>& v, const mat3<_ST>& m)
{
	vec3<_ST> r;
	r.x = m(0) * v.x + m(3) * v.y + m(6) * v.z;
	r.y = m(1) * v.x + m(4) * v.y + m(7) * v.z;
	r.z = m(2) * v.x + m(5) * v.y + m(8) * v.z;
	return r;
}

template <class _ST>
vec4<_ST> operator * (const vec4<_ST>& v, const mat4<_ST>& m)
{
	vec4<_ST> r;
	r.x = m(0) * v.x + m(4) * v.y + m(8) * v.z + m(12) * v.w;
	r.y = m(1) * v.x + m(5) * v.y + m(9) * v.z + m(13) * v.w;
	r.z = m(2) * v.x + m(6) * v.y + m(10) * v.z + m(14) * v.w;
	r.w = m(3) * v.x + m(7) * v.y + m(11) * v.z + m(15) * v.w;
	return r;
}

template <class _ST>
vec3<_ST> operator * (const vec3<_ST>& v, const mat4<_ST>& m)
{
	vec3<_ST> r;
	r.x = m(0) * v.x + m(4) * v.y + m(8) * v.z + m(12);
	r.y = m(1) * v.x + m(5) * v.y + m(9) * v.z + m(13);
	r.z = m(2) * v.x + m(6) * v.y + m(10) * v.z + m(14);
	return r;
}

template <typename _ST>
vec2<_ST> operator * (const vec2<_ST>& v, const mat3<_ST>& m)
{
	vec2<_ST> r;
	r.x = m(0) * v.x + m(3) * v.y + m(6);
	r.y = m(1) * v.x + m(4) * v.y + m(7);
	return r;
}

// Decomposes matrix m into translation, rotation and scale factors.
// NOTE: scale will always be positive, that is, decomposition cannot
// detect if the matrix was constructed originally with negative scale
// (mirroring) on both axes or rotation.
template <typename _ST>
void decompose_2d(const mat3<_ST>& m, vec2<_ST>& transl, _ST& rot, vec2<_ST>& scale)
{
	transl.set(m(6), m(7));

	rot = std::atan2(m(1), m(4));
	rot = (rot < 0.0) ? TWO_PI + rot : rot;

	scale.x = std::sqrt(m(0) * m(0) + m(3) * m(3));
	scale.y = std::sqrt(m(1) * m(1) + m(4) * m(4));
}

template <typename _ST>
void compose_2d(mat3<_ST>& m, const vec2<_ST>& transl, _ST rot, const vec2<_ST>& scale)
{
	m.set_scale_2d(scale);
	m.rotate_2d(rot);
	m.translate_2d(transl);
}

// transforms direction vector v with 3x3 sub-matrix of matrix m
// m must be orthogonal
template <class _ST>
vec3<_ST> transform_dir(const vec3<_ST>& v, const mat4<_ST>& m)
{
	vec3<_ST> r;
	r.x = m(0) * v.x + m(4) * v.y + m(8) * v.z;
	r.y = m(1) * v.x + m(5) * v.y + m(9) * v.z;
	r.z = m(2) * v.x + m(6) * v.y + m(10) * v.z;
	return r;
}


// Vector transform by quaternion.
template <class _ST>
vec3<_ST> rotate(const vec3<_ST>& v, const quat<_ST>& q)
{
	return q * v * inverse(q);
}

template <class _ST>
vec3<_ST> project(const vec3<_ST>& worldPt, const mat4<_ST>& view_proj, const vec4<_ST>& viewport)
{
	vec4<_ST> clipPt = vec4<_ST>(worldPt, _ST(1)) * view_proj;
	vec3<_ST> ndc = clipPt.rvec3() / clipPt.w;
	return vec3<_ST>(
		ndc.x * viewport.z * _ST(0.5) + viewport.x + viewport.z * _ST(0.5),
		ndc.y * viewport.w * _ST(0.5) + viewport.y + viewport.w * _ST(0.5),
		ndc.z * _ST(0.5) + _ST(0.5)
	);
}

template <class _ST>
vec3<_ST> unproject(int screen_x, int screen_y, const mat4<_ST>& inv_view_proj, const vec4<_ST>& viewport)
{
	vec4<_ST> ndc(
		((float)screen_x - viewport.x) / viewport.z * _ST(2.0) - _ST(1.0),
		((float)screen_y - viewport.y) / viewport.w * _ST(2.0) - _ST(1.0),
		_ST(-1.0),
		_ST(1.0));

	vec4f temp = ndc * inv_view_proj;
	return temp.rvec3() / temp.w;
}

} // namespace math3d


#endif // _MATH3D_TRANSFORM_H_

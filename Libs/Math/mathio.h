
/* ----------------------------------------
	File: mathio.h
	Purpose: defines operators for using 3d math 
			 library types with C++ iostreams;
			 must be included separately from
			 math3d.h
	Author: Milan D.
	Date: 01.05.2005
   ---------------------------------------- */


#ifndef _MATH3D_MATHIO_H_
#define _MATH3D_MATHIO_H_



namespace math3d
{


// output

template <class _ST>
std::ostream& operator << (std::ostream& os, const vec2<_ST>& v)
{
	return os << "[" << v.x << ", " << v.y << "]";
}

template <class _ST>
std::ostream& operator << (std::ostream& os, const vec3<_ST>& v)
{
	return os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}

template <class _ST>
std::ostream& operator << (std::ostream& os, const vec4<_ST>& v)
{
	return os << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";
}

template <class _ST>
std::ostream& operator << (std::ostream& os, const mat3<_ST>& m)
{
	for(int i = 0; i < 9; ++i)
	{
		if(!(i % 3))
			os << "| ";

		os.precision(2);
		os.width(6);
		os << m(i) << (!((i - 2) % 3)? " |\n": ", ");
	}

	return os;
}

template <class _ST>
std::ostream& operator << (std::ostream& os, const mat4<_ST>& m)
{
	for(int i = 0; i < 16; ++i)
	{
		if(!(i % 4))
			os << "| ";

		os.precision(2);
		os.width(6);
		os << m(i) << (!((i - 3) % 4)? " |\n": ", ");
	}

	return os;
}


// input

template <class _ST>
std::istream& operator >> (std::istream& is, vec2<_ST>& v)
{
	return is >> v.x >> v.y;
}

template <class _ST>
std::istream& operator >> (std::istream& is, vec3<_ST>& v)
{
	return is >> v.x >> v.y >> v.z;
}

template <class _ST>
std::istream& operator >> (std::istream& is, vec4<_ST>& v)
{
	return is >> v.x >> v.y >> v.z >> v.w;
}

template <class _ST>
std::istream& operator >> (std::istream& is, mat3<_ST>& m)
{
	for(int i = 0; i < 9; ++i)
		is >> m(i);

	return is;
}

template <class _ST>
std::istream& operator >> (std::istream& is, mat4<_ST>& m)
{
	for(int i = 0; i < 16; ++i)
		is >> m(i);

	return is;
}



} // namespace math3d


#endif // _MATH3D_MATHIO_H_

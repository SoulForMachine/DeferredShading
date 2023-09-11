
#ifndef _GLSLAYER_VERTEX_FORMAT_H_
#define _GLSLAYER_VERTEX_FORMAT_H_

#include "Common.h"


namespace gls
{

	/*
		- Normalized flag is used only if integer flag is false.
		- If integer is true, type must be one of integer types and data is treated as pure integers.
		- If integer is false and type is one of integer types, then:
			- If normalized is false, data is converted to float directly.
			- If normalized flag is true, data is converted to normalized float ([0,1] for unsigned or [-1,1] for signed types).
	*/
	struct VertexAttribDesc
	{
		int stream;
		uint attribute;
		int numComponents;
		DataType type;
		bool integer;
		bool normalized;
		uint offset;
	};

	class IVertexFormat
	{
	public:
		virtual ~IVertexFormat() = 0;
	};

	inline
	IVertexFormat::~IVertexFormat()
	{
	}

}


#endif // _GLSLAYER_VERTEX_FORMAT_H_


#ifndef _GLSLAYER_BUFFER_H_
#define _GLSLAYER_BUFFER_H_

#include "Resource.h"


namespace gls
{

	class IBuffer: public IResource
	{
	public:
		virtual void BufferSubData(intptr offset, sizeiptr size, const void* data) = 0;
		virtual void GetBufferSubData(intptr offset, sizeiptr size, void* data) = 0;
		virtual void* Map(uint map_flags) = 0;
		virtual void* MapRange(intptr offset, sizeiptr length, uint map_flags) = 0;
		virtual void FlushMappedRange(intptr offset, sizeiptr length) = 0;
		virtual bool Unmap() = 0;
		virtual void ClearData(PixelFormat internal_format, ImageFormat format, DataType type, const void* data) = 0;
		virtual void ClearSubData(PixelFormat internal_format, ImageFormat format, DataType type, intptr offset, sizeiptr size, const void* data) = 0;
		virtual void InvalidateData() = 0;
		virtual void InvalidateSubData(intptr offset, sizeiptr size) = 0;
	};

}

#endif // _GLSLAYER_BUFFER_H_


#ifndef _GL_BUFFER_H_
#define _GL_BUFFER_H_

#include "GLSlayer/Buffer.h"
#include "GLResource.h"


namespace gls::internals
{

class GLBuffer : public gls::IBuffer, public GLResource
{
public:
	IMPLEMENT_IRESOURCE

	static const int typeID = TYPE_ID_BUFFER;

	GLBuffer() = default;

	GLBuffer(const GLBuffer&) = delete;
	GLBuffer& operator = (const GLBuffer&) = delete;

	bool Create(GLState* state, sizeiptr size, const void* data, uint flags);
	void Destroy();

	virtual void* DynamicCast(int type_id) override { return (type_id == TYPE_ID_BUFFER) ? this : GLResource::DynamicCast(type_id); }
	virtual void BufferSubData(intptr offset, sizeiptr size, const void* data) override;
	virtual void GetBufferSubData(intptr offset, sizeiptr size, void* data) override;
	virtual void* Map(uint map_flags) override;
	virtual void* MapRange(intptr offset, sizeiptr length, uint map_flags) override;
	virtual void FlushMappedRange(intptr offset, sizeiptr length) override;
	virtual bool Unmap() override;
	virtual void ClearData(PixelFormat internal_format, ImageFormat format, DataType type, const void* data) override;
	virtual void ClearSubData(PixelFormat internal_format, ImageFormat format, DataType type, intptr offset, sizeiptr size, const void* data) override;
	virtual void InvalidateData() override;
	virtual void InvalidateSubData(intptr offset, sizeiptr size) override;
	sizeiptr GetSize() const { return _size; }

private:
	sizeiptr _size = 0; // size of buffer in bytes
	GLState* _glState = nullptr;
};

} // namespace gls::internals

#endif // _GL_BUFFER_H_

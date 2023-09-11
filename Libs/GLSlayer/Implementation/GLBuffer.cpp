
#include <cassert>
#include "GLBuffer.h"


#define STATE_MACHINE_HACK \
	if(_id != _glState->copyWriteBuffer) \
	{ \
		glBindBuffer(GL_COPY_WRITE_BUFFER, _id); \
		_glState->copyWriteBuffer = _id; \
	}


namespace gls::internals
{

bool GLBuffer::Create(GLState* gl_state, sizeiptr size, const void* data, uint flags)
{
	if (_id || size == 0)
		return false;

	_glState = gl_state;

	_resType = ResourceType::Buffer;
	_target = GL_COPY_WRITE_BUFFER;
	_size = size;

	glGenBuffers(1, &_id);

	if (!_id)
		return false;

	glBindBuffer(GL_COPY_WRITE_BUFFER, _id);
	_glState->copyWriteBuffer = _id;

	GLbitfield glFlags = 0;

	if (flags & BUFFER_MAP_READ_BIT)
		glFlags |= GL_MAP_READ_BIT;
	if (flags & BUFFER_MAP_WRITE_BIT)
		glFlags |= GL_MAP_WRITE_BIT;
	if (flags & BUFFER_MAP_PERSISTENT_BIT)
		glFlags |= GL_MAP_PERSISTENT_BIT;
	if (flags & BUFFER_MAP_COHERENT_BIT)
		glFlags |= GL_MAP_COHERENT_BIT;
	if (flags & BUFFER_DYNAMIC_STORAGE_BIT)
		glFlags |= GL_DYNAMIC_STORAGE_BIT;
	if (flags & BUFFER_CLIENT_STORAGE_BIT)
		glFlags |= GL_CLIENT_STORAGE_BIT;

	glBufferStorage(GL_COPY_WRITE_BUFFER, size, data, glFlags);

	if (glGetError() != GL_NO_ERROR)
	{
		Destroy();
		return false;
	}

	return true;
}

void GLBuffer::Destroy()
{
	if (_id)
	{
		if (_id == _glState->copyWriteBuffer)
		{
			glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
			_glState->copyWriteBuffer = 0;
		}

		glDeleteBuffers(1, &_id);

		_id = 0;
		_size = 0;
	}
}

void GLBuffer::BufferSubData(intptr offset, sizeiptr size, const void* data)
{
	assert(_id);
	STATE_MACHINE_HACK
	glBufferSubData(GL_COPY_WRITE_BUFFER, offset, size, data);
}

void GLBuffer::GetBufferSubData(intptr offset, sizeiptr size, void* data)
{
	assert(_id);
	STATE_MACHINE_HACK
	glGetBufferSubData(GL_COPY_WRITE_BUFFER, offset, size, data);
}

void* GLBuffer::Map(uint map_flags)
{
	return MapRange(0, _size, map_flags);
}

/*
	map_flags is a combination of BufferMapFlags
*/
void* GLBuffer::MapRange(intptr offset, sizeiptr length, uint map_flags)
{
	assert(_id);
	STATE_MACHINE_HACK

	GLbitfield bits = 0;

	if (map_flags & MAP_READ_BIT)
		bits |= GL_MAP_READ_BIT;
	if (map_flags & MAP_WRITE_BIT)
		bits |= GL_MAP_WRITE_BIT;
	if (map_flags & MAP_INVALIDATE_RANGE_BIT)
		bits |= GL_MAP_INVALIDATE_RANGE_BIT;
	if (map_flags & MAP_INVALIDATE_BUFFER_BIT)
		bits |= GL_MAP_INVALIDATE_BUFFER_BIT;
	if (map_flags & MAP_FLUSH_EXPLICIT_BIT)
		bits |= GL_MAP_FLUSH_EXPLICIT_BIT;
	if (map_flags & MAP_UNSYNCHRONIZED_BIT)
		bits |= GL_MAP_UNSYNCHRONIZED_BIT;

	void* ptr = glMapBufferRange(GL_COPY_WRITE_BUFFER, offset, length, bits);
	return ptr;
}

void GLBuffer::FlushMappedRange(intptr offset, sizeiptr length)
{
	assert(_id);
	STATE_MACHINE_HACK
	glFlushMappedBufferRange(GL_COPY_WRITE_BUFFER, offset, length);
}

bool GLBuffer::Unmap()
{
	assert(_id);
	STATE_MACHINE_HACK
	bool result = (glUnmapBuffer(GL_COPY_WRITE_BUFFER) == GL_TRUE);
	return result;
}

void GLBuffer::ClearData(PixelFormat internal_format, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	STATE_MACHINE_HACK
	glClearBufferData(GL_COPY_WRITE_BUFFER, GetGLEnum(internal_format), GetGLEnum(format), GetGLEnum(type), data);
}

void GLBuffer::ClearSubData(PixelFormat internal_format, ImageFormat format, DataType type, intptr offset, sizeiptr size, const void* data)
{
	assert(_id);
	STATE_MACHINE_HACK
	glClearBufferSubData(GL_COPY_WRITE_BUFFER, GetGLEnum(internal_format), offset, size, GetGLEnum(format), GetGLEnum(type), data);
}

void GLBuffer::InvalidateData()
{
	assert(_id);
	glInvalidateBufferData(_id);
}

void GLBuffer::InvalidateSubData(intptr offset, sizeiptr size)
{
	assert(_id);
	glInvalidateBufferSubData(_id, offset, size);
}

} // namespace gls::internals


#include <cassert>
#include "GLBuffer.h"
#include "GLTexture.h"
#include "GLFramebuffer.h"


#define STATE_MACHINE_HACK \
	if(_id != _glState->imageUnits[0].texture) \
	{ \
		if (_glState->activeTexture != 0) { \
			glActiveTexture(GL_TEXTURE0); \
			_glState->activeTexture = 0; } \
		if (_glState->imageUnits[0].texture != 0 && _glState->imageUnits[0].target != 0 && _glState->imageUnits[0].target != _target) \
			glBindTexture(_glState->imageUnits[0].target, 0); \
		glBindTexture(_target, _id); \
		_glState->imageUnits[0].texture = _id; \
		_glState->imageUnits[0].target = _target; \
	}

#define PACK_BUFFER_STATE_MACHINE_HACK \
	if(dyn_cast_ptr<GLResource*>(buffer)->GetID() != _glState->pixelPackBuf) \
	{ \
		glBindBuffer(GL_PIXEL_PACK_BUFFER, dyn_cast_ptr<GLResource*>(buffer)->GetID()); \
		_glState->pixelPackBuf = dyn_cast_ptr<GLResource*>(buffer)->GetID(); \
	}

#define UNPACK_BUFFER_STATE_MACHINE_HACK \
	if(dyn_cast_ptr<GLResource*>(buffer)->GetID() != _glState->pixelUnpackBuf) \
	{ \
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, dyn_cast_ptr<GLResource*>(buffer)->GetID()); \
		_glState->pixelUnpackBuf = dyn_cast_ptr<GLResource*>(buffer)->GetID(); \
	}

#define FRAMEBUFFER_STATE_MACHINE_HACK \
	if(dyn_cast_ptr<GLFramebuffer*>(source_fbuf)->GetID() != _glState->readFbuf) \
	{ \
		glBindFramebuffer(GL_READ_FRAMEBUFFER, dyn_cast_ptr<GLFramebuffer*>(source_fbuf)->GetID()); \
		_glState->readFbuf = dyn_cast_ptr<GLFramebuffer*>(source_fbuf)->GetID(); \
	} \
	glReadBuffer(GetGLEnum(source_color_buf)); \


namespace gls::internals
{

bool IsCompressedFormat(PixelFormat format)
{
	switch(format)
	{
	case PixelFormat::Compressed_R:
	case PixelFormat::Compressed_RG:
	case PixelFormat::Compressed_RGB:
	case PixelFormat::Compressed_RGBA:
	case PixelFormat::Compressed_SRGB:
	case PixelFormat::Compressed_SRGB_A:
	case PixelFormat::Compressed_R_RGTC1:
	case PixelFormat::Compressed_Signed_R_RGTC1:
	case PixelFormat::Compressed_RG_RGTC2:
	case PixelFormat::Compressed_Signed_RG_RGTC2:
	case PixelFormat::Compressed_RGB_DXT1:
	case PixelFormat::Compressed_RGBA_DXT1:
	case PixelFormat::Compressed_RGBA_DXT3:
	case PixelFormat::Compressed_RGBA_DXT5:
	case PixelFormat::Compressed_SRGB_DXT1:
	case PixelFormat::Compressed_SRGB_A_DXT1:
	case PixelFormat::Compressed_SRGB_A_DXT3:
	case PixelFormat::Compressed_SRGB_A_DXT5:
	case PixelFormat::Compressed_RGBA_BPTC_UNorm:
	case PixelFormat::Compressed_SRGB_Alpha_BPTC_UNorm:
	case PixelFormat::Compressed_RGB_BPTC_SignedFloat:
	case PixelFormat::Compressed_RGB_BPTC_UnsignedFloat:
	case PixelFormat::Compressed_RGB8_ETC2:
	case PixelFormat::Compressed_SRGB8_ETC2:
	case PixelFormat::Compressed_RGB8_PunchthroughAlpha1_ETC2:
	case PixelFormat::Compressed_SRGB8_PunchthroughAlpha1_ETC2:
	case PixelFormat::Compressed_RGBA8_ETC2_EAC:
	case PixelFormat::Compressed_SRGB8_ALPHA8_ETC2_EAC:
	case PixelFormat::Compressed_R11_EAC:
	case PixelFormat::Compressed_Signed_R11_EAC:
	case PixelFormat::Compressed_RG11_EAC:
	case PixelFormat::Compressed_Signed_RG11_EAC:
		return true;

	default:
		return false;
	};
}

//==================== Texture ====================

void GLTexture::Destroy()
{
	if(_id != 0)
	{
		GLuint tmp_id = _id;
		_id = 0;
		STATE_MACHINE_HACK
		glDeleteTextures(1, &tmp_id);
	}
}

TextureType GLTexture::GetTextureType()
{
	return _textureType;
}

void GLTexture::GenerateMipmap()
{
	if(_textureType != TextureType::TexBuffer && _textureType != TextureType::TexRectangle)
	{
		STATE_MACHINE_HACK
		glGenerateMipmap(_target);
	}
	else
	{
		assert(0);
	}
}

void GLTexture::SetBaseLevel(int base_level)
{
	STATE_MACHINE_HACK
	glTexParameteri(_target, GL_TEXTURE_BASE_LEVEL, base_level);
	_baseLevel = base_level;
}

void GLTexture::SetMaxLevel(int max_level)
{
	STATE_MACHINE_HACK
	glTexParameteri(_target, GL_TEXTURE_MAX_LEVEL, max_level);
	_maxLevel = max_level;
}

int GLTexture::GetCompressedSize(int level) const
{
	if(_textureType != TextureType::TexBuffer && _textureType != TextureType::TexRectangle)
	{
		STATE_MACHINE_HACK
		GLint size;
		glGetTexLevelParameteriv(_target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &size);
		return size;
	}
	else
	{
		assert(0);
		return 0;
	}
}

void GLTexture::ComponentSwizzle(TexSwizzleDest dest, TexSwizzleSource source)
{
	STATE_MACHINE_HACK
	glTexParameteri(_target, GetGLEnum(dest), GetGLEnum(source));
}

void GLTexture::ComponentSwizzle(TexSwizzleSource source_red, TexSwizzleSource source_green, TexSwizzleSource source_blue, TexSwizzleSource source_alpha)
{
	GLint src_vals[] = {
		(GLint)GetGLEnum(source_red),
		(GLint)GetGLEnum(source_green),
		(GLint)GetGLEnum(source_blue),
		(GLint)GetGLEnum(source_alpha)
	};
	STATE_MACHINE_HACK
	glTexParameteriv(_target, GL_TEXTURE_SWIZZLE_RGBA, src_vals);
}

void GLTexture::DepthStencilMode(DepthStencilTexMode mode)
{
	STATE_MACHINE_HACK
	glTexParameteri(_target, GL_DEPTH_STENCIL_TEXTURE_MODE, GetGLEnum(mode));
}

//==================== Texture1D ====================

bool GLTexture1D::Create(GLState* gl_state)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex1D;
	_target = GL_TEXTURE_1D;
	_width = 0;

	STATE_MACHINE_HACK

	return true;
}

bool GLTexture1D::CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width)
{
	if(!Create(gl_state))
		return false;

	glTexStorage1D(GL_TEXTURE_1D, levels, GetGLEnum(internal_format), width);

	_format = internal_format;
	_width = width;

	return true;
}

bool GLTexture1D::CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	glTextureView(_id, GL_TEXTURE_1D, orig_tex, GetGLEnum(internal_format), min_level, num_levels, min_layer, 1);

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex1D;
	_target = GL_TEXTURE_1D;
	_format = internal_format;

	STATE_MACHINE_HACK

	glGetTexLevelParameteriv(GL_TEXTURE_1D, 0, GL_TEXTURE_WIDTH, &_width);

	return true;
}

void GLTexture1D::TexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelUnpackState(_glState, pixel_store);
	glTexSubImage1D(GL_TEXTURE_1D, level, xoffset, width, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTexture1D::TexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset)
{
	UNPACK_BUFFER_STATE_MACHINE_HACK
	TexSubImage(level, xoffset, width, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTexture1D::CompressedTexSubImage(int level, int xoffset, int width, ImageFormat format, sizei size, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	glCompressedTexSubImage1D(GL_TEXTURE_1D, level, xoffset, width, GetGLEnum(format), size, pixels);
}

void GLTexture1D::CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, PixelFormat internal_format, int x, int y, int width)
{
	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexImage1D(GL_TEXTURE_1D, level, GetGLEnum(internal_format), x, y, width, 0);

	if(level == 0)
	{
		_format = internal_format;
		_width = width;
	}
}

void GLTexture1D::CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int x, int y, int width)
{
	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexSubImage1D(GL_TEXTURE_1D, level, xoffset, x, y, width);
}

void GLTexture1D::InvalidateTexImage(int level)
{
	assert(_id);
	glInvalidateTexImage(_id, level);
}

void GLTexture1D::InvalidateTexSubImage(int level, int xoffset, int width)
{
	assert(_id);
	glInvalidateTexSubImage(_id, level, xoffset, 0, 0, width, 1, 1);
}

void GLTexture1D::ClearTexImage(int level, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexImage(_id, level, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTexture1D::ClearTexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexSubImage(_id, level, xoffset, 0, 0, width, 1, 1, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTexture1D::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTexImage(GL_TEXTURE_1D, level, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTexture1D::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetTexImage(level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTexture1D::GetTexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const
{
	assert(_id);

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, 0, 0, width, 1, 1, GetGLEnum(format), GetGLEnum(type), bufferSize, pixels);
}

void GLTexture1D::GetTexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);
	PACK_BUFFER_STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, 0, 0, width, 1, 1, GetGLEnum(format), GetGLEnum(type), bufferSize, BUFFER_OFFSET(buffer_offset));
}

void GLTexture1D::GetCompressedTexImage(int level, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	glGetCompressedTexImage(GL_TEXTURE_1D, level, pixels);
}

void GLTexture1D::GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetCompressedTexImage(level, BUFFER_OFFSET(buffer_offset));
}

void GLTexture1D::GetCompressedTexSubImage(int level, int xoffset, int width, void* pixels, sizei bufferSize) const
{
	assert(_id);

	glGetCompressedTextureSubImage(_id, level, xoffset, 0, 0, width, 1, 1, bufferSize, pixels);
}

void GLTexture1D::GetCompressedTexSubImage(int level, int xoffset, int width, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);

	PACK_BUFFER_STATE_MACHINE_HACK
	glGetCompressedTextureSubImage(_id, level, xoffset, 0, 0, width, 1, 1, bufferSize, BUFFER_OFFSET(buffer_offset));
}


//==================== Texture2D ====================

bool GLTexture2D::Create(GLState* gl_state)
{
	if(_id)
		return false;
	
	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex2D;
	_target = GL_TEXTURE_2D;
	_width = 0;
	_height = 0;

	STATE_MACHINE_HACK

	return true;
}

bool GLTexture2D::CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width, int height)
{
	if(!Create(gl_state))
		return false;

	glTexStorage2D(GL_TEXTURE_2D, levels, GetGLEnum(internal_format), width, height);

	_format = internal_format;
	_width = width;
	_height = height;

	return true;
}

bool GLTexture2D::CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	glTextureView(_id, GL_TEXTURE_2D, orig_tex, GetGLEnum(internal_format), min_level, num_levels, min_layer, 1);

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex2D;
	_target = GL_TEXTURE_2D;
	_format = internal_format;

	STATE_MACHINE_HACK

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_height);

	return true;
}

void GLTexture2D::TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelUnpackState(_glState, pixel_store);
	glTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width, height, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTexture2D::TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset)
{
	UNPACK_BUFFER_STATE_MACHINE_HACK
	TexSubImage(level, xoffset, yoffset, width, height, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTexture2D::CompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, sizei size, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	glCompressedTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width, height, GetGLEnum(format), size, pixels);
}

void GLTexture2D::CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, PixelFormat internal_format, int x, int y, int width, int height)
{
	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexImage2D(GL_TEXTURE_2D, level, GetGLEnum(internal_format), x, y, width, height, 0);

	if(level == 0)
	{
		_format = internal_format;
		_width = width;
		_height = height;
	}
}

void GLTexture2D::CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int x, int y, int width, int height)
{
	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, x, y, width, height);
}

void GLTexture2D::InvalidateTexImage(int level)
{
	assert(_id);
	glInvalidateTexImage(_id, level);
}

void GLTexture2D::InvalidateTexSubImage(int level, int xoffset, int yoffset, int width, int height)
{
	assert(_id);
	glInvalidateTexSubImage(_id, level, xoffset, yoffset, 0, width, height, 1);
}

void GLTexture2D::ClearTexImage(int level, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexImage(_id, level, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTexture2D::ClearTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTexture2D::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTexImage(GL_TEXTURE_2D, level, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTexture2D::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetTexImage(level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTexture2D::GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const
{
	assert(_id);

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, GetGLEnum(format), GetGLEnum(type), bufferSize, pixels);
}

void GLTexture2D::GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);
	PACK_BUFFER_STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, GetGLEnum(format), GetGLEnum(type), bufferSize, BUFFER_OFFSET(buffer_offset));
}

void GLTexture2D::GetCompressedTexImage(int level, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	glGetCompressedTexImage(GL_TEXTURE_2D, level, pixels);
}

void GLTexture2D::GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetCompressedTexImage(level, BUFFER_OFFSET(buffer_offset));
}

void GLTexture2D::GetCompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, void* pixels, sizei bufferSize) const
{
	assert(_id);

	glGetCompressedTextureSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, bufferSize, pixels);
}

void GLTexture2D::GetCompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);

	PACK_BUFFER_STATE_MACHINE_HACK
	glGetCompressedTextureSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, bufferSize, BUFFER_OFFSET(buffer_offset));
}


//==================== Texture2DMultisample ====================

bool GLTexture2DMultisample::Create(GLState* gl_state)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex2DMultisample;
	_target = GL_TEXTURE_2D_MULTISAMPLE;
	_width = 0;
	_height = 0;

	STATE_MACHINE_HACK

	return true;
}

bool GLTexture2DMultisample::CreateImmutable(GLState* gl_state, int samples, PixelFormat internal_format, int width, int height, bool fixed_sample_locations)
{
	if(!Create(gl_state))
		return false;

	glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GetGLEnum(internal_format), width, height, fixed_sample_locations);

	_format = internal_format;
	_width = width;
	_height = height;

	return true;
}

bool GLTexture2DMultisample::CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	glTextureView(_id, GL_TEXTURE_2D_MULTISAMPLE, orig_tex, GetGLEnum(internal_format), min_level, num_levels, 0, 1);

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex2DMultisample;
	_target = GL_TEXTURE_2D_MULTISAMPLE;
	_format = internal_format;

	STATE_MACHINE_HACK

	glGetTexLevelParameteriv(GL_TEXTURE_2D_MULTISAMPLE, 0, GL_TEXTURE_WIDTH, &_width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D_MULTISAMPLE, 0, GL_TEXTURE_HEIGHT, &_height);

	return true;
}

void GLTexture2DMultisample::InvalidateTexImage()
{
	assert(_id);
	glInvalidateTexImage(_id, 0);
}

void GLTexture2DMultisample::InvalidateTexSubImage(int xoffset, int yoffset, int width, int height)
{
	assert(_id);
	glInvalidateTexSubImage(_id, 0, xoffset, yoffset, 0, width, height, 1);
}

void GLTexture2DMultisample::ClearTexImage(int level, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexImage(_id, level, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTexture2DMultisample::ClearTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, GetGLEnum(format), GetGLEnum(type), data);
}

//==================== Texture3D ====================

bool GLTexture3D::Create(GLState* gl_state)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex3D;
	_target = GL_TEXTURE_3D;
	_width = 0;
	_height = 0;
	_depth = 0;

	STATE_MACHINE_HACK

	return true;
}

bool GLTexture3D::CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width, int height, int depth)
{
	if(!Create(gl_state))
		return false;

	glTexStorage3D(GL_TEXTURE_3D, levels, GetGLEnum(internal_format), width, height, depth);

	_format = internal_format;
	_width = width;
	_height = height;
	_depth = depth;

	return true;
}

bool GLTexture3D::CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	glTextureView(_id, GL_TEXTURE_3D, orig_tex, GetGLEnum(internal_format), min_level, num_levels, 0, 1);

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex3D;
	_target = GL_TEXTURE_3D;
	_format = internal_format;

	STATE_MACHINE_HACK

	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &_width);
	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_HEIGHT, &_height);
	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_DEPTH, &_depth);

	return true;
}
	
void GLTexture3D::TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelUnpackState(_glState, pixel_store);
	glTexSubImage3D(GL_TEXTURE_3D, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTexture3D::TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset)
{
	UNPACK_BUFFER_STATE_MACHINE_HACK
	TexSubImage(level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTexture3D::CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, sizei size, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	glCompressedTexSubImage3D(GL_TEXTURE_3D, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), size, pixels);
}
	
void GLTexture3D::CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height)
{
	//assert(false && "do not call this function for now, no GL_EXT_copy_texture extension although glCopyTexSubImage3DEXT entry exists");

	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexSubImage3D(GL_TEXTURE_3D, level, xoffset, yoffset, zoffset, x, y, width, height);
}

void GLTexture3D::InvalidateTexImage(int level)
{
	assert(_id);
	glInvalidateTexImage(_id, level);
}

void GLTexture3D::InvalidateTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth)
{
	assert(_id);
	glInvalidateTexSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth);
}

void GLTexture3D::ClearTexImage(int level, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexImage(_id, level, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTexture3D::ClearTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTexture3D::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTexImage(GL_TEXTURE_3D, level, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTexture3D::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetTexImage(level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTexture3D::GetTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const
{
	assert(_id);

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), GetGLEnum(type), bufferSize, pixels);
}

void GLTexture3D::GetTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);
	PACK_BUFFER_STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), GetGLEnum(type), bufferSize, BUFFER_OFFSET(buffer_offset));
}

void GLTexture3D::GetCompressedTexImage(int level, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	glGetCompressedTexImage(GL_TEXTURE_3D, level, pixels);
}

void GLTexture3D::GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetCompressedTexImage(level, BUFFER_OFFSET(buffer_offset));
}

void GLTexture3D::GetCompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, void* pixels, sizei bufferSize) const
{
	assert(_id);

	glGetCompressedTextureSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth, bufferSize, pixels);
}

void GLTexture3D::GetCompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);

	PACK_BUFFER_STATE_MACHINE_HACK
	glGetCompressedTextureSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth, bufferSize, BUFFER_OFFSET(buffer_offset));
}


//==================== TextureCube ====================

bool GLTextureCube::Create(GLState* gl_state)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	_resType = ResourceType::Texture;
	_textureType = TextureType::TexCube;
	_target = GL_TEXTURE_CUBE_MAP;
	_width = 0;

	STATE_MACHINE_HACK

	return true;
}

bool GLTextureCube::CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width)
{
	if(!Create(gl_state))
		return false;

	glTexStorage2D(GL_TEXTURE_CUBE_MAP, levels, GetGLEnum(internal_format), width, width);

	_format = internal_format;
	_width = width;

	return true;
}

bool GLTextureCube::CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	glTextureView(_id, GL_TEXTURE_CUBE_MAP, orig_tex, GetGLEnum(internal_format), min_level, num_levels, min_layer, num_layers);

	_resType = ResourceType::Texture;
	_textureType = TextureType::TexCube;
	_target = GL_TEXTURE_CUBE_MAP;
	_format = internal_format;

	STATE_MACHINE_HACK

	glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP, 0, GL_TEXTURE_WIDTH, &_width);

	return true;
}

void GLTextureCube::TexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelUnpackState(_glState, pixel_store);
	glTexSubImage2D(GetGLEnum(face), level, xoffset, yoffset, width, height, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTextureCube::TexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset)
{
	UNPACK_BUFFER_STATE_MACHINE_HACK
	TexSubImage(face, level, xoffset, yoffset, width, height, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTextureCube::CompressedTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, sizei size, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	glCompressedTexSubImage2D(GetGLEnum(face), level, xoffset, yoffset, width, height, GetGLEnum(format), size, pixels);
}

void GLTextureCube::CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, CubeFace face, int level, PixelFormat internal_format, int x, int y, int width)
{
	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexImage2D(GetGLEnum(face), level, GetGLEnum(internal_format), x, y, width, width, 0);

	if(level == 0)
	{
		_format = internal_format;
		_width = width;
	}
}

void GLTextureCube::CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, CubeFace face, int level, int xoffset, int yoffset, int x, int y, int width, int height)
{
	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexSubImage2D(GetGLEnum(face), level, xoffset, yoffset, x, y, width, height);
}

void GLTextureCube::InvalidateTexImage(int level)
{
	assert(_id);
	glInvalidateTexImage(_id, level);
}

void GLTextureCube::InvalidateTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height)
{
	assert(_id);
	glInvalidateTexSubImage(_id, level, xoffset, yoffset, (GLint)face, width, height, 1); //! depth? maybe add num faces; also for clear below
}

void GLTextureCube::ClearTexImage(int level, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexImage(_id, level, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTextureCube::ClearTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexSubImage(_id, level, xoffset, yoffset, (GLint)face, width, height, 1, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTextureCube::GetTexImage(CubeFace face, int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTexImage(GetGLEnum(face), level, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTextureCube::GetTexImage(CubeFace face, int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetTexImage(face, level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTextureCube::GetTexSubImage(CubeFace face, int numFaces, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const
{
	assert(_id);

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, static_cast<GLint>(face), width, height, numFaces, GetGLEnum(format), GetGLEnum(type), bufferSize, pixels);
}

void GLTextureCube::GetTexSubImage(CubeFace face, int numFaces, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);
	PACK_BUFFER_STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, static_cast<GLint>(face), width, height, numFaces, GetGLEnum(format), GetGLEnum(type), bufferSize, BUFFER_OFFSET(buffer_offset));
}

void GLTextureCube::GetCompressedTexImage(CubeFace face, int level, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	glGetCompressedTexImage(GetGLEnum(face), level, pixels);
}

void GLTextureCube::GetCompressedTexImage(CubeFace face, int level, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetCompressedTexImage(face, level, BUFFER_OFFSET(buffer_offset));
}

void GLTextureCube::GetCompressedTexSubImage(CubeFace face, int numFaces, int level, int xoffset, int yoffset, int width, int height, void* pixels, sizei bufferSize) const
{
	assert(_id);

	glGetCompressedTextureSubImage(_id, level, xoffset, yoffset, static_cast<GLint>(face), width, height, numFaces, bufferSize, pixels);
}

void GLTextureCube::GetCompressedTexSubImage(CubeFace face, int numFaces, int level, int xoffset, int yoffset, int width, int height, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);

	PACK_BUFFER_STATE_MACHINE_HACK
	glGetCompressedTextureSubImage(_id, level, xoffset, yoffset, static_cast<GLint>(face), width, height, numFaces, bufferSize, BUFFER_OFFSET(buffer_offset));
}


//==================== Texture1DArray ====================

bool GLTexture1DArray::Create(GLState* gl_state)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex1DArray;
	_target = GL_TEXTURE_1D_ARRAY;
	_width = 0;
	_height = 0;

	STATE_MACHINE_HACK

	return true;
}

bool GLTexture1DArray::CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width, int height)
{
	if(!Create(gl_state))
		return false;

	glTexStorage2D(GL_TEXTURE_1D_ARRAY, levels, GetGLEnum(internal_format), width, height);

	_format = internal_format;
	_width = width;
	_height = height;

	return true;
}

bool GLTexture1DArray::CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	glTextureView(_id, GL_TEXTURE_1D_ARRAY, orig_tex, GetGLEnum(internal_format), min_level, num_levels, min_layer, num_layers);

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex1DArray;
	_target = GL_TEXTURE_1D_ARRAY;
	_format = internal_format;

	STATE_MACHINE_HACK

	glGetTexLevelParameteriv(GL_TEXTURE_1D_ARRAY, 0, GL_TEXTURE_WIDTH, &_width);
	glGetTexLevelParameteriv(GL_TEXTURE_1D_ARRAY, 0, GL_TEXTURE_HEIGHT, &_height);

	return true;
}

void GLTexture1DArray::TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelUnpackState(_glState, pixel_store);
	glTexSubImage2D(GL_TEXTURE_1D_ARRAY, level, xoffset, yoffset, width, height, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTexture1DArray::TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset)
{
	UNPACK_BUFFER_STATE_MACHINE_HACK
	TexSubImage(level, xoffset, yoffset, width, height, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTexture1DArray::CompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, sizei size, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	glCompressedTexSubImage2D(GL_TEXTURE_1D_ARRAY, level, xoffset, yoffset, width, height, GetGLEnum(format), size, pixels);
}

void GLTexture1DArray::CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, PixelFormat internal_format, int x, int y, int width, int height)
{
	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexImage2D(GL_TEXTURE_1D_ARRAY, level, GetGLEnum(internal_format), x, y, width, height, 0);

	if(level == 0)
	{
		_format = internal_format;
		_width = width;
		_height = height;
	}
}

void GLTexture1DArray::CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int x, int y, int width, int height)
{
	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexSubImage2D(GL_TEXTURE_1D_ARRAY, level, xoffset, yoffset, x, y, width, height);
}

void GLTexture1DArray::InvalidateTexImage(int level)
{
	assert(_id);
	glInvalidateTexImage(_id, level);
}

void GLTexture1DArray::InvalidateTexSubImage(int level, int xoffset, int yoffset, int width, int height)
{
	assert(_id);
	glInvalidateTexSubImage(_id, level, xoffset, yoffset, 0, width, height, 1);
}

void GLTexture1DArray::ClearTexImage(int level, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexImage(_id, level, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTexture1DArray::ClearTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTexture1DArray::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTexImage(GL_TEXTURE_1D_ARRAY, level, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTexture1DArray::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetTexImage(level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTexture1DArray::GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const
{
	assert(_id);

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, GetGLEnum(format), GetGLEnum(type), bufferSize, pixels);
}

void GLTexture1DArray::GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);
	PACK_BUFFER_STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, GetGLEnum(format), GetGLEnum(type), bufferSize, BUFFER_OFFSET(buffer_offset));
}

void GLTexture1DArray::GetCompressedTexImage(int level, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	glGetCompressedTexImage(GL_TEXTURE_1D_ARRAY, level, pixels);
}

void GLTexture1DArray::GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetCompressedTexImage(level, BUFFER_OFFSET(buffer_offset));
}

void GLTexture1DArray::GetCompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, void* pixels, sizei bufferSize) const
{
	assert(_id);

	glGetCompressedTextureSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, bufferSize, pixels);
}

void GLTexture1DArray::GetCompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);

	PACK_BUFFER_STATE_MACHINE_HACK
	glGetCompressedTextureSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, bufferSize, BUFFER_OFFSET(buffer_offset));
}


//==================== Texture2DArray ====================

bool GLTexture2DArray::Create(GLState* gl_state)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex2DArray;
	_target = GL_TEXTURE_2D_ARRAY;
	_width = 0;
	_height = 0;
	_depth = 0;

	STATE_MACHINE_HACK

	return true;
}

bool GLTexture2DArray::CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width, int height, int depth)
{
	if(!Create(gl_state))
		return false;

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, levels, GetGLEnum(internal_format), width, height, depth);

	_format = internal_format;
	_width = width;
	_height = height;
	_depth = depth;

	return true;
}

bool GLTexture2DArray::CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	glTextureView(_id, GL_TEXTURE_2D_ARRAY, orig_tex, GetGLEnum(internal_format), min_level, num_levels, min_layer, num_layers);

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex2DArray;
	_target = GL_TEXTURE_2D_ARRAY;
	_format = internal_format;

	STATE_MACHINE_HACK

	glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &_width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_HEIGHT, &_height);
	glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_HEIGHT, &_depth);

	return true;
}

void GLTexture2DArray::TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelUnpackState(_glState, pixel_store);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTexture2DArray::TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset)
{
	UNPACK_BUFFER_STATE_MACHINE_HACK
	TexSubImage(level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTexture2DArray::CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, sizei size, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), size, pixels);
}

void GLTexture2DArray::CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height)
{
	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, zoffset, x, y, width, height);
}

void GLTexture2DArray::InvalidateTexImage(int level)
{
	assert(_id);
	glInvalidateTexImage(_id, level);
}

void GLTexture2DArray::InvalidateTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth)
{
	assert(_id);
	glInvalidateTexSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth);
}

void GLTexture2DArray::ClearTexImage(int level, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexImage(_id, level, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTexture2DArray::ClearTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTexture2DArray::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTexImage(GL_TEXTURE_2D_ARRAY, level, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTexture2DArray::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetTexImage(level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTexture2DArray::GetTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const
{
	assert(_id);

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), GetGLEnum(type), bufferSize, pixels);
}

void GLTexture2DArray::GetTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);
	PACK_BUFFER_STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), GetGLEnum(type), bufferSize, BUFFER_OFFSET(buffer_offset));
}

void GLTexture2DArray::GetCompressedTexImage(int level, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	glGetCompressedTexImage(GL_TEXTURE_2D_ARRAY, level, pixels);
}

void GLTexture2DArray::GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetCompressedTexImage(level, BUFFER_OFFSET(buffer_offset));
}

void GLTexture2DArray::GetCompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, void* pixels, sizei bufferSize) const
{
	assert(_id);

	glGetCompressedTextureSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth, bufferSize, pixels);
}

void GLTexture2DArray::GetCompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);

	PACK_BUFFER_STATE_MACHINE_HACK
	glGetCompressedTextureSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth, bufferSize, BUFFER_OFFSET(buffer_offset));
}


//==================== Texture2DMultisampleArray ====================

bool GLTexture2DMultisampleArray::Create(GLState* gl_state)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex2DMultisampleArray;
	_target = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
	_width = 0;
	_height = 0;
	_depth = 0;

	STATE_MACHINE_HACK

	return true;
}

bool GLTexture2DMultisampleArray::CreateImmutable(GLState* gl_state, int samples, PixelFormat internal_format, int width, int height, int depth, bool fixed_sample_locations)
{
	if(!Create(gl_state))
		return false;

	glTexStorage3DMultisample(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, samples, GetGLEnum(internal_format), width, height, depth, fixed_sample_locations);

	_format = internal_format;
	_width = width;
	_height = height;
	_depth = depth;

	return true;
}

bool GLTexture2DMultisampleArray::CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	glTextureView(_id, GL_TEXTURE_2D_MULTISAMPLE_ARRAY, orig_tex, GetGLEnum(internal_format), min_level, num_levels, min_layer, num_layers);

	_resType = ResourceType::Texture;
	_textureType = TextureType::Tex2DMultisampleArray;
	_target = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
	_format = internal_format;

	STATE_MACHINE_HACK

	glGetTexLevelParameteriv(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, 0, GL_TEXTURE_WIDTH, &_width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, 0, GL_TEXTURE_HEIGHT, &_height);
	glGetTexLevelParameteriv(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, 0, GL_TEXTURE_HEIGHT, &_depth);

	return true;
}

void GLTexture2DMultisampleArray::InvalidateTexImage()
{
	assert(_id);
	glInvalidateTexImage(_id, 0);
}

void GLTexture2DMultisampleArray::InvalidateTexSubImage(int xoffset, int yoffset, int zoffset, int width, int height, int depth)
{
	assert(_id);
	glInvalidateTexSubImage(_id, 0, xoffset, yoffset, zoffset, width, height, depth);
}

void GLTexture2DMultisampleArray::ClearTexImage(int level, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexImage(_id, level, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTexture2DMultisampleArray::ClearTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), GetGLEnum(type), data);
}

//==================== TextureCubeArray ====================

bool GLTextureCubeArray::Create(GLState* gl_state)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	_resType = ResourceType::Texture;
	_textureType = TextureType::TexCubeArray;
	_target = GL_TEXTURE_CUBE_MAP_ARRAY;
	_width = 0;
	_depth = 0;

	STATE_MACHINE_HACK

	return true;
}

bool GLTextureCubeArray::CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width, int depth)
{
	if(!Create(gl_state))
		return false;

	glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, levels, GetGLEnum(internal_format), width, width, depth);

	_format = internal_format;
	_width = width;
	_depth = depth;

	return true;
}

bool GLTextureCubeArray::CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	glTextureView(_id, GL_TEXTURE_CUBE_MAP_ARRAY, orig_tex, GetGLEnum(internal_format), min_level, num_levels, min_layer, num_layers);

	_resType = ResourceType::Texture;
	_textureType = TextureType::TexCubeArray;
	_target = GL_TEXTURE_CUBE_MAP_ARRAY;
	_format = internal_format;

	STATE_MACHINE_HACK

	glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_TEXTURE_WIDTH, &_width);
	glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_TEXTURE_DEPTH, &_depth);

	return true;
}

void GLTextureCubeArray::TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelUnpackState(_glState, pixel_store);
	glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTextureCubeArray::TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset)
{
	UNPACK_BUFFER_STATE_MACHINE_HACK
	TexSubImage(level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTextureCubeArray::CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, sizei size, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	glCompressedTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), size, pixels);
}

void GLTextureCubeArray::CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height)
{
	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, level, xoffset, yoffset, zoffset, x, y, width, height);
}

void GLTextureCubeArray::InvalidateTexImage(int level)
{
	assert(_id);
	glInvalidateTexImage(_id, level);
}

void GLTextureCubeArray::InvalidateTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth)
{
	assert(_id);
	glInvalidateTexSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth);
}

void GLTextureCubeArray::ClearTexImage(int level, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexImage(_id, level, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTextureCubeArray::ClearTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexSubImage(_id, level, xoffset, yoffset, zoffset, width, height, depth, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTextureCubeArray::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTexImage(GL_TEXTURE_CUBE_MAP_ARRAY, level, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTextureCubeArray::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetTexImage(level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTextureCubeArray::GetTexSubImage(int layerFace, int numLayerFaces, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const
{
	assert(_id);

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, layerFace, width, height, numLayerFaces, GetGLEnum(format), GetGLEnum(type), bufferSize, pixels);
}

void GLTextureCubeArray::GetTexSubImage(int layerFace, int numLayerFaces, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);
	PACK_BUFFER_STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, layerFace, width, height, numLayerFaces, GetGLEnum(format), GetGLEnum(type), bufferSize, BUFFER_OFFSET(buffer_offset));
}


//==================== TextureBuffer ====================

bool GLTextureBuffer::Create(GLState* gl_state)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	_resType = ResourceType::Texture;
	_textureType = TextureType::TexBuffer;
	_target = GL_TEXTURE_BUFFER;
	_size = 0;

	STATE_MACHINE_HACK

	return true;
}

void GLTextureBuffer::TexBuffer(PixelFormat internal_format, IBuffer* buffer)
{
	assert(_id);
	STATE_MACHINE_HACK

	glTexBuffer(GL_TEXTURE_BUFFER, GetGLEnum(internal_format), dyn_cast_ptr<GLResource*>(buffer)->GetID());

	_format = internal_format;
	_size = dyn_cast_ptr<GLBuffer*>(buffer)->GetSize();
}

void GLTextureBuffer::TexBufferRange(PixelFormat internal_format, IBuffer* buffer, intptr offset, sizeiptr size)
{
	assert(_id);
	STATE_MACHINE_HACK

	glTexBufferRange(GL_TEXTURE_BUFFER, GetGLEnum(internal_format), dyn_cast_ptr<GLResource*>(buffer)->GetID(), offset, size);

	_format = internal_format;
	_size = dyn_cast_ptr<GLBuffer*>(buffer)->GetSize();
}

void GLTextureBuffer::InvalidateTexImage()
{
	assert(_id);
	glInvalidateTexImage(_id, 0);
}

void GLTextureBuffer::InvalidateTexSubImage(int xoffset, int yoffset, int width, int height)
{
	assert(_id);
	glInvalidateTexSubImage(_id, 0, xoffset, yoffset, 0, width, height, 1);
}

//==================== TextureRectangle ====================
// - Repeat AND MirroredRepeat not alowed for texture wrap mode
// - minification filter must be Nearest or Linear
// - compressed formats not allowed
// - mipmaps not allowed
// - texture base level must be 0
bool GLTextureRectangle::Create(GLState* gl_state)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	_resType = ResourceType::Texture;
	_textureType = TextureType::TexRectangle;
	_target = GL_TEXTURE_RECTANGLE;
	_width = 0;
	_height = 0;

	STATE_MACHINE_HACK

	return true;
}

bool GLTextureRectangle::CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width, int height)
{
	if(!Create(gl_state))
		return false;

	glTexStorage2D(GL_TEXTURE_RECTANGLE, levels, GetGLEnum(internal_format), width, height);

	_format = internal_format;
	_width = width;
	_height = height;

	return true;
}

bool GLTextureRectangle::CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	if(_id)
		return false;

	_glState = gl_state;

	glGenTextures(1, &_id);
	if(!_id)
		return false;

	glTextureView(_id, GL_TEXTURE_RECTANGLE, orig_tex, GetGLEnum(internal_format), min_level, num_levels, 0, 1);

	_resType = ResourceType::Texture;
	_textureType = TextureType::TexRectangle;
	_target = GL_TEXTURE_RECTANGLE;
	_format = internal_format;

	STATE_MACHINE_HACK

	glGetTexLevelParameteriv(GL_TEXTURE_RECTANGLE, 0, GL_TEXTURE_WIDTH, &_width);
	glGetTexLevelParameteriv(GL_TEXTURE_RECTANGLE, 0, GL_TEXTURE_HEIGHT, &_height);

	return true;
}

void GLTextureRectangle::TexSubImage(int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels)
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelUnpackState(_glState, pixel_store);
	glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, xoffset, yoffset, width, height, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTextureRectangle::TexSubImage(int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset)
{
	UNPACK_BUFFER_STATE_MACHINE_HACK
	TexSubImage(xoffset, yoffset, width, height, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTextureRectangle::CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, PixelFormat internal_format, int x, int y, int width, int height)
{
	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexImage2D(GL_TEXTURE_RECTANGLE, 0, GetGLEnum(internal_format), x, y, width, height, 0);

	_format = internal_format;
	_width = width;
	_height = height;
}

void GLTextureRectangle::CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int xoffset, int yoffset, int x, int y, int width, int height)
{
	assert(_id);
	FRAMEBUFFER_STATE_MACHINE_HACK
	STATE_MACHINE_HACK

	glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, xoffset, yoffset, x, y, width, height);
}

void GLTextureRectangle::InvalidateTexImage()
{
	assert(_id);
	glInvalidateTexImage(_id, 0);
}

void GLTextureRectangle::InvalidateTexSubImage(int xoffset, int yoffset, int width, int height)
{
	assert(_id);
	glInvalidateTexSubImage(_id, 0, xoffset, yoffset, 0, width, height, 1);
}

void GLTextureRectangle::ClearTexImage(int level, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexImage(_id, level, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTextureRectangle::ClearTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data)
{
	assert(_id);
	glClearTexSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, GetGLEnum(format), GetGLEnum(type), data);
}

void GLTextureRectangle::GetTexImage(ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
{
	assert(_id);
	STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTexImage(GL_TEXTURE_RECTANGLE, 0, GetGLEnum(format), GetGLEnum(type), pixels);
}

void GLTextureRectangle::GetTexImage(ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const
{
	PACK_BUFFER_STATE_MACHINE_HACK
	GetTexImage(format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
}

void GLTextureRectangle::GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const
{
	assert(_id);

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, GetGLEnum(format), GetGLEnum(type), bufferSize, pixels);
}

void GLTextureRectangle::GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const
{
	assert(_id);
	PACK_BUFFER_STATE_MACHINE_HACK

	__SetPixelPackState(_glState, pixel_store);
	glGetTextureSubImage(_id, level, xoffset, yoffset, 0, width, height, 1, GetGLEnum(format), GetGLEnum(type), bufferSize, BUFFER_OFFSET(buffer_offset));
}

} // namespace gls::internals

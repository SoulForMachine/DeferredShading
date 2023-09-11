
#ifndef _GL_TEXTURE_H_
#define _GL_TEXTURE_H_

#include "GLSlayer/Texture.h"
#include "GLResource.h"


namespace gls::internals
{

bool IsCompressedFormat(PixelFormat format);


#define IMPLEMENT_ITEXTURE \
	virtual TextureType GetTextureType() override { return GLTexture::GetTextureType(); } \
	virtual void GenerateMipmap() override { GLTexture::GenerateMipmap(); } \
	virtual PixelFormat GetFormat() const override { return GLTexture::GetFormat(); } \
	virtual void SetBaseLevel(int base_level) override { GLTexture::SetBaseLevel(base_level); } \
	virtual int GetBaseLevel() const override { return GLTexture::GetBaseLevel(); } \
	virtual void SetMaxLevel(int max_level) override { GLTexture::SetMaxLevel(max_level); } \
	virtual int GetMaxLevel() const override { return GLTexture::GetMaxLevel(); } \
	virtual bool IsCompressed() const override { return GLTexture::IsCompressed(); } \
	virtual int GetCompressedSize(int level) const override { return GLTexture::GetCompressedSize(level); } \
	virtual void ComponentSwizzle(TexSwizzleDest dest, TexSwizzleSource source) override { GLTexture::ComponentSwizzle(dest, source); } \
	virtual void ComponentSwizzle(TexSwizzleSource source_red, TexSwizzleSource source_green, TexSwizzleSource source_blue, TexSwizzleSource source_alpha) override \
		{ GLTexture::ComponentSwizzle(source_red, source_green, source_blue, source_alpha); } \
	virtual void DepthStencilMode(DepthStencilTexMode mode) override { GLTexture::DepthStencilMode(mode); }


class GLTexture : public GLResource
{
public:
	static const int typeID = TYPE_ID_TEXTURE;

	GLTexture() = default;
	virtual ~GLTexture() = 0;

	GLTexture(const GLTexture&) = delete;
	GLTexture& operator = (const GLTexture&) = delete;

	void Destroy();

	void* DynamicCast(int type_id)	{ return (type_id == TYPE_ID_TEXTURE) ? this : GLResource::DynamicCast(type_id); }
	TextureType GetTextureType();
	void GenerateMipmap();
	PixelFormat GetFormat() const		{ return _format; }
	void SetBaseLevel(int base_level);
	int GetBaseLevel() const		{ return _baseLevel; }
	void SetMaxLevel(int max_level);
	int GetMaxLevel() const			{ return _maxLevel; }
	bool IsCompressed() const		{ return IsCompressedFormat(_format); }
	int GetCompressedSize(int level) const;
	void ComponentSwizzle(TexSwizzleDest dest, TexSwizzleSource source);
	void ComponentSwizzle(TexSwizzleSource source_red, TexSwizzleSource source_green, TexSwizzleSource source_blue, TexSwizzleSource source_alpha);
	void DepthStencilMode(DepthStencilTexMode mode);

protected:
	GLState* _glState;
	PixelFormat _format;	// that's internal format
	int _baseLevel = 0;
	int _maxLevel = 1000;
	TextureType _textureType;
};

inline GLTexture::~GLTexture()
{
}


class GLTexture1D : public ITexture1D, public GLTexture
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ITEXTURE

	static const int typeID = TYPE_ID_TEXTURE_1D;

	GLTexture1D() = default;

	GLTexture1D(const GLTexture1D&) = delete;
	GLTexture1D& operator = (const GLTexture1D&) = delete;

	bool CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width);
	bool CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer);

	virtual void* DynamicCast(int type_id) override	{ return (type_id == TYPE_ID_TEXTURE_1D) ? this : GLTexture::DynamicCast(type_id); }
	virtual void TexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) override;
	virtual void TexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) override;
	virtual void CompressedTexSubImage(int level, int xoffset, int width, ImageFormat format, sizei size, const void* pixels) override;
	virtual void CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, PixelFormat internal_format, int x, int y, int width) override;
	virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int x, int y, int width) override;
	virtual void InvalidateTexImage(int level) override;
	virtual void InvalidateTexSubImage(int level, int xoffset, int width) override;
	virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) override;
	virtual void ClearTexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const void* data) override;
	virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const override;
	virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetTexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const override;
	virtual void GetTexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;
	virtual void GetCompressedTexImage(int level, void* pixels) const override;
	virtual void GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetCompressedTexSubImage(int level, int xoffset, int width, void* pixels, sizei bufferSize) const override;
	virtual void GetCompressedTexSubImage(int level, int xoffset, int width, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;

	virtual int GetWidth() const override
		{ return _width; }

private:
	bool Create(GLState* gl_state);

	int _width;
};

class GLTexture2D : public ITexture2D, public GLTexture
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ITEXTURE

	static const int typeID = TYPE_ID_TEXTURE_2D;

	GLTexture2D() = default;

	GLTexture2D(const GLTexture2D&) = delete;
	GLTexture2D& operator = (const GLTexture2D&) = delete;

	bool CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width, int height);
	bool CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer);

	virtual void* DynamicCast(int type_id) override	{ return (type_id == TYPE_ID_TEXTURE_2D) ? this : GLTexture::DynamicCast(type_id); }
	virtual void TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) override;
	virtual void TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) override;
	virtual void CompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, sizei size, const void* pixels) override;
	virtual void CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, PixelFormat internal_format, int x, int y, int width, int height) override;
	virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int x, int y, int width, int height) override;
	virtual void InvalidateTexImage(int level) override;
	virtual void InvalidateTexSubImage(int level, int xoffset, int yoffset, int width, int height) override;
	virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) override;
	virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data) override;
	virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const override;
	virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const override;
	virtual void GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;
	virtual void GetCompressedTexImage(int level, void* pixels) const override;
	virtual void GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, void* pixels, sizei bufferSize) const override;
	virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;

	virtual int GetWidth() const override
		{ return _width; }
	virtual int GetHeight() const override
		{ return _height; }

private:
	bool Create(GLState* gl_state);

	int _width;
	int _height;
};

class GLTexture2DMultisample : public ITexture2DMultisample, public GLTexture
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ITEXTURE

	static const int typeID = TYPE_ID_TEXTURE_2D_MULTISAMPLE;

	GLTexture2DMultisample() = default;

	GLTexture2DMultisample(const GLTexture2DMultisample&) = delete;
	GLTexture2DMultisample& operator = (const GLTexture2DMultisample&) = delete;

	bool CreateImmutable(GLState* gl_state, int samples, PixelFormat internal_format, int width, int height, bool fixed_sample_locations);
	bool CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels);

	virtual void* DynamicCast(int type_id) override	{ return (type_id == TYPE_ID_TEXTURE_2D_MULTISAMPLE) ? this : GLTexture::DynamicCast(type_id); }
	virtual void InvalidateTexImage() override;
	virtual void InvalidateTexSubImage(int xoffset, int yoffset, int width, int height) override;
	virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) override;
	virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data) override;


	virtual int GetWidth() const override
		{ return _width; }
	virtual int GetHeight() const override
		{ return _height; }

private:
	bool Create(GLState* gl_state);

	int _width;
	int _height;
};

class GLTexture3D : public ITexture3D, public GLTexture
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ITEXTURE

	static const int typeID = TYPE_ID_TEXTURE_3D;

	GLTexture3D() = default;

	GLTexture3D(const GLTexture3D&) = delete;
	GLTexture3D& operator = (const GLTexture3D&) = delete;

	bool CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width, int height, int depth);
	bool CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels);

	virtual void* DynamicCast(int type_id) override	{ return (type_id == TYPE_ID_TEXTURE_3D) ? this : GLTexture::DynamicCast(type_id); }
	virtual void TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) override;
	virtual void TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) override;
	virtual void CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, sizei size, const void* pixels) override;
	virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height) override;
	virtual void InvalidateTexImage(int level) override;
	virtual void InvalidateTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth) override;
	virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) override;
	virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const void* data) override;
	virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const override;
	virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const override;
	virtual void GetTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;
	virtual void GetCompressedTexImage(int level, void* pixels) const override;
	virtual void GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, void* pixels, sizei bufferSize) const override;
	virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;

	virtual int GetWidth() const override
		{ return _width; }
	virtual int GetHeight() const override
		{ return _height; }
	virtual int GetDepth() const override
		{ return _depth; }

private:
	bool Create(GLState* gl_state);

	int _width;
	int _height;
	int _depth;
};

class GLTextureCube : public ITextureCube, public GLTexture
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ITEXTURE

	static const int typeID = TYPE_ID_TEXTURE_CUBE;

	GLTextureCube() = default;

	GLTextureCube(const GLTextureCube&) = delete;
	GLTextureCube& operator = (const GLTextureCube&) = delete;

	bool CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width);
	bool CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers);

	virtual void* DynamicCast(int type_id) override	{ return (type_id == TYPE_ID_TEXTURE_CUBE) ? this : GLTexture::DynamicCast(type_id); }
	virtual void TexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) override;
	virtual void TexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) override;
	virtual void CompressedTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, sizei size, const void* pixels) override;
	virtual void CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, CubeFace face, int level, PixelFormat internal_format, int x, int y, int width) override;
	virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, CubeFace face, int level, int xoffset, int yoffset, int x, int y, int width, int height) override;
	virtual void InvalidateTexImage(int level) override;
	virtual void InvalidateTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height) override;
	virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) override;
	virtual void ClearTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data) override;
	virtual void GetTexImage(CubeFace face, int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const override;
	virtual void GetTexImage(CubeFace face, int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetTexSubImage(CubeFace face, int numFaces, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const override;
	virtual void GetTexSubImage(CubeFace face, int numFaces, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;
	virtual void GetCompressedTexImage(CubeFace face, int level, void* pixels) const override;
	virtual void GetCompressedTexImage(CubeFace face, int level, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetCompressedTexSubImage(CubeFace face, int numFaces, int level, int xoffset, int yoffset, int width, int height, void* pixels, sizei bufferSize) const override;
	virtual void GetCompressedTexSubImage(CubeFace face, int numFaces, int level, int xoffset, int yoffset, int width, int height, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;

	virtual int GetWidth() const override
		{ return _width; }

private:
	bool Create(GLState* gl_state);

	int _width;
};

class GLTexture1DArray : public ITexture1DArray, public GLTexture
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ITEXTURE

	static const int typeID = TYPE_ID_TEXTURE_1D_ARRAY;

	GLTexture1DArray() = default;

	GLTexture1DArray(const GLTexture1DArray&) = delete;
	GLTexture1DArray& operator = (const GLTexture1DArray&) = delete;

	bool CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width, int height);
	bool CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers);

	virtual void* DynamicCast(int type_id) override	{ return (type_id == TYPE_ID_TEXTURE_1D_ARRAY) ? this : GLTexture::DynamicCast(type_id); }
	virtual void TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) override;
	virtual void TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) override;
	virtual void CompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, sizei size, const void* pixels) override;
	virtual void CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, PixelFormat internal_format, int x, int y, int width, int height) override;
	virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int x, int y, int width, int height) override;
	virtual void InvalidateTexImage(int level) override;
	virtual void InvalidateTexSubImage(int level, int xoffset, int yoffset, int width, int height) override;
	virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) override;
	virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data) override;
	virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const override;
	virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const override;
	virtual void GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;
	virtual void GetCompressedTexImage(int level, void* pixels) const override;
	virtual void GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, void* pixels, sizei bufferSize) const override;
	virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;

	virtual int GetWidth() const override
		{ return _width; }
	virtual int GetHeight() const override
		{ return _height; }

private:
	bool Create(GLState* gl_state);

	int _width;
	int _height;
};

class GLTexture2DArray : public ITexture2DArray, public GLTexture
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ITEXTURE

	static const int typeID = TYPE_ID_TEXTURE_2D_ARRAY;

	GLTexture2DArray() = default;

	GLTexture2DArray(const GLTexture2DArray&) = delete;
	GLTexture2DArray& operator = (const GLTexture2DArray&) = delete;

	bool CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width, int height, int depth);
	bool CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers);

	virtual void* DynamicCast(int type_id) override	{ return (type_id == TYPE_ID_TEXTURE_2D_ARRAY) ? this : GLTexture::DynamicCast(type_id); }
	virtual void TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) override;
	virtual void TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) override;
	virtual void CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, sizei size, const void* pixels) override;
	virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height) override;
	virtual void InvalidateTexImage(int level) override;
	virtual void InvalidateTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth) override;
	virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) override;
	virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const void* data) override;
	virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const override;
	virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const override;
	virtual void GetTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;
	virtual void GetCompressedTexImage(int level, void* pixels) const override;
	virtual void GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, void* pixels, sizei bufferSize) const override;
	virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;

	virtual int GetWidth() const override
		{ return _width; }
	virtual int GetHeight() const override
		{ return _height; }
	virtual int GetDepth() const override
		{ return _depth; }

private:
	bool Create(GLState* gl_state);

	int _width;
	int _height;
	int _depth;
};

class GLTexture2DMultisampleArray : public ITexture2DMultisampleArray, public GLTexture
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ITEXTURE

	static const int typeID = TYPE_ID_TEXTURE_2D_MULTISAMPLE_ARRAY;

	GLTexture2DMultisampleArray() = default;

	GLTexture2DMultisampleArray(const GLTexture2DMultisampleArray&) = delete;
	GLTexture2DMultisampleArray& operator = (const GLTexture2DMultisampleArray&) = delete;

	bool CreateImmutable(GLState* gl_state, int samples, PixelFormat internal_format, int width, int height, int depth, bool fixed_sample_locations);
	bool CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers);

	virtual void* DynamicCast(int type_id) override	{ return (type_id == TYPE_ID_TEXTURE_2D_MULTISAMPLE_ARRAY) ? this : GLTexture::DynamicCast(type_id); }
	virtual void InvalidateTexImage() override;
	virtual void InvalidateTexSubImage(int xoffset, int yoffset, int zoffset, int width, int height, int depth) override;
	virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) override;
	virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const void* data) override;

	virtual int GetWidth() const override
		{ return _width; }
	virtual int GetHeight() const override
		{ return _height; }
	virtual int GetDepth() const override
		{ return _depth; }

private:
	bool Create(GLState* gl_state);

	int _width;
	int _height;
	int _depth;
};

class GLTextureCubeArray : public ITextureCubeArray, public GLTexture
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ITEXTURE

	static const int typeID = TYPE_ID_TEXTURE_CUBE_ARRAY;

	GLTextureCubeArray() = default;

	GLTextureCubeArray(const GLTextureCubeArray&) = delete;
	GLTextureCubeArray& operator = (const GLTextureCubeArray&) = delete;

	bool CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width, int depth);
	bool CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers);

	virtual void* DynamicCast(int type_id) override	{ return (type_id == TYPE_ID_TEXTURE_CUBE_ARRAY) ? this : GLTexture::DynamicCast(type_id); }
	virtual void TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) override;
	virtual void TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) override;
	virtual void CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, sizei size, const void* pixels) override;
	virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height) override;
	virtual void InvalidateTexImage(int level) override;
	virtual void InvalidateTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth) override;
	virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) override;
	virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const void* data) override;
	virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const override;
	virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetTexSubImage(int layerFace, int numLayerFaces, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const override;
	virtual void GetTexSubImage(int layerFace, int numLayerFaces, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;

	virtual int GetWidth() const override
		{ return _width; }
	virtual int GetDepth() const override
		{ return _depth; }

private:
	bool Create(GLState* gl_state);

	int _width;
	int _depth;
};

class GLTextureBuffer : public ITextureBuffer, public GLTexture
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ITEXTURE

	static const int typeID = TYPE_ID_TEXTURE_BUFFER;

	GLTextureBuffer() = default;

	GLTextureBuffer(const GLTextureBuffer&) = delete;
	GLTextureBuffer& operator = (const GLTextureBuffer&) = delete;

	bool Create(GLState* gl_state);

	virtual void* DynamicCast(int type_id) override	{ return (type_id == TYPE_ID_TEXTURE_BUFFER) ? this : GLTexture::DynamicCast(type_id); }
	virtual void TexBuffer(PixelFormat internal_format, IBuffer* buffer) override;
	virtual void TexBufferRange(PixelFormat internal_format, IBuffer* buffer, intptr offset, sizeiptr size) override;
	virtual void InvalidateTexImage() override;
	virtual void InvalidateTexSubImage(int xoffset, int yoffset, int width, int height) override;

	virtual sizeiptr GetSize() const override
		{ return _size; }

private:
	// these are not applicable to buffer textures
	using GLTexture::GenerateMipmap;
	using GLTexture::IsCompressed;

	sizeiptr _size;
};

class GLTextureRectangle : public ITextureRectangle, public GLTexture
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ITEXTURE

	static const int typeID = TYPE_ID_TEXTURE_RECTANGLE;

	GLTextureRectangle() = default;

	GLTextureRectangle(const GLTextureRectangle&) = delete;
	GLTextureRectangle& operator = (const GLTextureRectangle&) = delete;

	bool CreateImmutable(GLState* gl_state, sizei levels, PixelFormat internal_format, int width, int height);
	bool CreateView(GLState* gl_state, GLuint orig_tex, PixelFormat internal_format, uint min_level, uint num_levels);

	virtual void* DynamicCast(int type_id) override	{ return (type_id == TYPE_ID_TEXTURE_RECTANGLE) ? this : GLTexture::DynamicCast(type_id); }
	virtual void TexSubImage(int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) override;
	virtual void TexSubImage(int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) override;
	virtual void CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, PixelFormat internal_format, int x, int y, int width, int height) override;
	virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int xoffset, int yoffset, int x, int y, int width, int height) override;
	virtual void InvalidateTexImage() override;
	virtual void InvalidateTexSubImage(int xoffset, int yoffset, int width, int height) override;
	virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) override;
	virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data) override;
	virtual void GetTexImage(ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const override;
	virtual void GetTexImage(ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const override;
	virtual void GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const override;
	virtual void GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const override;

	virtual int GetWidth() const override
		{ return _width; }
	virtual int GetHeight() const override
		{ return _height; }

private:
	// these are not applicable to rectangle textures
	using GLTexture::GenerateMipmap;
	using GLTexture::IsCompressed;

	bool Create(GLState* gl_state);

	int _width;
	int _height;
};

} // namespace gls::internals

#endif //_GL_TEXTURE_H_

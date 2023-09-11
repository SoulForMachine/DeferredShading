
#ifndef _GLSLAYER_TEXTURE_H_
#define _GLSLAYER_TEXTURE_H_

#include "Resource.h"


namespace gls
{

	class IBuffer;
	class ISamplerState;
	class IFramebuffer;


	class ITexture: public IResource
	{
	public:
		virtual TextureType GetTextureType() = 0;
		virtual void GenerateMipmap() = 0;
		virtual PixelFormat GetFormat() const = 0;
		virtual void SetBaseLevel(int base_level) = 0;
		virtual int GetBaseLevel() const = 0;
		virtual void SetMaxLevel(int max_level) = 0;
		virtual int GetMaxLevel() const = 0;
		virtual bool IsCompressed() const = 0;
		virtual int GetCompressedSize(int level) const = 0;
		virtual void ComponentSwizzle(TexSwizzleDest dest, TexSwizzleSource source) = 0;
		virtual void ComponentSwizzle(TexSwizzleSource source_red, TexSwizzleSource source_green, TexSwizzleSource source_blue, TexSwizzleSource source_alpha) = 0;
		virtual void DepthStencilMode(DepthStencilTexMode mode) = 0;
	};

	class ITexture1D: public ITexture
	{
	public:
		virtual void TexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) = 0;
		virtual void TexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) = 0;
		virtual void CompressedTexSubImage(int level, int xoffset, int width, ImageFormat format, sizei size, const void* pixels) = 0;
		virtual void CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, PixelFormat internal_format, int x, int y, int width) = 0;
		virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int x, int y, int width) = 0;
		virtual void InvalidateTexImage(int level) = 0;
		virtual void InvalidateTexSubImage(int level, int xoffset, int width) = 0;
		virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) = 0;
		virtual void ClearTexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const void* data) = 0;
		virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const = 0;
		virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetTexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const = 0;
		virtual void GetTexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;
		virtual void GetCompressedTexImage(int level, void* pixels) const = 0;
		virtual void GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetCompressedTexSubImage(int level, int xoffset, int width, void* pixels, sizei bufferSize) const = 0;
		virtual void GetCompressedTexSubImage(int level, int xoffset, int width, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;

		virtual int GetWidth() const = 0;
	};

	class ITexture2D: public ITexture
	{
	public:
		virtual void TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) = 0;
		virtual void TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) = 0;
		virtual void CompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, sizei size, const void* pixels) = 0;
		virtual void CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, PixelFormat internal_format, int x, int y, int width, int height) = 0;
		virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int x, int y, int width, int height) = 0;
		virtual void InvalidateTexImage(int level) = 0;
		virtual void InvalidateTexSubImage(int level, int xoffset, int yoffset, int width, int height) = 0;
		virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) = 0;
		virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data) = 0;
		virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const = 0;
		virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const = 0;
		virtual void GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;
		virtual void GetCompressedTexImage(int level, void* pixels) const = 0;
		virtual void GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, void* pixels, sizei bufferSize) const = 0;
		virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
	};

	class ITexture2DMultisample : public ITexture
	{
	public:
		virtual void InvalidateTexImage() = 0;
		virtual void InvalidateTexSubImage(int xoffset, int yoffset, int width, int height) = 0;
		virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) = 0;
		virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data) = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
	};

	class ITexture3D: public ITexture
	{
	public:
		virtual void TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) = 0;
		virtual void TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) = 0;
		virtual void CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, sizei size, const void* pixels) = 0;
		virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height) = 0;
		virtual void InvalidateTexImage(int level) = 0;
		virtual void InvalidateTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth) = 0;
		virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) = 0;
		virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const void* data) = 0;
		virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const = 0;
		virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const = 0;
		virtual void GetTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;
		virtual void GetCompressedTexImage(int level, void* pixels) const = 0;
		virtual void GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, void* pixels, sizei bufferSize) const = 0;
		virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual int GetDepth() const = 0;
	};

	class ITextureCube: public ITexture
	{
	public:
		virtual void TexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) = 0;
		virtual void TexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) = 0;
		virtual void CompressedTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, sizei size, const void* pixels) = 0;
		virtual void CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, CubeFace face, int level, PixelFormat internal_format, int x, int y, int width) = 0;
		virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, CubeFace face, int level, int xoffset, int yoffset, int x, int y, int width, int height) = 0;
		virtual void InvalidateTexImage(int level) = 0;
		virtual void InvalidateTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height) = 0;
		virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) = 0;
		virtual void ClearTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data) = 0;
		virtual void GetTexImage(CubeFace face, int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const = 0;
		virtual void GetTexImage(CubeFace face, int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetTexSubImage(CubeFace face, int numFaces, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const = 0;
		virtual void GetTexSubImage(CubeFace face, int numFaces, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;
		virtual void GetCompressedTexImage(CubeFace face, int level, void* pixels) const = 0;
		virtual void GetCompressedTexImage(CubeFace face, int level, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetCompressedTexSubImage(CubeFace face, int numFaces, int level, int xoffset, int yoffset, int width, int height, void* pixels, sizei bufferSize) const = 0;
		virtual void GetCompressedTexSubImage(CubeFace face, int numFaces, int level, int xoffset, int yoffset, int width, int height, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;

		virtual int GetWidth() const = 0;
	};

	class ITexture1DArray: public ITexture
	{
	public:
		virtual void TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) = 0;
		virtual void TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) = 0;
		virtual void CompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, sizei size, const void* pixels) = 0;
		virtual void CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, PixelFormat internal_format, int x, int y, int width, int height) = 0;
		virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int x, int y, int width, int height) = 0;
		virtual void InvalidateTexImage(int level) = 0;
		virtual void InvalidateTexSubImage(int level, int xoffset, int yoffset, int width, int height) = 0;
		virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) = 0;
		virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data) = 0;
		virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const = 0;
		virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const = 0;
		virtual void GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;
		virtual void GetCompressedTexImage(int level, void* pixels) const = 0;
		virtual void GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, void* pixels, sizei bufferSize) const = 0;
		virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
	};

	class ITexture2DArray: public ITexture
	{
	public:
		virtual void TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) = 0;
		virtual void TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) = 0;
		virtual void CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, sizei size, const void* pixels) = 0;
		virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height) = 0;
		virtual void InvalidateTexImage(int level) = 0;
		virtual void InvalidateTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth) = 0;
		virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) = 0;
		virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const void* data) = 0;
		virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const = 0;
		virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const = 0;
		virtual void GetTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;
		virtual void GetCompressedTexImage(int level, void* pixels) const = 0;
		virtual void GetCompressedTexImage(int level, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, void* pixels, sizei bufferSize) const = 0;
		virtual void GetCompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual int GetDepth() const = 0;
	};

	class ITexture2DMultisampleArray : public ITexture
	{
	public:
		virtual void InvalidateTexImage() = 0;
		virtual void InvalidateTexSubImage(int xoffset, int yoffset, int zoffset, int width, int height, int depth) = 0;
		virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) = 0;
		virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const void* data) = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual int GetDepth() const = 0;
	};

	class ITextureCubeArray : public ITexture
	{
	public:
		virtual void TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) = 0;
		virtual void TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) = 0;
		virtual void CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, sizei size, const void* pixels) = 0;
		virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height) = 0;
		virtual void InvalidateTexImage(int level) = 0;
		virtual void InvalidateTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth) = 0;
		virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) = 0;
		virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const void* data) = 0;
		virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const = 0;
		virtual void GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetTexSubImage(int layerFace, int numLayerFaces, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const = 0;
		virtual void GetTexSubImage(int layerFace, int numLayerFaces, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;

		virtual int GetWidth() const = 0;
		virtual int GetDepth() const = 0;
	};

	class ITextureBuffer: public ITexture
	{
	public:
		virtual void TexBuffer(PixelFormat internal_format, IBuffer* buffer) = 0;
		virtual void TexBufferRange(PixelFormat internal_format, IBuffer* buffer, intptr offset, sizeiptr size) = 0;
		virtual void InvalidateTexImage() = 0;
		virtual void InvalidateTexSubImage(int xoffset, int yoffset, int width, int height) = 0;

		virtual sizeiptr GetSize() const = 0;

	private:
		// these are not applicable to buffer textures
		using ITexture::GenerateMipmap;
		using ITexture::IsCompressed;
	};

	class ITextureRectangle: public ITexture
	{
	public:
		virtual void TexSubImage(int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const void* pixels) = 0;
		virtual void TexSubImage(int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) = 0;
		virtual void CopyTexImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, PixelFormat internal_format, int x, int y, int width, int height) = 0;
		virtual void CopyTexSubImage(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int xoffset, int yoffset, int x, int y, int width, int height) = 0;
		virtual void InvalidateTexImage() = 0;
		virtual void InvalidateTexSubImage(int xoffset, int yoffset, int width, int height) = 0;
		virtual void ClearTexImage(int level, ImageFormat format, DataType type, const void* data) = 0;
		virtual void ClearTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const void* data) = 0;
		virtual void GetTexImage(ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const = 0;
		virtual void GetTexImage(ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) const = 0;
		virtual void GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels, sizei bufferSize) const = 0;
		virtual void GetTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset, sizei bufferSize) const = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

	private:
		// these are not applicable to buffer textures
		using ITexture::GenerateMipmap;
		using ITexture::IsCompressed;
	};

}

#endif //_GLSLAYER_TEXTURE_H_

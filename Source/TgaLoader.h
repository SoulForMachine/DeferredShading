#ifndef _TGA_LOADER_H_
#define _TGA_LOADER_H_

#include <cstdint>

class TgaLoader
{
public:
	enum class ImagePixelFormat
	{
		Luminance,
		RGB,
		RGBA,
	};

	struct ImageData
	{
		ImagePixelFormat format;
		int bits;
		int width;
		int height;
		int depth;
		int bytesPerScanline;
		int size;
		uint8_t* pixels;
	};

	TgaLoader() = default;
	~TgaLoader();

	bool Load(const char* fileName);
	void Unload();

	const ImageData& GetImageData() const
		{ return _data; }

private:
	ImageData _data = { };
};


#endif // _TGA_LOADER_H_

#include "TgaLoader.h"
#include <cstdio>
#include <cstring>


#pragma pack(push, 1)

struct TgaHeader
{
	char id_length;
	char colormap_type;
	char image_type;

	short first_entry;
	short colormap_len;
	char clrmap_entry_size;

	short x_orig;
	short y_orig;
	short img_width;
	short img_height;
	char pixel_depth;
	char img_desc;
};

#pragma pack(pop)


TgaLoader::~TgaLoader()
{
	Unload();
}

bool TgaLoader::Load(const char* fileName)
{
	FILE* file = fopen(fileName, "rb");
	if (file == nullptr)
		return false;

	Unload();

	// read the header
	TgaHeader header;
	fread(&header, 1, sizeof(header), file);
	if (ferror(file) != 0)
	{
		fclose(file);
		return false;
	}

	// don't support colormap images
	if (header.colormap_type)
	{
		fclose(file);
		return false;
	}

	// check if supported format
	if (header.image_type != 2 &&
		header.image_type != 10 &&
		header.image_type != 3 &&
		header.image_type != 11)
	{
		fclose(file);
		return false;
	}

	// skip image id if any
	if (header.id_length)
		fseek(file, header.id_length, SEEK_CUR);

	// read image data
	_data.bits = header.pixel_depth;

	if (_data.bits == 8)
		_data.format = ImagePixelFormat::Luminance;
	else if (_data.bits == 24)
		_data.format = ImagePixelFormat::RGB;
	else if (_data.bits == 32)
		_data.format = ImagePixelFormat::RGBA;
	else
	{
		fclose(file);
		return false;
	}

	int px_bytes = _data.bits / 8;
	int num_px = header.img_width * header.img_height;
	int data_size = num_px * px_bytes;

	_data.width = header.img_width;
	_data.height = header.img_height;
	_data.depth = 1;
	_data.bytesPerScanline = px_bytes * header.img_width;
	_data.size = data_size;

	uint8_t* temp = new uint8_t[data_size];

	fread(temp, 1, data_size, file);

	_data.pixels = new uint8_t[data_size];

	const uint8_t* src = temp;
	uint8_t* dest = _data.pixels;
	if (header.image_type == 2)
	{ // uncompressed rgb(a)
		for (int px = 0; px < num_px; ++px)
		{
			dest[0] = src[2];
			dest[1] = src[1];
			dest[2] = src[0];

			if (px_bytes == 4)
				dest[3] = src[3];

			src += px_bytes;
			dest += px_bytes;
		}
	}
	else if (header.image_type == 10)
	{ // RLE compressed rgb(a)
		for (int px = 0; px < num_px;)
		{
			uint8_t packet_hdr = *src++;
			int packet_count = (packet_hdr & 0x7f) + 1;

			if (packet_hdr & 0x80)
			{ // run-length packet
				for (int i = 0; i < packet_count; ++i)
				{
					dest[0] = src[2];
					dest[1] = src[1];
					dest[2] = src[0];

					if (px_bytes == 4)
						dest[3] = src[3];

					dest += px_bytes;
					++px;
				}

				src += px_bytes;
			}
			else
			{ // raw packet
				for (int i = 0; i < packet_count; ++i)
				{
					dest[0] = src[2];
					dest[1] = src[1];
					dest[2] = src[0];

					if (px_bytes == 4)
						dest[3] = src[3];

					src += px_bytes;
					dest += px_bytes;
					++px;
				}
			}
		}
	}
	else if (header.image_type == 3)
	{ // uncompressed grayscale
		memcpy(dest, src, data_size);
	}
	else if (header.image_type == 11)
	{ // RLE compressed grayscale
		for (int px = 0; px < num_px; )
		{
			uint8_t packet_hdr = *src++;
			int packet_count = (packet_hdr & 0x7f) + 1;

			if (packet_hdr & 0x80)
			{ // run-length packet
				for (int i = 0; i < packet_count; ++i)
				{
					*dest++ = *src;
					++px;
				}
			}
			else
			{ // raw packet
				for (int i = 0; i < packet_count; ++i)
				{
					*dest++ = *src++;
					++px;
				}
			}
		}
	}

	if ((header.img_desc & 0x10) == 1)
	{
		// If we have top-to-bottom pixel ordering, flip the image verticaly.
		int scanline_bytes = _data.bytesPerScanline;
		uint8_t* ptr1 = ((uint8_t*)_data.pixels) + (_data.height - 1) * scanline_bytes;
		uint8_t* ptr2 = (uint8_t*)_data.pixels;
		int count = _data.height / 2;
		for (int row_i = 0; row_i < count; ++row_i)
		{
			memcpy(temp, ptr1, scanline_bytes);
			memcpy(ptr1, ptr2, scanline_bytes);
			memcpy(ptr2, temp, scanline_bytes);

			ptr1 -= scanline_bytes;
			ptr2 += scanline_bytes;
		}
	}

	delete[] temp;
	fclose(file);

	return true;
}

void TgaLoader::Unload()
{
	if(_data.pixels != nullptr)
	{
		delete[] _data.pixels;
		_data = {};
	}
}

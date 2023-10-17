#include "ImageLoad.h"
#include <fstream>
#include <iostream>
#include <cstring>

ImageLoad::ImageLoad():width(0),height(0),bitCount(0),dataSize(0),data(0),dataRGB(0)
{

}

ImageLoad::ImageLoad(const ImageLoad &obj)
{
	width = obj.getWidth();
	height = obj.getHeight();
	bitCount = obj.getBitCount();
	dataSize = obj.getDataSize();
	if (obj.getData())
	{
		data = new unsigned char[dataSize];
		memcpy(data, obj.getData(), dataSize);
	}
	else
	{
		data = 0;
	}

	if (obj.getDataRGB())
	{
		dataRGB = new unsigned char[dataSize];
		memcpy(dataRGB, obj.getDataRGB(), dataSize);
	}
	else
	{
		dataRGB = 0;
	}
}

ImageLoad::~ImageLoad()
{
	delete[]data;
	data = 0;
	delete[]dataRGB;
	dataRGB = 0;
}

ImageLoad &ImageLoad::operator=(const ImageLoad &obj)
{
	if (this == &obj)
	{
		return *this;
	}

	width = obj.getWidth();
	height = obj.getHeight();
	bitCount = obj.getBitCount();
	dataSize = obj.getDataSize();

	if (obj.getData())
	{
		data = new unsigned char[dataSize];
		memcpy(data, obj.getData(), dataSize);
	}
	else
	{
		data = 0;
	}

	if (obj.getDataRGB())
	{
		dataRGB = new unsigned char[dataSize];
		memcpy(dataRGB, obj.getDataRGB(), dataSize);
	}
	else
	{
		dataRGB = 0;
	}

	return *this;
}

void ImageLoad::Init()
{
	width = 0;
	height = 0;
	bitCount = 0;
	dataSize = 0;
	delete[]data;
	data = 0;
	delete[]dataRGB;
	dataRGB = 0;
}

bool ImageLoad::Load(const char *fileName)
{
	this->Init();

	// check NULL pointer
	if (!fileName)
	{
		return false;
	}

	// open a BMP file as binary mode
	std::ifstream inFile;
	inFile.open(fileName, std::ios::binary);    // binary mode
	if (!inFile.good())
	{
		return false;            // exit if failed
	}

	// list of entries in BMP header
	char id[2];             // magic identifier "BM" (2 bytes)
	int fileSize;           // file size in bytes (4)
	short reserved1;        // reserved 1 (2)
	short reserved2;        // reserved 2 (2)
	int dataOffset;         // starting offset of bitmap data (4)
	int infoHeaderSize;     // info header size (4)
	int width;              // image width (4)
	int height;             // image height (4)
	short planeCount;       // # of planes (2)
	short bitCount;         // # of bits per pixel (2)
	int compression;        // compression mode (4)
	int dataSizeWithPaddings; // bitmap data size with paddings in bytes (4)

	// read BMP header infos
	inFile.read(id, 2);                         // should be "BM"
	inFile.read((char*)&fileSize, 4);           // should be same as file size
	inFile.read((char*)&reserved1, 2);          // should be 0
	inFile.read((char*)&reserved2, 2);          // should be 0
	inFile.read((char*)&dataOffset, 4);
	inFile.read((char*)&infoHeaderSize, 4);     // should be 40
	inFile.read((char*)&width, 4);
	inFile.read((char*)&height, 4);
	inFile.read((char*)&planeCount, 2);         // should be 1
	inFile.read((char*)&bitCount, 2);           // 1, 4, 8, 24, or 32
	inFile.read((char*)&compression, 4);        // 0(uncompressed), 1(8-bit RLE), 2(4-bit RLE), 3(RGB with mask)
	inFile.read((char*)&dataSizeWithPaddings, 4);

	inFile.seekg(0, std::ios::end);
	fileSize = (int)inFile.tellg();

	// compute the number of paddings
	// In BMP, each scanline must be divisible evenly by 4.
	// If not divisible by 4, then each line adds
	// extra paddings. So it can be divided evenly by 4.
	int paddings = (4 - ((width * bitCount / 8) % 4)) % 4;

	// compute data size without paddings
	// NOTE: height can be negative
	int dataSize = width * abs(height) * bitCount / 8;

	// recompute data size with paddings (do not trust the data size in header)
	dataSizeWithPaddings = fileSize - dataOffset;   // it maybe greater than "dataSize+(height*paddings)" because 4-byte boundary for file size

	// now it is ready to store info and image data
	this->width = width;
	this->height = abs(height);
	this->bitCount = bitCount;
	this->dataSize = dataSize;

	// allocate data arrays
	// add extra bytes for paddings if width is not divisible by 4
	data = new unsigned char[dataSizeWithPaddings];
	dataRGB = new unsigned char[dataSize];

	if (compression == 0)                    // uncompressed
	{
		inFile.seekg(dataOffset, std::ios::beg); // move cursor to the starting position of data
		inFile.read((char*)data, dataSizeWithPaddings);
	}
	else if (compression == 1)               // 8-bit RLE(Run Length Encode) compressed
	{
		// get length of encoded data
		int size = fileSize - dataOffset;

		// allocate tmp array to store the encoded data
		unsigned char *encData = new unsigned char[size];

		// read data from file
		inFile.seekg(dataOffset, std::ios::beg);
		inFile.read((char*)encData, size);

		// decode RLE into image data buffer
		decodeRLE8(encData, data);

		// deallocate encoded data buffer after decoding
		delete[] encData;
	}

	// close it after reading
	inFile.close();

	// we don't need paddings, trim paddings from each line
	// Note that there is no padding in RLE compressed data
	if (compression == 0 && paddings > 0)
	{
		int lineWidth = width * bitCount / 8;

		// copy line by line
		int lineCount = abs(height);
		for (int i = 1; i < lineCount; ++i)
		{
			memcpy(&data[i*lineWidth], &data[i*(lineWidth + paddings)], lineWidth);
		}
	}

	// BMP is bottom-to-top orientation by default, flip image vertically
	// But if the height is negative value, then it is top-to-bottom orientation.
	if (height > 0)
		flipImage(data, width, height, bitCount / 8);

	// the colour components order of BMP image is BGR
	// convert image data to RGB order for convenience
	memcpy(dataRGB, data, dataSize);    // copy data to dataRGB first
	if (bitCount == 24 || bitCount == 32)
	{
		swapRedBlue(dataRGB, dataSize, bitCount / 8);
	}

	return true;
}

bool ImageLoad::decodeRLE8(const unsigned char *encData, unsigned char *outData)
{
	if (!encData || !outData)
	{
		return false;
	}

	unsigned char first, second;
	int i;
	bool stop = false;

	while (!stop)
	{
		// grab 2 bytes at the current position
		first = *encData++;
		second = *encData++;

		if (first)                   // encoded run mode
		{
			for (i = 0; i < first; ++i)
			{
				*outData++ = second;
			}
		}
		else
		{
			if (second == 1)         // reached the end of bitmap
			{
				stop = true;        // must stop decoding
			}
			else if (second == 2)    // delta mark
			{
				encData += 2;       // do nothing, but move the cursor 2 more bytes
			}
			else                    // unencoded run mode (second >= 3)
			{
				for (i = 0; i < second; ++i)
				{
					*outData++ = *encData++;
				}

				if (second % 2)      // if it is odd number, then there is a padding 0. ignore it
				{
					encData++;
				}
			}
		}
	}

	return true;
}

void ImageLoad::flipImage(unsigned char *data, int width, int height, int channelCount)
{
	if (!data) return;

	int lineSize = width * channelCount;
	unsigned char* temp = new unsigned char[lineSize];
	int half = height / 2;

	int line1 = 0;                          // first line
	int line2 = (height - 1) * lineSize;    // last line

	// scan only half of height
	for (int i = 0; i < half; ++i)
	{
		// copy line by line
		memcpy(temp, &data[line1], lineSize);
		memcpy(&data[line1], &data[line2], lineSize);
		memcpy(&data[line2], temp, lineSize);

		// move to next line
		line1 += lineSize;
		line2 -= lineSize;
	}

	delete[] temp;
}

void ImageLoad::swapRedBlue(unsigned char *data, int dataSize, int channelCount)
{
	if (!data)
	{
		return;
	}
	if (channelCount < 3)
	{
		return;            // must be 3 or 4
	}
	if (dataSize % channelCount)
	{
		return;     // must be divisible by the number of channels
	}

	unsigned char temp;
	int i;

	// swap the position of red and blue components
	for (i = 0; i < dataSize; i += channelCount)
	{
		temp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = temp;
	}
}
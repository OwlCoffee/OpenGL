#pragma once
#include "stdafx.h"

class ImageLoad
{
public:
	ImageLoad();
	ImageLoad(const ImageLoad &obj);
	~ImageLoad();

	ImageLoad &operator=(const ImageLoad &obj);

	bool Load(const char *fileName);

	int getWidth() const;
	int getHeight() const;
	int getBitCount() const;
	int getDataSize() const;
	const unsigned char *getData() const;
	const unsigned char *getDataRGB() const;

private:
	int width;
	int height;
	int bitCount;
	int dataSize;
	unsigned char *data;
	unsigned char *dataRGB;

	void Init();
	static bool decodeRLE8(const unsigned char *encData, unsigned char *data);
	static void flipImage(unsigned char *data, int width, int height, int channelCount);
	static void swapRedBlue(unsigned char *data, int dataSize, int channelCount);
};

inline int ImageLoad::getWidth() const
{
	return width;
}

inline int ImageLoad::getHeight() const
{
	return height;
}

inline int ImageLoad::getBitCount() const
{
	return bitCount;
}

inline int ImageLoad::getDataSize() const
{
	return dataSize;
}

inline const unsigned char *ImageLoad::getData() const
{
	return data;
}

inline const unsigned char *ImageLoad::getDataRGB() const
{
	return dataRGB;
}
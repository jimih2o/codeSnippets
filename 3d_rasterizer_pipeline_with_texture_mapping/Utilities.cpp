/* Start Header -------------------------------------------------------
File Name: Utilities.cpp
Purpose: Contains definition for FrameBuffer
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#include "Utilities.h"

unsigned char *FrameBuffer::buffer = NULL;
int FrameBuffer::width = 0;
int FrameBuffer::height = 0;

float *DepthBuffer::buffer = NULL;
int DepthBuffer::width = 0;
int DepthBuffer::height = 0;

DepthBuffer::DepthBuffer()
{
}

DepthBuffer::~DepthBuffer()
{
}

//Clears the frame buffer to the given (r, g, b) color
void DepthBuffer::Clear(float val)
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			buffer[i + j * width] = val ;
		}
	}
}

//Initializes the frame buffer
void DepthBuffer::Init(const unsigned int &w, const unsigned int &h)
{
	width = w;
	height = h;
	buffer = new float [width * height];

	Clear(1.0F);
}

//Set the pixel's color at (x,y)
void DepthBuffer::SetPixel(const int &x, const int &y, float z)
{
	int _y = height - y;

	if (x < 0 || x >= width || _y < 0 || _y >= height)
		return;

	buffer[x + _y * width] = z;
}

//Stores the color of the given (x,y) pixel in r, g & b
float DepthBuffer::GetPixel(const int &x, const int &y)
{
	int _y = height - y;

	if (x < 0 || x >= width || _y < 0 || _y >= height)
		return -1.0F ;

	return buffer[x + _y * width] ;
}

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
}

//Clears the frame buffer to the given (r, g, b) color
void FrameBuffer::Clear(const unsigned char &r, const unsigned char &g, const unsigned char &b)
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j<height; ++j)
		{
			buffer[i * 3 + j * width * 3] = r;
			buffer[i * 3 + j * width * 3 + 1] = g;
			buffer[i * 3 + j * width * 3 + 2] = b;
		}
	}
}

//Initializes the frame buffer
void FrameBuffer::Init(const unsigned int &w, const unsigned int &h)
{
	width = w;
	height = h;
	buffer = new unsigned char [width * height * 3];

	Clear(255, 255, 255);
}

//Set the pixel's color at (x,y)
void FrameBuffer::SetPixel(const int &x, const int &y, const unsigned char &r, const unsigned char &g, const unsigned char &b)
{
	int _y = height - y;

	if (x < 0 || x >= width || _y < 0 || _y >= height)
		return;

	buffer[x * 3 + _y * width * 3] = r;
	buffer[x * 3 + _y * width * 3 + 1] = g;
	buffer[x * 3 + _y * width * 3 + 2] = b;
}

//Stores the color of the given (x,y) pixel in r, g & b
void FrameBuffer::GetPixel(const int &x, const int &y, unsigned char &r, unsigned char &g, unsigned char &b)
{
	int _y = height - y;

	if (x < 0 || x >= width || _y < 0 || _y >= height)
		return;

	r = buffer[x * 3 + _y * width * 3];
	g = buffer[x * 3 + _y * width * 3 + 1];
	b = buffer[x * 3 + _y * width * 3 + 2];
}

/////
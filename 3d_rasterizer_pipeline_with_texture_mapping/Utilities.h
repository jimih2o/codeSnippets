/* Start Header -------------------------------------------------------
File Name: Utilities.h
Purpose: Contains declarations for FrameBuffer and glut
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#pragma once

#define NOMINMAX
#include "windows.h"
#include "GL/glut.h"


#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdlib>

// Typedefs for consistency
typedef unsigned char      u8;
typedef char               s8;
typedef unsigned int       u32;
typedef int                s32;
typedef unsigned long long u64;
typedef long long          s64;
typedef float              f32;
typedef double             f64;

// Do not change
static const f32 EPSILON = 0.0001f;

namespace util {
	static inline s32 Round(f32 f) {
		return (s32)(f + 0.5F) ;
	}
}

#define WIDTH  640
#define HEIGHT 480

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)

class FrameBuffer
{
	public:
		FrameBuffer();
		~FrameBuffer();

		static void Clear(const unsigned char &r, const unsigned char &g, const unsigned char &b);

		static void Init(const unsigned int &w, const unsigned int &h);
		static void SetPixel(const int &x, const int &y, const unsigned char &r, const unsigned char &g, const unsigned char &b);
		static void GetPixel(const int &x, const int &y, unsigned char &r, unsigned char &g, unsigned char &b);

	public:
		static unsigned char *buffer;
		static int width;
		static int height;
};

class DepthBuffer {
public:
	DepthBuffer() ;
	~DepthBuffer() ;
	static void Clear(float val);

	static void Init(const unsigned int &w, const unsigned int &h);
	static void SetPixel(const int &x, const int &y, float z);
	static float GetPixel(const int &x, const int &y);

	static float *buffer ;
	static int width ;
	static int height ;
};
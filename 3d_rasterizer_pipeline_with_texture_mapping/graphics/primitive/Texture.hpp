/* Start Header -------------------------------------------------------
File Name: Texture.hpp
Purpose: Texture stuff
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_6
Author: Jimi Huard, jimi.e 180003312
Creation date: 11/26/2013
- End Header --------------------------------------------------------*/
#pragma once

#include <cstring>
#include "../../math/MathDef.hpp"

namespace graphics {
	struct Texture {
		int width, height ;

		Texture() : bytes(nullptr), size(0), width(0), height(0) {}
		~Texture() {delete []bytes ;}

		Texture(Texture const &other) {
			std::memcpy(bytes = new unsigned char[other.size], other.bytes, other.size) ;
			size = other.size ;
			width = other.width ;
			height = other.height ;
		}

		void SetRawBytes(unsigned char *b, unsigned count, int w, int h) {
			bytes = b ;
			size = count ;
			width = w ;
			height = h ;
		}

		unsigned char const *GetBytes() const {
			return bytes ;
		}

		unsigned char *GetBytes() {
			return bytes ;
		}

		void PrintToConsole() {
			for (int y = height - 1; y >= 0; --y) {
				for (int x = 0; x < width; ++x) {
					unsigned char *location = bytes + x * width * 4 + y * 4 ;

					int rgba = *reinterpret_cast<int *>(location) ;
					printf("%X", rgba) ;
				}
				printf("\n") ;
			}
		}

		Vec4 UVToColor(float u, float v) {
			Vec4 color(0, 255, 255, 0) ;
			if (!bytes) return color ;

			int x = math::Round(u * (width - 1)) ;
			int y = math::Round(v * (height - 1)) ;

			if (x >= width)	x = width - 1 ;
			if (x < 0)      x = 0 ;

			if (y >= height) y = height - 1 ;
			if (y < 0)       y = 0 ;

			unsigned char r, g, b, a ;

			// 4 = rgba
			unsigned char *base = bytes + x * width * 4 + y * 4 ;

			r = base[0] ;
			g = base[1] ;
			b = base[2] ;
			a = base[3] ;

			color.r = static_cast<float>(r) ;
			color.g = static_cast<float>(g) ;
			color.b = static_cast<float>(b) ;
			color.a = static_cast<float>(a) ;

			return color ;
		}

	private:
		unsigned char *bytes ;
		unsigned size ;
	};

	static Texture GenerateGridTexture() {
		unsigned char * chessBoard = (unsigned char *)::operator new(128 * 128 * 4);
		bool black = false;
		for (int j = 0; j < 128; j++) {
			if (j % 32 == 0) black = !black;
			for (int i = 0; i < 128; i++) {
				if (i % 32 == 0) black = !black;
				*(chessBoard + (j * 128 * 4) + (i * 4) + 0) = black ? 0 : 255;
				*(chessBoard + (j * 128 * 4) + (i * 4) + 1) = black ? 0 : 255;
				*(chessBoard + (j * 128 * 4) + (i * 4) + 2) = black ? 0 : 255;
				*(chessBoard + (j * 128 * 4) + (i * 4) + 3) = 128;
			}
		}
		Texture t ;
		t.SetRawBytes(chessBoard, 128 * 128 * 4, 128, 128) ;
//		t.PrintToConsole() ;
		return t ;
	}

	class TextureManager {

	public:
		static Texture *GetTexture() {
			static Texture hack = GenerateGridTexture() ;
			return &hack ;
		}
	} ;
}
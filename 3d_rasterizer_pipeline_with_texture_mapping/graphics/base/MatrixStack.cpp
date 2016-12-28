/* Start Header -------------------------------------------------------
File Name: MatrixStack.cpp
Purpose: Contains definition for MatrixStack
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#include "MatrixStack.hpp"

namespace graphics {
	MatrixStack::MatrixStack() {
	}
	
	MatrixStack::~MatrixStack() {
	}

	Mat4 MatrixStack::Top() {
		if (matrices.Size() < 1)
			matrices.push_back(Mat4()) ;
		return matrices[matrices.Size() - 1] ;
	}

	void MatrixStack::Push(const Mat4 &mat) {
		if (matrices.Size() < 1)
			matrices.push_back(Mat4()) ;
		matrices.push_back(matrices[matrices.Size() - 1] * mat) ;
	}

	void MatrixStack::Pop(uint32 n) {
		for (uint32 i = 0; i < n && matrices.Size() > 1; ++i) {
			matrices.pop_back() ;
		}
	}
}

/* Start Header -------------------------------------------------------
File Name: MatrixStack.hpp
Purpose: Contains declaration for MatrixStack
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#pragma once

#include "../../type/Type.hpp"
#include "../../math/MathDef.hpp"
#include "../../type/Array.hpp"

namespace graphics {
	class MatrixStack {
		Array<Mat4> matrices ;

	public:
		MatrixStack() ;
		~MatrixStack() ;

		Mat4 Top() ;

		void Push(const Mat4 &mat) ;
		void Pop(uint32 n = 1) ;
		
		uint32 Size() const ;
	};
}
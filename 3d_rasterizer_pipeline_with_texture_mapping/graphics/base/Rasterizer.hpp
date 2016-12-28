/* Start Header -------------------------------------------------------
File Name: Rasterizer.hpp
Purpose: Contains definition for MatrixStack
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#pragma once

#include "../../Utilities.h"
#include "../../type/Type.hpp"
#include "../../type/Array.hpp"
#include "../../math/MathDef.hpp"
#include "../primitive/Camera.hpp"
#include "../primitive/Vertex.hpp"
#include "MatrixStack.hpp"

namespace graphics {
	class Model ;
	struct Mesh ;

	struct Viewport {
	private:
		Mat4 trans ;
		int32 x,y,width,height ;
		void CalculateTransform() {
			trans = Mat4() ;

			trans.m11 = width / 2 ;
			trans.m14 = width / 2 + x ;
			trans.m22 = -height / 2 ;
			trans.m24 = height / 2 + y ;

		}
		
	public:
		Viewport() : x(0),y(0),width(WIDTH),height(HEIGHT){CalculateTransform();};

		Viewport(int32 x,int32 y, int32 width, int32 height)
			:x(x),y(y),width(width),height(height){CalculateTransform();};

		Mat4 const &Transform() {
			return trans ;
		}
	};

	class Rasterizer {
	public:
		enum class State : uint32 {
			Fill,
			WireFrame,
			Nil
		};
	private:
		float fov, aspect ;

		enum p {
			near,
			far,
			left,
			right,
			bottom,
			top,
			count
		};

		Vec4 planes[p::count] ;

		State state ;
		Array<Model *> models ;
		Array<Viewport> viewports ;
		
		Mat4 perspective ;
		
		bool ShouldCullVertices(Array<Vertex> const &verts) ;
		void BackFaceRemove(Array<Vertex> &verts) ;

		void Triangularize(Array<Vertex> &verts) ;
		void ClipVertices(Array<Vertex> &verts) ;

		void ClipSegment(Vertex const &A, Vertex const &B, Vertex &nA, Vertex &nB, bool &success) ;
	public:
		MatrixStack stack ;

		Rasterizer() ;
		~Rasterizer() ;

		void SetState(State state) ;
		void Add(Model *m) ;

		void DrawElements(const Array<Vertex> &verts, const Array<uint32> &ind) ;
		void Draw(const Mesh &mesh) ;

		void Rasterize() ;

		Camera camera ;

		//! Draws a line in screen space
		void DrawLine(Vertex s, Vertex e) ;

		//! Draws a triangle in screen space
		void DrawTriangle(const Vertex&a, const Vertex&b, const Vertex&c) ;

		void Perspective(float fov, float aspect, float near, float far) ;
	};
}

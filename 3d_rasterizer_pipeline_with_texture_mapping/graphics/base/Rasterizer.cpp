/* Start Header -------------------------------------------------------
File Name: Rasterizer.cpp
Purpose: Contains definition for Rasterizer
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#include "Rasterizer.hpp"

#include "Model.hpp"
#include <src\Utilities.h>
#undef min
#undef max
#include <algorithm>
#include "../primitive/Texture.hpp"

namespace graphics {
	Rasterizer::Rasterizer()
	: state(State::Fill) {
		viewports.push_back(Viewport(0, 0, WIDTH, HEIGHT)) ;
		fov = aspect = 0.0F ;
	}

	Rasterizer::~Rasterizer() {
		for (Model *m : models)
			delete m ;
	}

	void Rasterizer::Add(Model *m) {
		models.push_back(m) ;
	}

	bool Rasterizer::ShouldCullVertices(Array<Vertex> const &verts) {
		bool keep = true ;

		for (uint32 i = 0; keep && i < p::count; ++i) {
			keep = false ;
			Vec4 const &plane = planes[i] ;
			
			for (uint32 j = 0; j < verts.Size(); ++j) {
				Vec4 const &vertex = verts[j].position ;
				if (vertex.w != 1.0F) __debugbreak() ;

				if (math::Dot(plane, vertex) > math::Epsilon) {
					keep = true ; 
					break ;
				}
			}
		}

		return !keep ;
	}


	void Rasterizer::BackFaceRemove(Array<Vertex> &verts) {
		// get each face
		Array<uint32> remove ;
		for (uint32 i = 0; i < verts.Size(); i += 3) {
			uint32 _i = i % verts.Size() ;
			uint32 n = (_i + 1) % (verts.Size()) ;
			uint32 l = (_i + 2) % (verts.Size()) ;
			Vertex &v1 = verts[_i] ;
			Vertex &v2 = verts[n] ;
			Vertex &v3 = verts[l] ;

			Vec4 normal = math::Cross(v2.position - v1.position, v3.position - v1.position) ;
			Vec4 p_to_eye = camera.position - v1.position ;
			if (math::Dot(normal, p_to_eye) < 0) {
				remove.push_back(_i) ;
				remove.push_back(n) ;
				remove.push_back(l) ;
			}
		}

		int32 offset = 0 ;
		for (uint32 i = 0; i < remove.Size(); i += 3) {
			uint32 a, b, c ;
			a = remove[i] ;
			b = remove[(i + 1) % remove.Size()] ;
			c = remove[(i + 2) % remove.Size()] ;
			
			auto it = verts.at(a + offset) ;
			verts.erase(it) ;
			--offset ;

			it = verts.at(b + offset) ;
			verts.erase(it) ;
			--offset ;

			it = verts.at(c + offset) ;
			verts.erase(it) ;
			--offset ;
		}
	}

	void Rasterizer::Triangularize(Array<Vertex> &verts) {
		Array<Vertex> n_verts ;

		Vertex const &n = verts[0] ;
		
		for (uint32 i = 1; i < verts.Size(); ++i) {
			Vertex const &v1 = verts[i] ;
			Vertex const &v2 = verts[(i + 1) % verts.Size()] ;

			n_verts.push_back(n) ;
			n_verts.push_back(v1) ;
			n_verts.push_back(v2) ;
		}

		verts = n_verts ;
	}

	void Rasterizer::ClipSegment(Vertex const &A, Vertex const &B, Vertex &nA, Vertex &nB, bool &success) {
		float a = 0, b = 1 ;

		for (Vec4 const &plane : planes) {
			if (a > b) break ;
			float alpha = math::Dot(plane, A.position) ;
			float beta  = math::Dot(plane, B.position) ;

			if (beta - alpha > 0) {
				b = std::min(b, alpha / (alpha - beta)) ;
			} else if (beta - alpha < 0) {
				a = std::max(a, alpha / (alpha - beta)) ;
			} else if (alpha > 0) {
				a = b + 1 ;
			}
		}
		if (a < b) {
			nA = A + a * (B-A) ;
			nB = A + b * (B-A) ;
			success = true ;
		} else success = false ;
	}

	static inline Vertex IntersectionVertex(Vertex const &A, Vertex const &B, float alpha, float beta) {
		return A + (B - A) * (alpha / (alpha - beta)) ;
	}

	// only clips to near plane as that produced best frame rate results 
	void Rasterizer::ClipVertices(Array<Vertex> &verts) {
		Array<Vertex> n_verts ;
		Array<Vertex> tri ;

		for (uint32 i = 0; i < verts.Size(); i += 3) {
			uint32 _i = i % verts.Size() ;
			uint32 n = (_i + 1) % (verts.Size()) ;
			uint32 l = (_i + 2) % (verts.Size()) ;
			Vertex const &v1 = verts[_i] ;
			Vertex const &v2 = verts[n] ;
			Vertex const &v3 = verts[l] ;

			// clipping goes here
			tri.push_back(v3) ;
			tri.push_back(v1) ;
			tri.push_back(v2) ;

			// sutherland-hodgman clipping algorithm
			int I = 3 ; // uses 3 to avoid unnecessary Size function call
			int J = 1 ;
			float beta = math::Dot(planes[p::near], tri[I - 1].position) ;
			Array<Vertex> L ;
			while (J <= 3) {
				float alpha = beta ;
				beta = math::Dot(planes[p::near], tri[J - 1].position) ;
				
				if (alpha >= 0 && beta >= 0) {
					L.push_back(tri[J - 1]) ;
				} else if (alpha >= 0 && beta < 0) {
					L.push_back(IntersectionVertex(tri[I - 1], tri[J - 1], alpha, beta)) ;
				} else if (alpha < 0 && beta >= 0) {
					L.push_back(IntersectionVertex(tri[I - 1], tri[J - 1], alpha, beta)) ;
					L.push_back(tri[J - 1]) ;
				}
				I = J ;
				J = J + 1 ;
			}

			// Only clipping to the near plane resulted in better frame rate
			Triangularize(L) ;

			for (Vertex const &v : L)
				n_verts.push_back(v) ;

			tri.Clear() ;
		}

		verts = n_verts ;
	}

	void Rasterizer::DrawElements(const Array<Vertex> &verts, const Array<uint32> &ind) {
		for (Viewport &view : viewports) {
			Array<Vertex> transformed ;
			Mat4 world_transform = stack.Top() ;

			for (uint32 i = 0; i < ind.Size(); i += 3) {
				uint32 _i = i % ind.Size() ;
				uint32 n = (_i + 1) % (ind.Size()) ;
				uint32 l = (_i + 2) % (ind.Size()) ;
				Vertex const &v1 = verts[ind[_i]] ;
				Vertex const &v2 = verts[ind[n]] ;
				Vertex const &v3 = verts[ind[l]] ;

				transformed.push_back(v1) ;
				transformed.push_back(v2) ;
				transformed.push_back(v3) ;
			}

			// put things into world space for culling/clipping
			for (Vertex &v : transformed) {	
				// MODEL->WORLD
				v.position = world_transform * v.position ;
			}

			// remove back faces
			BackFaceRemove(transformed) ;

			// Cull
			if (ShouldCullVertices(transformed)) return ;

			// Clip to near plane (could clip to all planes but actually decreased frame rate rather than improved)
			ClipVertices(transformed) ;

			for (Vertex &v : transformed) {
				// MODEL->CAMERA
				v.position = camera.GetTransform() * v.position ;
				v.cz = v.position.z ;

				if (v.cz >= -camera.focal) return ;

				// PERSPECTIVE
				v.position = perspective * v.position ;

				// NDC
				v.position.x /= v.position.w ;
				v.position.y /= v.position.w ;
				v.position.z /= v.position.w ;
				v.position.w /= v.position.w ;

				// VIEWPORT
				v.position = view.Transform() * v.position ;
			}

			for (uint32 i = 0; i < transformed.Size(); i += 3) {
				uint32 _i = i % transformed.Size() ;
				uint32 n = (_i + 1) % (transformed.Size()) ;
				uint32 l = (_i + 2) % (transformed.Size()) ;
				Vertex &v1 = transformed[_i] ;
				Vertex &v2 = transformed[n] ;
				Vertex &v3 = transformed[l] ;

				switch (state) {
				case State::Fill:
					DrawTriangle(v1, v2, v3) ;
					break ;

				case State::WireFrame:
					DrawLine(v1, v2) ;
					DrawLine(v2, v3) ;
					DrawLine(v3, v1) ;
					break ;

				default:
					break ;
				}
			}
		}
	}

	void Rasterizer::Draw(const Mesh &mesh) {
		DrawElements(mesh.verts, mesh.inds) ;
	}

	void Rasterizer::Rasterize() {
		Vec4 r0, r1, r2, r3 ;
		Mat4 trans = perspective * camera.GetTransform() ;

		r0 = *reinterpret_cast<Vec4 *>(trans.r1) ;
		r1 = *reinterpret_cast<Vec4 *>(trans.r2) ;
		r2 = *reinterpret_cast<Vec4 *>(trans.r3) ;
		r3 = *reinterpret_cast<Vec4 *>(trans.r4) ; 
/*
		r0 = Vec4(trans.m11, trans.m12, trans.m13, trans.m14) ;
		r1 = Vec4(trans.m21, trans.m22, trans.m23, trans.m24) ;
		r2 = Vec4(trans.m31, trans.m32, trans.m33, trans.m34) ;
		r3 = Vec4(trans.m41, trans.m42, trans.m43, trans.m44) ;
*/
		planes[p::near]   = -r3 - r2 ;
		planes[p::far]    =  r2 - r3 ;
		planes[p::left]   = -r0 - r3 ;
		planes[p::right]  =  r0 - r3 ;
		planes[p::bottom] = -r1 - r3 ;
		planes[p::top]    =  r1 - r3 ;

		for (Model *m : models) {
			m->Draw(*this) ;
		}
	}

	void Rasterizer::SetState(State state) {
		this->state = state ;
	}

	float32 ChangeSpace(float32 z, Camera const &cam) {
		return cam.far / (cam.far - cam.near) * (1.0F + cam.near / z) ;
	}

	void Rasterizer::DrawLine(Vertex s, Vertex e) {
		bool steep = std::abs(e.position.y - s.position.y) > std::abs(e.position.x - s.position.x) ;

		if (steep) {
			std::swap(s.position.x, s.position.y) ;
			std::swap(e.position.x, e.position.y) ;
		}

		if (s.position.x > e.position.x) {
			std::swap(s.position.x, e.position.x) ;
			std::swap(s.position.y, e.position.y) ;
			std::swap(s.cz, e.cz) ;
		}

		s32 x = s.position.x ;
		s32 y = s.position.y ;

		s32 dx = e.position.x - s.position.x ;
		s32 dy = std::abs(e.position.y - s.position.y) ;

		s32 dp = 2 * dy - dx ;

		s32 y_step = (s.position.y > e.position.y) ? -1 : 1 ;

		float32 z = ChangeSpace(s.cz,camera) ;
		float32 z_step = ChangeSpace(e.cz,camera) - ChangeSpace(s.cz,camera) ;
		float32 x_extent = (e.position.x - s.position.x) ;
		z_step /= x_extent ;

		Vec4 color = s.color ;
		Vec4 c_step = e.color - s.color ;
		c_step /= x_extent ;

		for (;x <= e.position.x; ++x) {
			if (steep) {
				if (z < DepthBuffer::GetPixel(y, x)) {
					DepthBuffer::SetPixel(y,x,z) ;
					FrameBuffer::SetPixel(y, x, color.r, color.g, color.b) ;
				}
			} else { 
				if (z < DepthBuffer::GetPixel(x, y)) {
					DepthBuffer::SetPixel(x, y, z) ;
					FrameBuffer::SetPixel(x, y, color.r, color.g, color.b) ;
				}
			}

			dp += 2 * dy ;

			if (dp > 0) {
				y  += y_step ;
				dp -= 2 * dx ;
			}
			z += z_step ;
			color += c_step ;
		}
	}

	static void ConfigurePoints(const Vertex&P1, const Vertex&P2, const Vertex&P3, 
									  Vertex&T, Vertex&M, Vertex&B, bool &left) {
		if (P1.position.y < P2.position.y) {
			if (P1.position.y < P3.position.y) {
				if (P2.position.y < P3.position.y) {
					T = P1 ; M = P2 ; B = P3 ; left = true ; 
				} else {
					T = P1 ; M = P3 ; B = P2 ; left = false ;
				}
			} else {
				T = P3 ; M = P1 ; B = P2 ; left = true ;
			}
		} else {
			if (P1.position.y < P3.position.y) {
				T = P2 ; M = P1 ; B = P3 ; left = false ;
			} else {
				if (P2.position.y < P3.position.y) {
					T = P2 ; M = P3 ; B = P1 ; left = true ;
				} else {
					T = P3 ; M = P2 ; B = P1 ; left = false ;
				}
			}
		}
	}

	void Rasterizer::DrawTriangle(const Vertex&a, const Vertex&_b, const Vertex&c) {
		f32 ml_inv, mr_inv, xl, xr ;
		Vertex top, mid, bottom ;
		bool left ;
		
		Texture *texture = TextureManager::GetTexture() ;

		ConfigurePoints(a, _b, c, top, mid, bottom, left) ;

		if (left) {
			ml_inv = (mid.position.x - top.position.x) 
				   / (mid.position.y - top.position.y) ;
			
			mr_inv = (bottom.position.x - top.position.x) 
				   / (bottom.position.y - top.position.y) ;
		} else {
			ml_inv  = (bottom.position.x - top.position.x)
					/ (bottom.position.y - top.position.y) ;

			mr_inv  = (mid.position.x - top.position.x)
					/ (mid.position.y - top.position.y) ;
		}

		Vec4 v1, v2 ;
		float32 z ;
		float32 z1, z2, z3 ;
		float32 az, bz, cz ;

		z1 = ChangeSpace(top.cz, camera) ;
		z2 = ChangeSpace(mid.cz, camera) ;
		z3 = ChangeSpace(bottom.cz, camera) ;

		v1 =    mid.position - top.position ;
		v2 = bottom.position - top.position ;

		v1.z = z2 - z1 ;
		v2.z = z3 - z1 ;

		Vec4 cross = math::Cross(v1,v2) ;

		az = cross.x ;
		bz = cross.y ;
		cz = cross.z ;

		float32 dzdx = -az / cz ;
		float32 dzdy = (-az * ml_inv - bz) / cz ;

		xl = xr = top.position.x ;
		z = z1 ;

		Vec2 l_start_uv = top.texCoord ;
		Vec2 l_end_uv = left ? mid.texCoord : bottom.texCoord ;
		
		float l_start_z = top.cz ;
		float l_end_z = left ? mid.cz : bottom.cz ;

		Vec2 r_start_uv = top.texCoord ;
		Vec2 r_end_uv   = left ? bottom.texCoord : mid.texCoord ;

		float r_start_z = top.cz ;
		float r_end_z = left ? bottom.cz : mid.cz ;

		Vec2 uv = top.texCoord ;

		s32 ceil_my = (s32)std::ceil(mid.position.y) - 1 ;
		for (s32 y = (s32)ceil(top.position.y);
			y <= ceil_my; ++y) {
			s32 ceil_xr = (s32)std::ceil(xr) - 1 ;
			float32 temp_z = z ;

			float y_travel = (y - top.position.y) / (mid.position.y - top.position.y) ;
			Vec2 l_uv = l_start_uv + y_travel * (l_end_uv - l_start_uv) ;
			Vec2 r_uv = r_start_uv + y_travel * (r_end_uv - r_start_uv) ;
			float l_z = l_start_z + y_travel * (l_end_z - l_start_z) ;
			float r_z = r_start_z + y_travel * (r_end_z - r_start_z) ;

			for(s32 x = (s32)std::ceil(xl); x <= ceil_xr; ++x) {
				if (temp_z < DepthBuffer::GetPixel(x, y)) {
					DepthBuffer::SetPixel(x, y, temp_z) ;
					
					float t_rel = (x - xl) / (xr - xl) ;
					t_rel = t_rel * l_z / (r_z + t_rel * (l_z - r_z)) ;

					uv = l_uv + t_rel * (r_uv - l_uv) ;

					Vec4 color = texture->UVToColor(uv.u, uv.v) ;
					FrameBuffer::SetPixel(x, y, color.r, color.g, color.b) ;	
				}

				temp_z += dzdx ;
			}

			xr += mr_inv ;
			xl += ml_inv ;
			z += dzdy ;

		}

		if (left) {
			ml_inv = (bottom.position.x - mid.position.x)
				   / (bottom.position.y - mid.position.y) ;

			xl = mid.position.x ;
		} else {
			mr_inv = (bottom.position.x - mid.position.x) 
				   / (bottom.position.y - mid.position.y) ;

			xr = mid.position.x ;
		}

		l_start_uv = left ? mid.texCoord : l_end_uv ;
		l_end_uv = bottom.texCoord ;

		r_start_uv = left ? r_end_uv : mid.texCoord ;
		r_end_uv = bottom.texCoord ;

		dzdy = (-az * ml_inv - bz) / cz ;

		s32 ceil_by = (s32)std::ceil(bottom.position.y) - 1 ;
		for (s32 y = (s32)std::ceil(mid.position.y); y <= ceil_by; ++y) {
			s32 ceil_xr = (s32)std::ceil(xr) - 1 ;
			float32 temp_z = z ;

			float y_travel = (y - mid.position.y) / (bottom.position.y - mid.position.y) ;
			Vec2 l_uv = l_start_uv + y_travel * (l_end_uv - l_start_uv) ;
			Vec2 r_uv = r_start_uv + y_travel * (r_end_uv - r_start_uv) ;
			float l_z = l_start_z + y_travel * (l_end_z - l_start_z) ;
			float r_z = r_start_z + y_travel * (r_end_z - r_start_z) ;

			for (s32 x = (s32)std::ceil(xl); x <= ceil_xr; ++x) {
				if (temp_z < DepthBuffer::GetPixel(x, y)) {
					DepthBuffer::SetPixel(x, y, temp_z) ;
						
					float t_rel = (x - xl) / (xr - xl) ;

					t_rel = t_rel * l_z / (r_z + t_rel * (l_z - r_z)) ;
					uv = l_uv + t_rel * (r_uv - l_uv) ;

					Vec4 color = texture->UVToColor(uv.u, uv.v) ;
					FrameBuffer::SetPixel(x, y, color.r, color.g, color.b) ;
				}

				temp_z += dzdx ;
			}

			xr += mr_inv ;
			xl += ml_inv ;
			z  += dzdy ;
		}
	}

	void Rasterizer::Perspective(float fov, float aspect, float near, float far) {
		perspective.Zero() ;
		float cotan = 1.0F / (std::tan((fov * 3.1415926F / 180.0F) / 2.0F)) ;
		perspective.m11 = cotan / aspect ;
		perspective.m22 = cotan ; 
//		perspective.m11 = camera.focal / (camera.r - camera.l) ;
//		perspective.m22 = camera.focal / (camera.t - camera.b) ;
		perspective.m33 = -(near+far) / (far-near) ;
		perspective.m34 = -(2*near*far) / (far-near) ;
		perspective.m43 = 1 ;

		this->fov = fov * 3.1415926F / 180.0F ;
		this->aspect = aspect ;
	}

}

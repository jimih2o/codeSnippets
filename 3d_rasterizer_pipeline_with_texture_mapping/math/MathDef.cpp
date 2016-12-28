#include "MathDef.hpp"

#include <algorithm>

namespace math {
	Vec3 Promote(Vec2 const &v) {
		return Vec3(v) ;
	}

	Vec4 Promote(Vec3 const &v) {
		return Vec4(v) ;
	}

	Mat3 Promote(Mat2 const &m) {
		return Mat3(m.m11, m.m12, 0,
			        m.m21, m.m22, 0,
					0,     0,     1) ;
	}

	Mat4 Promote(Mat3 const &m) {
		return Mat4(
			m.m11, m.m12, m.m13, 0,
			m.m21, m.m22, m.m23, 0,
			m.m31, m.m32, m.m33, 0,
			0,     0,     0,     1) ;
	}


	/*****************************************
	Common functions
	*/
	float Dot(Vec2 const &v1, Vec2 const &v2) {
		return v1.x * v2.x + v1.y * v2.y ;
	}
	float Dot(Vec3 const &v1, Vec3 const &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z ;
	}
	float Dot(Vec4 const &v1, Vec4 const &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w ;
	}

	float Length(Vec2 const &v) {
		return std::sqrt(LengthSq(v)) ;
	}

	float Length(Vec3 const &v) {
		return std::sqrt(LengthSq(v)) ;
	}

	float Length(Vec4 const &v) {
		return std::sqrt(LengthSq(v)) ;
	}
	
	float LengthSq(Vec2 const &v) {
		return Dot(v,v) ;
	}

	float LengthSq(Vec3 const &v) {
		return Dot(v,v) ;
	}

	float LengthSq(Vec4 const &v) {
		return Dot(v,v) ;
	}

	float Cross2D(Vec2 const &v1, Vec2 const &v2) {
		return v1.x * v2.y - v1.y * v2.x ;
	}

	Vec3 Cross(Vec3 const &v1, Vec3 const &v2) {
		Vec3 ret ;
		ret.x = v1.y * v2.z - v1.z * v2.y ;
		ret.y = v1.z * v2.x - v1.x * v2.z ;
		ret.z = v1.x * v2.y - v1.y * v2.x ;
		return ret ;
	}

	Vec4 Cross(Vec4 const &v1, Vec4 const &v2) {
		return Promote(Cross(v1.xyz(), v2.xyz())) ;
	}

	/*****************************************
	Binary operators
	*/
	Vec2 operator+(Vec2 const &v1, Vec2 const &v2) {
		return Vec2(v1.x + v2.x, v1.y + v2.y) ;
	}

	Vec3 operator+(Vec3 const &v1, Vec3 const &v2) {
		return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z) ;
	}

	Vec4 operator+(Vec4 const &v1, Vec4 const &v2) {
		return Vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w) ;
	}

	Mat2 operator+(Mat2 const &m1, Mat2 const &m2) {
		return Mat2(m1.a + m2.a, m1.b + m2.b, m1.c + m2.c, m1.d + m2.d) ;
	}

	Mat3 operator+(Mat3 const &m1, Mat3 const &m2) {
		return Mat3(
			m1.a + m2.a, m1.b + m2.b, m1.c + m2.c,
			m1.d + m2.d, m1.e + m2.e, m1.f + m2.f,
			m1.g + m2.g, m1.h + m2.h, m1.i + m2.i
			) ;
	}

	Mat4 operator+(Mat4 const &m1, Mat4 const &m2) {
		return Mat4(
			m1.a + m2.a, m1.b + m2.b, m1.c + m2.c, m1.d + m2.d, 
			m1.e + m2.e, m1.f + m2.f, m1.g + m2.g, m1.h + m2.h, 
			m1.i + m2.i, m1.j + m2.j, m1.k + m2.k, m1.l + m2.l,
			m1.m + m2.m, m1.n + m2.n, m1.o + m2.o, m1.p + m2.p
			) ;
	}

	Vec2 operator-(Vec2 const &v1, Vec2 const &v2) {
		return Vec2(v1.x - v2.x, v1.y - v2.y) ;
	}
	
	Vec3 operator-(Vec3 const &v1, Vec3 const &v2) {
		return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z) ;
	}

	Vec4 operator-(Vec4 const &v1, Vec4 const &v2) {
		return Vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w) ;
	}

	Vec2 operator*(Vec2 const &v, float scalar) {
		return Vec2(v.x * scalar, v.y * scalar) ;
	}
	
	Vec3 operator*(Vec3 const &v, float scalar) {
		return Vec3(v.x * scalar, v.y * scalar, v.z * scalar) ;
	}

	Vec4 operator*(Vec4 const &v, float scalar) {
		return Vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar) ;
	}

	Vec2 operator*(Mat2 const &m, Vec2 const &v) {
		return Vec2(m.m11 * v.x + m.m12 * v.y,
			m.m21 * v.x + m.m22 * v.y) ;
	}

	Vec3 operator*(Mat3 const &m, Vec3 const &v) {
		return Vec3(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z,
			        m.m21 * v.x + m.m22 * v.y + m.m23 * v.z,
					m.m31 * v.x + m.m32 * v.y + m.m33 * v.z) ;
	}
	
	Vec4 operator*(Mat4 const &m, Vec4 const &v) {
		return Vec4(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z + m.m14 * v.w,
			        m.m21 * v.x + m.m22 * v.y + m.m23 * v.z + m.m24 * v.w,
			        m.m31 * v.x + m.m32 * v.y + m.m33 * v.z + m.m34 * v.w,
					m.m41 * v.x + m.m42 * v.y + m.m43 * v.z + m.m44 * v.w) ;
	}

	Mat2 operator*(Mat2 const &m1, Mat2 const &m2) {
		static const int n_sides = 2 ;

		Mat2 n ;
		n.Zero() ;

		for (int i = 0; i < n_sides; ++i) {
			for (int j = 0; j < n_sides; ++j) {
				for (int k = 0; k < n_sides; ++k) {
					n.mat[i][j] += m1.mat[i][k] * m2.mat[k][j] ;
				}
			}
		}

		return n ;
	}

	Mat3 operator*(Mat3 const &m1, Mat3 const &m2) {
		static const int n_sides = 3 ;

		Mat3 n ;
		n.Zero() ;

		for (int i = 0; i < n_sides; ++i) {
			for (int j = 0; j < n_sides; ++j) {
				for (int k = 0; k < n_sides; ++k) {
					n.mat[i][j] += m1.mat[i][k] * m2.mat[k][j] ;
				}
			}
		}

		return n ;
	}

	Mat4 operator*(Mat4 const &m1, Mat4 const &m2) {
		static const int n_sides = 4 ;

		Mat4 n ;
		n.Zero() ;

		for (int i = 0; i < n_sides; ++i) {
			for (int j = 0; j < n_sides; ++j) {
				for (int k = 0; k < n_sides; ++k) {
					n.mat[i][j] += m1.mat[i][k] * m2.mat[k][j] ;
				}
			}
		}

		return n ;
	}

	Vec2 operator/(Vec2 const &v, float scalar) {
		return Vec2(v.x / scalar, v.y / scalar) ;
	}
	
	Vec3 operator/(Vec3 const &v, float scalar) {
		return Vec3(v.x / scalar, v.y / scalar, v.z / scalar) ;
	}

	Vec4 operator/(Vec4 const &v, float scalar) {
		return Vec4(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar) ;
	}

	Vec2 operator*(float scalar, Vec2 const &v) {
		return v * scalar ;
	}

	Vec3 operator*(float scalar, Vec3 const &v) {
		return v * scalar ;
	}

	Vec4 operator*(float scalar, Vec4 const &v) {
		return v * scalar ;
	}

	/*****************************************
	Side effect + Unary operators/functions
	*/
	Vec2& Normalize(Vec2 &v) {
		float len = Length(v) ;
		if (len < Epsilon) return v ;
		v.x /= len ;
		v.y /= len ;
		return v ;
	}

	Vec3& Normalize(Vec3 &v) {
		float len = Length(v) ;
		if (len < Epsilon) return v ;
		v.x /= len ;
		v.y /= len ;
		v.z /= len ;
		return v ;
	}

	Vec4& Normalize(Vec4 &v) {
		float len = Length(v) ;
		if (len < Epsilon) return v ;
		v.x /= len ;
		v.y /= len ;
		v.z /= len ;
		v.w /= len ;
		return v ;
	}

	Vec2 Normal(Vec2 const &v) {
		return Normalize(Vec2(v)) ;
	}

	Vec3 Normal(Vec3 const &v) {
		return Normalize(Vec3(v)) ;
	}

	Vec4 Normal(Vec4 const &v) {
		return Normalize(Vec4(v)) ;
	}

	Vec2 &operator+=(Vec2 &v1, Vec2 const &v2) {
		v1.x += v2.x ;
		v1.y += v2.y ;
		return v1 ;
	}

	Vec3 &operator+=(Vec3 &v1, Vec3 const &v2) {
		v1.x += v2.x ;
		v1.y += v2.y ;
		v1.z += v2.z ;
		return v1 ;
	}
	
	Vec4 &operator+=(Vec4 &v1, Vec4 const &v2) {
		v1.x += v2.x ;
		v1.y += v2.y ;
		v1.z += v2.z ;
		v1.w += v2.w ;
		return v1 ;
	}

	Vec2 &operator-=(Vec2 &v1, Vec2 const &v2) {
		v1.x -= v2.x ;
		v1.y -= v2.y ;
		return v1 ;
	}
	
	Vec3 &operator-=(Vec3 &v1, Vec3 const &v2) {
		v1.x -= v2.x ;
		v1.y -= v2.y ;
		v1.z -= v2.z ;
		return v1 ;
	}
	
	Vec4 &operator-=(Vec4 &v1, Vec4 const &v2) {
		v1.x -= v2.x ;
		v1.y -= v2.y ;
		v1.z -= v2.z ;
		v1.w -= v2.w ;
		return v1 ;
	}

	Vec2 &operator*=(Vec2 &v1, float scalar) {
		v1.x *= scalar ;
		v1.y *= scalar ;
		return v1 ;
	}

	Vec3 &operator*=(Vec3 &v1, float scalar) {
		v1.x *= scalar ;
		v1.y *= scalar ;
		v1.z *= scalar ;
		return v1 ;
	}

	Vec4 &operator*=(Vec4 &v1, float scalar) {
		v1.x *= scalar ;
		v1.y *= scalar ;
		v1.z *= scalar ;
		v1.w *= scalar ;
		return v1 ;
	}

	Vec2 &operator/=(Vec2 &v1, float scalar) {
		v1.x /= scalar ;
		v1.y /= scalar ;
		return v1 ;
	}

	Vec3 &operator/=(Vec3 &v1, float scalar) {
		v1.x /= scalar ;
		v1.y /= scalar ;
		v1.z /= scalar ;
		return v1 ;
	}

	Vec4 &operator/=(Vec4 &v1, float scalar) {
		v1.x /= scalar ;
		v1.y /= scalar ;
		v1.z /= scalar ;
		v1.w /= scalar ;
		return v1 ;
	}

	/*****************************************
	Useful creation functions
	*/
	Mat2 Rotate2D(float angle) {
		float sin = std::sin(angle) ;
		float cos = std::cos(angle) ;
		return Mat2(cos, -sin, sin, cos) ;
	}

	Mat4 Rotate4D(float angle, Vec4 const &axis) {
		Vec4 normal = Normal(axis) ;

		float cos = std::cos(angle) ;
		float sin = std::sin(angle) ;

		Mat4 s1 = Scale4D(cos, cos, cos, 1.0F) ;
		float one_minus_cos = 1.0F - cos ;

		// Tensor matrix scaled by 1 - cos
		Mat4 s2 = Mat4(
			normal.x * normal.x * one_minus_cos, normal.x * normal.y * one_minus_cos, normal.x * normal.z * one_minus_cos, 0,
			normal.y * normal.x * one_minus_cos, normal.y * normal.y * one_minus_cos, normal.y * normal.z * one_minus_cos, 0,
			normal.z * normal.x * one_minus_cos, normal.z * normal.y * one_minus_cos, normal.z * normal.z * one_minus_cos, 0,
			0, 0, 0, 0
			) ;

		// cross matrix scaled by sin
		Mat4 s3 = Mat4(
			0, -normal.z * sin, normal.y * sin, 0,
			normal.z * sin, 0, -normal.x * sin, 0,
			-normal.y * sin, normal.x * sin, 0, 0,
			0, 0, 0, 0
			) ;

		return s1 + s2 + s3 ;
	}

	Mat3 Translate(Vec2 const &v) {
		return Mat3(1, 0, v.x,
			        0, 1, v.y,
					0, 0, 1) ;
	}

	Mat4 Translate(Vec3 const &v) {
		return Mat4(1, 0, 0, v.x,
			        0, 1, 0, v.y,
					0, 0, 1, v.z,
					0, 0, 0, 1) ;
	}

	Mat2 Scale2D(float scalar) {
		return Mat2(scalar, 0, 0, scalar) ;
	}

	Mat3 Scale3D(float scalar) {
		return Mat3(scalar, 0, 0,
			        0, scalar, 0, 
					0, 0, scalar) ;
	}

	Mat4 Scale4D(float scalar) {
		return Mat4(scalar, 0, 0, 0,
			        0, scalar, 0, 0,
					0, 0, scalar, 0,
					0, 0, 0, scalar) ;
	}

	Mat2 Scale2D(float x, float y) {
		return Mat2(x, 0, 0, y) ;
	}

	Mat3 Scale3D(float x, float y, float z) {
		return Mat3(x, 0, 0,
			        0, y, 0,
					0, 0, z) ;
	}

	Mat4 Scale4D(float x, float y, float z, float w) {
		return Mat4(x, 0, 0, 0,
			        0, y, 0, 0,
					0, 0, z, 0,
					0, 0, 0, w) ;
	}

	Mat2 Scale2D(Vec2 const &v) {
		return Scale2D(v.x, v.y) ;
	}

	Mat3 Scale3D(Vec3 const &v) {
		return Scale3D(v.x,v.y,v.z) ;
	}

	Mat4 Scale4D(Vec4 const &v) {
		return Scale4D(v.x,v.y,v.z,v.w) ;
	}

	Vec2 FastRotate(Vec2 const &v, float angle) {
		float cos = std::cos(angle) ;
		float sin = std::sin(angle) ;
		return Vec2(v.x * cos - v.y * sin, v.x * sin + v.y * cos) ;
	}
	
	/*********************************************
	Useful collision stuff
	*/
	static Vec2 Project2D(Vec2 const &vector, Vec2 const &line) {
		Vec2 n = Normal(line) ;
		return Dot(vector, n) * n ;
	} 

	// SEGMENT PQ -> does t lie within?
	static bool OverlapPointSegment(Vec2 const &p, Vec2 const &q, Vec2 const &t) {
		using std::max ;
		using std::min ;

		if (t.x <= max(p.x, q.x) && t.x >= min(p.x, q.x)
		 && t.y <= max(p.y, q.y) && t.y >= min(p.y, q.y))
			return true ;
		return false ;
	}

	static bool Overlap2D(Vec2 const &p1, Vec2 const &p2, Vec2 const &q1, Vec2 const &q2) {
		// test Q vs P
		if (OverlapPointSegment(p1, p2, q1)) return true ;
		if (OverlapPointSegment(p1, p2, q2)) return true ;

		// test P vs Q
		if (OverlapPointSegment(q1, q2, p1)) return true ;
		if (OverlapPointSegment(q1, q2, p2)) return true ;

		return false ;
	}

	static Vec2 CalculateANormal2D(Vec2 const &v1, Vec2 const &v2) {
		return Normal(Vec2(v2.y - v1.y, v1.x - v2.x)) ;
	}

	std::vector<Vec2> Generate2DNormals(std::vector<Vec2> const &edges) {
		std::vector<Vec2> norms ;
		for (unsigned i = 0; i < edges.size(); ++i) {
			Vec2 const &v1 = edges[i] ;
			Vec2 const &v2 = edges[(i + 1) % edges.size()] ;
			norms.push_back(CalculateANormal2D(v1, v2)) ;
		}
		return norms ;
	}

	static Vec2 GetMinProjected(Vec2 const &normal, std::vector<Vec2> const &obj) {
		if (obj.empty()) return Vec2() ; // uh ohes
		Vec2 min = Project2D(obj[0], normal) ;
		float least = Dot(normal, min) ;

		for (unsigned i = 1; i < obj.size(); ++i) {
			Vec2 projected = Project2D(obj[i], normal) ;
			float check = Dot(projected, normal) ;
			if (check < least) {
				least = check ;
				min = projected ;
			}
		}
		return min ;
	}

	static Vec2 GetMaxProjected(Vec2 const &normal, std::vector<Vec2> const &obj) {
		if (obj.empty()) return Vec2() ; // uh ohes
		Vec2 max = Project2D(obj[0], normal) ;
		float most = Dot(normal, max) ;

		for (unsigned i = 1; i < obj.size(); ++i) {
			Vec2 projected = Project2D(obj[i], normal) ;
			float check = Dot(projected, normal) ;
			if (check > most) {
				most = check ;
				max = projected ;
			}
		}
		return max ;
	}


	bool SAT2D(std::vector<Vec2> const &obj1, std::vector<Vec2> const &obj2) {
		if (obj1.empty() || obj2.empty()) return false ;

		auto norm1 = Generate2DNormals(obj1) ;

		for (Vec2 const &normal : norm1) {
			Vec2 min1 ;
			Vec2 max1 ;
			Vec2 min2 ;
			Vec2 max2 ;

			// Generate min/max verts on obj1
			min1 = GetMinProjected(normal, obj1) ;
			max1 = GetMaxProjected(normal, obj1) ;

			// Generate min/max verts on obj2
			min2 = GetMinProjected(normal, obj2) ;
			max2 = GetMaxProjected(normal, obj2) ;

			if (!Overlap2D(min1, max1, min2, max2)) return false ;
		}

		auto norm2 = Generate2DNormals(obj2) ;
		for (Vec2 const &normal : norm2) {
			Vec2 min1 ;
			Vec2 max1 ;
			Vec2 min2 ;
			Vec2 max2 ;

			// Generate min/max verts on obj1
			min1 = GetMinProjected(normal, obj1) ;
			max1 = GetMaxProjected(normal, obj1) ;

			// Generate min/max verts on obj2
			min2 = GetMinProjected(normal, obj2) ;
			max2 = GetMaxProjected(normal, obj2) ;


			if (!Overlap2D(min1, max1, min2, max2)) return false ;
		}

		return true ;
	}

}
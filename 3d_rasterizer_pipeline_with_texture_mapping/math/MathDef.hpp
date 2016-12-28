#pragma once

#include <cmath>
#include <vector>

namespace math {
	static float const Epsilon = .1e-5F ;

	struct Vec2 ;
	struct Vec3 ;
	struct Vec4 ;
	struct Mat2 ;
	struct Mat3 ;
	struct Mat4 ;

	Vec3 Promote(Vec2 const &v) ;
	Vec4 Promote(Vec3 const &v) ;

	Mat3 Promote(Mat2 const &m) ;
	Mat4 Promote(Mat3 const &m) ;

	/*****************************************
	Common functions
	*/
	float Dot(Vec2 const &v1, Vec2 const &v2) ;
	float Dot(Vec3 const &v1, Vec3 const &v2) ;
	float Dot(Vec4 const &v1, Vec4 const &v2) ;
	
	float Length(Vec2 const &v) ;
	float Length(Vec3 const &v) ;
	float Length(Vec4 const &v) ;
	
	float LengthSq(Vec2 const &v) ;
	float LengthSq(Vec3 const &v) ;
	float LengthSq(Vec4 const &v) ;

	float Cross2D(Vec2 const &v1, Vec2 const &v2) ;
	Vec3 Cross(Vec3 const &v1, Vec3 const &v2) ;
	Vec4 Cross(Vec4 const &v1, Vec4 const &v2) ;

	/*****************************************
	Binary operators
	*/
	Vec2 operator+(Vec2 const &v1, Vec2 const &v2) ;
	Vec3 operator+(Vec3 const &v1, Vec3 const &v2) ;
	Vec4 operator+(Vec4 const &v1, Vec4 const &v2) ;

	Mat2 operator+(Mat2 const &m1, Mat2 const &m2) ;
	Mat3 operator+(Mat3 const &m1, Mat3 const &m2) ;
	Mat4 operator+(Mat4 const &m1, Mat4 const &m2) ;

	Vec2 operator-(Vec2 const &v1, Vec2 const &v2) ;
	Vec3 operator-(Vec3 const &v1, Vec3 const &v2) ;
	Vec4 operator-(Vec4 const &v1, Vec4 const &v2) ;

	Vec2 operator*(Vec2 const &v, float scalar) ;
	Vec3 operator*(Vec3 const &v, float scalar) ;
	Vec4 operator*(Vec4 const &v, float scalar) ;

	Vec2 operator*(Mat2 const &m, Vec2 const &v) ;
	Vec3 operator*(Mat3 const &m, Vec3 const &v) ;
	Vec4 operator*(Mat4 const &m, Vec4 const &v) ;

	Mat2 operator*(Mat2 const &m1, Mat2 const &m2) ;
	Mat3 operator*(Mat3 const &m1, Mat3 const &m2) ;
	Mat4 operator*(Mat4 const &m1, Mat4 const &m2) ;

	Vec2 operator/(Vec2 const &v, float scalar) ;
	Vec3 operator/(Vec3 const &v, float scalar) ;
	Vec4 operator/(Vec4 const &v, float scalar) ;

	Vec2 operator*(float scalar, Vec2 const &v) ;
	Vec3 operator*(float scalar, Vec3 const &v) ;
	Vec4 operator*(float scalar, Vec4 const &v) ;

	/*****************************************
	Side effect + Unary operators/functions
	*/
	Vec2& Normalize(Vec2 &v) ;
	Vec3& Normalize(Vec3 &v) ;
	Vec4& Normalize(Vec4 &v) ;

	Vec2 Normal(Vec2 const &v) ;
	Vec3 Normal(Vec3 const &v) ;
	Vec4 Normal(Vec4 const &v) ;

	Vec2 &operator+=(Vec2 &v1, Vec2 const &v2) ;
	Vec3 &operator+=(Vec3 &v1, Vec3 const &v2) ;
	Vec4 &operator+=(Vec4 &v1, Vec4 const &v2) ;

	Vec2 &operator-=(Vec2 &v1, Vec2 const &v2) ;
	Vec3 &operator-=(Vec3 &v1, Vec3 const &v2) ;
	Vec4 &operator-=(Vec4 &v1, Vec4 const &v2) ;

	Vec2 &operator*=(Vec2 &v1, float scalar) ;
	Vec3 &operator*=(Vec3 &v1, float scalar) ;
	Vec4 &operator*=(Vec4 &v1, float scalar) ;

	Vec2 &operator/=(Vec2 &v1, float scalar) ;
	Vec3 &operator/=(Vec3 &v1, float scalar) ;
	Vec4 &operator/=(Vec4 &v1, float scalar) ;

	/*****************************************
	Useful creation functions
	*/
	Mat2 Rotate2D(float angle) ;
	Mat4 Rotate4D(float angle, Vec4 const &axis) ;

	Mat3 Translate(Vec2 const &v) ;
	Mat4 Translate(Vec3 const &v) ;
	
	Mat2 Scale2D(float scalar) ;
	Mat3 Scale3D(float scalar) ;
	Mat4 Scale4D(float scalar) ;

	Mat2 Scale2D(float x, float y) ;
	Mat3 Scale3D(float x, float y, float z) ;
	Mat4 Scale4D(float x, float y, float z, float w) ;

	Mat2 Scale2D(Vec2 const &v) ;
	Mat3 Scale3D(Vec3 const &v) ;
	Mat4 Scale4D(Vec4 const &v) ;

	Vec2 FastRotate(Vec2 const &v, float angle) ;

	/*********************************************
	Useful collision stuff
	*/
	bool SAT2D(std::vector<Vec2> const &obj1, std::vector<Vec2> const &obj2) ;

	/*********************************************
	Inline helper functions
	*/
	inline float Min(float x, float y) {
		return x < y ? x : y ;
	}

	inline float Max(float x, float y) {
		return x > y ? x : y ;
	}

	inline int Round(float x) {
		return static_cast<int>(x + 0.5F) ;
	}

	struct Vec2 {
		union {
			struct {
				float x, y ;
			};
			struct {
				float u, v ;
			};
			float array[2] ;
		};

		Vec2(float x, float y) : x(x), y(y) {};
		Vec2() : x(0), y(0) {};

		Vec2 operator-() const {
			return Vec2(-x, -y) ;
		}
	};

	struct Vec3 {
		union {
			struct {
				float x, y, z ;
			};
			struct {
				float r, g, b ;
			};
			float array[3] ;
		};

		Vec3() : x(0), y(0), z(0) {};

		Vec3(float x, float y, float z = 0.0F)
			: x(x), y(y), z(z) {}

		Vec3(const Vec2 &v2, float _z = 0.0F) {
			x = v2.x ;
			y = v2.y ;
			z = _z ;
		}

		Vec3 operator-() const {
			return Vec3(-x, -y, -z) ;
		}

		Vec2 xy() const {
			return Vec2(x, y) ;
		}
	};

	struct Vec4 {
		union {
			struct {
				float x, y, z, w ;
			};
			struct {
				float r, g, b, a ;
			};
			
			float array[4] ;
		};

		Vec4()
			: x(0), y(0), z(0), w(0) {};
		
		Vec4(float x, float y, float z, float w = 0.0F)
			: x(x), y(y), z(z), w(w) {} ;

		Vec4(Vec3 const &v3, float w = 0.0F)
			: x(v3.x), y(v3.y), z(v3.z), w(w) {} ;

		Vec4 operator-() const {
			return Vec4(-x,-y,-z,-w) ;
		}

		Vec3 xyz() const {
			return Vec3(x, y, z) ;
		}
	};

	struct Mat2 {
		union {
			struct {
				float m11, m12 ;
				float m21, m22 ;
			};
			struct {
				float a, b, 
					  c, d ;
			};
			struct {
				float r1[2] ;
				float r2[2] ;
			};

			float mat[2][2] ;
			float array[4] ;
		};

		Mat2(float a, float b, float c, float d)
			: m11(a), m12(b), m21(c), m22(d) {}

		Mat2()
		: a(1), b(0), c(1), d(0) {}

		void Zero() {
			a = b = c = d = 0 ;
		}
	};

	struct Mat3 {
		union {
			struct {
				float m11, m12, m13 ;
				float m21, m22, m23 ;
				float m31, m32, m33 ;
			};
			struct {
				float a, b, c,
					  d, e, f,
					  g, h, i ;
			};
			struct {
				float r1[3] ;
				float r2[3] ;
				float r3[3] ;
			};

			float mat[3][3] ;
			float array[9] ;
		};

		Mat3() :
			a(1), b(0), c(0),
			d(0), e(1), f(0),
			g(0), h(0), i(1)
		{}

		Mat3(float a, float b, float c,
			 float d, float e, float f,
			 float g, float h, float i)
			 : a(a), b(b), c(c),
			   d(d), e(e), f(f),
			   g(g), h(h), i(i) {}

		void Zero() {
			a = b = c = d = e = f = g = h = i = 0 ;
		}
	};

	struct Mat4 {
		union {
			struct {
				float m11, m12, m13, m14 ;
				float m21, m22, m23, m24 ;
				float m31, m32, m33, m34 ;
				float m41, m42, m43, m44 ;
			};
			struct {
				float a, b, c, d,
					  e, f, g, h, 
					  i, j, k, l,
					  m, n, o, p ;
			};
			struct {
				float r1[4] ;
				float r2[4] ;
				float r3[4] ;
				float r4[4] ;
			};
			float mat[4][4] ;
			float array[16] ;
		};

		Mat4()
		: a(1), b(0), c(0), d(0),
		  e(0), f(1), g(0), h(0),
		  i(0), j(0), k(1), l(0),
		  m(0), n(0), o(0), p(1) {};

		Mat4(float a, float b, float c, float d,
			 float e, float f, float g, float h,
			 float i, float j, float k, float l,
			 float m, float n, float o, float p)
		: a(a), b(b), c(c), d(d),
		  e(e), f(f), g(g), h(h),
		  i(i), j(j), k(k), l(l),
		  m(m), n(n), o(o), p(p) {};

		void Zero() {
			a = b = c = d = e = f = g = h = i = j = k = l = m = n = o = p = 0 ;
		}
	};
}

#define __TYPEDEF_GLOBAL_MATHS

#if defined __TYPEDEF_GLOBAL_MATHS

typedef math::Vec2 Vec2 ;
typedef math::Vec3 Vec3 ;
typedef math::Vec4 Vec4 ;

typedef math::Mat2 Mat2 ;
typedef math::Mat3 Mat3 ;
typedef math::Mat4 Mat4 ;

#endif
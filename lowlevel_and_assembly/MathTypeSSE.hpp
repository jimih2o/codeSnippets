#pragma once

namespace SSE
{
	struct Mat4 {
		union {
			struct {
				float   m0,   m1,  m2,  m3,
					m4,   m5,  m6,  m7,
					m8,   m9, m10, m11,
					m12, m13, m14, m15 ;
			};
			struct {
				float mR0[4] ;
				float mR1[4] ;
				float mR2[4] ;
				float mR3[4] ;
			};
			float mRC[4][4] ;
			float mFlat[16] ;
		};
	};

	struct Vec4 {
		union {
			struct {
				float x, y, z, w ;
			};
			struct {
				float r, g, b, a ;
			};
			float mFlat[4] ;
		};
	};

	static Vec4 const Zero = {0, 0, 0, 0} ;

	Vec4 operator*(Mat4 const &m, Vec4 const &v) ;
}
#include "MathTypeIntrinsic.hpp"

#include <intrin.h>

#define E(x, i) ((x)[i])

namespace Intrinsic
{
	// follows same logic as SSE::operator*(mat,vec)
	Vec4 operator*(Mat4 const &m, Vec4 const &v)
	{
		Vec4 ret(0.0) ;
		Mat4 trans = {
			Vec4(E(m[0],0), E(m[1],0), E(m[2],0), E(m[3],0)),
			Vec4(E(m[0],1), E(m[1],1), E(m[2],1), E(m[3],1)),
			Vec4(E(m[0],2), E(m[1],2), E(m[2],2), E(m[3],2)),
			Vec4(E(m[0],3), E(m[1],3), E(m[2],3), E(m[3],3))
		};

		ret += _mm_mul_ps(trans[0], Vec4(*(float *)&v)) ;
		ret += _mm_mul_ps(trans[1], Vec4(((float *)&v)[1])) ;
		ret += _mm_mul_ps(trans[2], Vec4(((float *)&v)[2])) ;
		ret += _mm_mul_ps(trans[3], Vec4(((float *)&v)[3])) ;

		return ret ;
	}
}

#undef E
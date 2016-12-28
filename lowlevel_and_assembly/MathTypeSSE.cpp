#include "MathTypeSSE.hpp"

namespace SSE
{
	/* Uses following logic:
		float v0 = m[0][0]*t[0] + m[0][1]*t[1] + m[0][2]*t[2] + m[0][3]*t[3];
		float v1 = m[1][0]*t[0] + m[1][1]*t[1] + m[1][2]*t[2] + m[1][3]*t[3];
		float v2 = m[2][0]*t[0] + m[2][1]*t[1] + m[2][2]*t[2] + m[2][3]*t[3];
		float v3 = m[3][0]*t[0] + m[3][1]*t[1] + m[3][2]*t[2] + m[3][3]*t[3];
		
		can be significantly reduced in operations, by loading m[x][0] to an xmm reg,
		and multiplying this by t[0], then following with similiar operations of m[x][n] * t[n]

		this is done using xmmN registers as follows:
		xmm0 =  t0 |  t0 |  t0 |  t0
		xmm1 = m00 | m10 | m20 | m30
		xmm1 *= xmm0 =>
		xmm1 = m00 * t0 | m10 * t0 | m20 * t0 | m30 * t0
		xmm2 += xmm1 =>
		xmm2 = m00 * t0 | m10 * t0 | m20 * t0 | m30 * t0
		... next iteration ...
		xmm2 += xmm1 =>
		xmm2 = m00 * t0 + m01 * t1 | m10 * t0  + m11 * t1 | m20 * t0 + m21 * t1 | m30 * t0 + m31 * t1
		... etc ...
		xmm2 = m * v
		then
		movups [outV], xmm2
	*/
	Vec4 operator*(Mat4 const &m, Vec4 const &v)
	{
		Vec4 outV ;
		Mat4 trans = {
			m.m0, m.m4, m.m8,  m.m12,
			m.m1, m.m5, m.m9,  m.m13,
			m.m2, m.m6, m.m10, m.m14,
			m.m3, m.m7, m.m11, m.m15
		};

		// xmm0 = v[n]
		// xmm1 = m[x][n]
		// xmm2 = res
		// eax = loader
		__asm {
			movups xmm2, [Zero]

			// load v[0] into xmm0
			mov eax, v
			movups xmm0, [eax]
			shufps xmm0, xmm0, 0x00


			// load r0 into xmm1
			movups xmm1, [trans.mR0]
			mulps xmm1, xmm0

			// add into xmm2
			addps xmm2, xmm1

			// load v[1] into xmm0
			movups xmm0, [eax]
			shufps xmm0, xmm0, 0x55


			// load r1 into xmm1
			movups xmm1, [trans.mR1]
			mulps xmm1, xmm0

			// add into xmm2
			addps xmm2, xmm1

			// load v[2] into xmm0
			movups xmm0, [eax]
			shufps xmm0, xmm0, 0xAA

				
			// load r2 into xmm1
			movups xmm1, [trans.mR2]
			mulps xmm1, xmm0

			// add into xmm2
			addps xmm2, xmm1

			// load v[3] into xmm0
			movups xmm0, [eax]
			shufps xmm0, xmm0, 0xFF

			// load r3 into xmm1
			movups xmm1, [trans.mR3]
			mulps xmm1, xmm0

			// add into xmm2
			addps xmm2, xmm1

			// load return value
			movups [outV], xmm2
		}

		return outV ;
	}
}
//-----------------------------------------------------------------------------------------------------------
/*
 *	LibX - A Library to export/import .x-models
 *
 *	This file is
 *		Copyright (C) 2008 Christian Oberholzer <coberholzer@gmx.ch>
 *
 *	It is part of LibX, which is
 *		Copyright (C) 2008 Christian Oberholzer <coberholzer@gmx.ch>
 *		Copyright (C) 2008 Basil Fierz <basil@gmx.ch>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 *	
 *	This library is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *	Lesser General Public License for more details.
 *	
 *	You should have received a copy of the GNU Lesser General Public
 *	License along with this library; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
//-----------------------------------------------------------------------------------------------------------

#ifndef GLOBAL_MATH_MATRIX4X4_H
#define GLOBAL_MATH_MATRIX4X4_H

#include "../global.h"
#include "./vector3.h"

namespace x { namespace math {

template <
	typename TypeT
>
class Matrix4x4
{
public:
	Matrix4x4() 
	{ 
	}

	inline Matrix4x4(
		const TypeT _11, const TypeT _12, const TypeT _13, const TypeT _14,
		const TypeT _21, const TypeT _22, const TypeT _23, const TypeT _24,
		const TypeT _31, const TypeT _32, const TypeT _33, const TypeT _34,
		const TypeT _41, const TypeT _42, const TypeT _43, const TypeT _44
	)
	{
		data.elements._11 = _11; data.elements._12 = _12; data.elements._13 = _13; data.elements._14 = _14;
		data.elements._21 = _21; data.elements._22 = _22; data.elements._23 = _23; data.elements._24 = _24;
		data.elements._31 = _31; data.elements._32 = _32; data.elements._33 = _33; data.elements._34 = _34;
		data.elements._41 = _41; data.elements._42 = _42; data.elements._43 = _43; data.elements._44 = _44;
	}


	void Transpose()
	{
		Matrix4x4<TypeT> Copy;
		memcpy(&Copy, this, sizeof(TypeT)*16);

		data.elements._11 = Copy.data.elements._11; data.elements._12 = Copy.data.elements._21;  data.elements._13 = Copy.data.elements._31; data.elements._14 = Copy.data.elements._41;
		data.elements._21 = Copy.data.elements._12; data.elements._22 = Copy.data.elements._22;  data.elements._23 = Copy.data.elements._32; data.elements._24 = Copy.data.elements._42;
		data.elements._31 = Copy.data.elements._13; data.elements._32 = Copy.data.elements._23;  data.elements._33 = Copy.data.elements._33; data.elements._34 = Copy.data.elements._43;
		data.elements._41 = Copy.data.elements._14; data.elements._42 = Copy.data.elements._24;  data.elements._43 = Copy.data.elements._34; data.elements._44 = Copy.data.elements._44;
	}

	void ToIdentity()
	{
		data.elements._11 = 1.0; data.elements._12 = 0.0;  data.elements._13 = 0.0; data.elements._14 = 0.0;
		data.elements._21 = 0.0; data.elements._22 = 1.0;  data.elements._23 = 0.0; data.elements._24 = 0.0;
		data.elements._31 = 0.0; data.elements._32 = 0.0;  data.elements._33 = 1.0; data.elements._34 = 0.0;
		data.elements._41 = 0.0; data.elements._42 = 0.0;  data.elements._43 = 0.0; data.elements._44 = 1.0;
	}

	static const Matrix4x4<TypeT>& IDENTITY()
	{
		static Matrix4x4<TypeT> mat;
		mat.ToIdentity();
		return mat;
	}

	inline void ToTranslate(const Vector3<TypeT>& v)
	{
		data.elements._11 = 1.0f; data.elements._12 = 0.0f; data.elements._13 = 0.0f; data.elements._14 = v.x;
		data.elements._21 = 0.0f; data.elements._22 = 1.0f; data.elements._23 = 0.0f; data.elements._24 = v.y;
		data.elements._31 = 0.0f; data.elements._32 = 0.0f; data.elements._33 = 1.0f; data.elements._34 = v.z;
		data.elements._41 = 0.0f; data.elements._42 = 0.0f; data.elements._43 = 0.0f; data.elements._44 = 1.0f;
	}

	inline void Mul(const Matrix4x4<TypeT>& mat1, const Matrix4x4<TypeT>& mat2)
	{
		data.elements._11 = (mat1.data.elements._11*mat2.data.elements._11 + mat1.data.elements._12*mat2.data.elements._21 + mat1.data.elements._13*mat2.data.elements._31 + mat1.data.elements._14*mat2.data.elements._41);
		data.elements._12 = (mat1.data.elements._11*mat2.data.elements._12 + mat1.data.elements._12*mat2.data.elements._22 + mat1.data.elements._13*mat2.data.elements._32 + mat1.data.elements._14*mat2.data.elements._42);
		data.elements._13 = (mat1.data.elements._11*mat2.data.elements._13 + mat1.data.elements._12*mat2.data.elements._23 + mat1.data.elements._13*mat2.data.elements._33 + mat1.data.elements._14*mat2.data.elements._43);
		data.elements._14 = (mat1.data.elements._11*mat2.data.elements._14 + mat1.data.elements._12*mat2.data.elements._24 + mat1.data.elements._13*mat2.data.elements._34 + mat1.data.elements._14*mat2.data.elements._44);

		data.elements._21 = (mat1.data.elements._21*mat2.data.elements._11 + mat1.data.elements._22*mat2.data.elements._21 + mat1.data.elements._23*mat2.data.elements._31 + mat1.data.elements._24*mat2.data.elements._41);
		data.elements._22 = (mat1.data.elements._21*mat2.data.elements._12 + mat1.data.elements._22*mat2.data.elements._22 + mat1.data.elements._23*mat2.data.elements._32 + mat1.data.elements._24*mat2.data.elements._42);
		data.elements._23 = (mat1.data.elements._21*mat2.data.elements._13 + mat1.data.elements._22*mat2.data.elements._23 + mat1.data.elements._23*mat2.data.elements._33 + mat1.data.elements._24*mat2.data.elements._43);
		data.elements._24 = (mat1.data.elements._21*mat2.data.elements._14 + mat1.data.elements._22*mat2.data.elements._24 + mat1.data.elements._23*mat2.data.elements._34 + mat1.data.elements._24*mat2.data.elements._44);

		data.elements._31 = (mat1.data.elements._31*mat2.data.elements._11 + mat1.data.elements._32*mat2.data.elements._21 + mat1.data.elements._33*mat2.data.elements._31 + mat1.data.elements._34*mat2.data.elements._41);
		data.elements._32 = (mat1.data.elements._31*mat2.data.elements._12 + mat1.data.elements._32*mat2.data.elements._22 + mat1.data.elements._33*mat2.data.elements._32 + mat1.data.elements._34*mat2.data.elements._42);
		data.elements._33 = (mat1.data.elements._31*mat2.data.elements._13 + mat1.data.elements._32*mat2.data.elements._23 + mat1.data.elements._33*mat2.data.elements._33 + mat1.data.elements._34*mat2.data.elements._43);
		data.elements._34 = (mat1.data.elements._31*mat2.data.elements._14 + mat1.data.elements._32*mat2.data.elements._24 + mat1.data.elements._33*mat2.data.elements._34 + mat1.data.elements._34*mat2.data.elements._44);

		data.elements._41 = (mat1.data.elements._41*mat2.data.elements._11 + mat1.data.elements._42*mat2.data.elements._21 + mat1.data.elements._43*mat2.data.elements._31 + mat1.data.elements._44*mat2.data.elements._41);
		data.elements._42 = (mat1.data.elements._41*mat2.data.elements._12 + mat1.data.elements._42*mat2.data.elements._22 + mat1.data.elements._43*mat2.data.elements._32 + mat1.data.elements._44*mat2.data.elements._42);
		data.elements._43 = (mat1.data.elements._41*mat2.data.elements._13 + mat1.data.elements._42*mat2.data.elements._23 + mat1.data.elements._43*mat2.data.elements._33 + mat1.data.elements._44*mat2.data.elements._43);
		data.elements._44 = (mat1.data.elements._41*mat2.data.elements._14 + mat1.data.elements._42*mat2.data.elements._24 + mat1.data.elements._43*mat2.data.elements._34 + mat1.data.elements._44*mat2.data.elements._44);
	}

	inline Matrix4x4<TypeT> operator * (const Matrix4x4<TypeT>& mat) const
	{
		Matrix4x4<TypeT> res;
		res.Mul(*this, mat);
		return res;
	}

	inline Vector3<TypeT> operator * (const Vector3<TypeT>& v) const
	{
		Vector3f res;
		res.x = data.elements._11 * v.x + data.elements._12 * v.y + data.elements._13 * v.z + data.elements._14;
		res.y = data.elements._21 * v.x + data.elements._22 * v.y + data.elements._23 * v.z + data.elements._24;
		res.z = data.elements._31 * v.x + data.elements._32 * v.y + data.elements._33 * v.z + data.elements._34;
		return res;
	}

	Matrix4x4<TypeT> GetInverse() const
	{
		TypeT fA0 = data.array1d[ 0]*data.array1d[ 5] - data.array1d[ 1]*data.array1d[ 4];
		TypeT fA1 = data.array1d[ 0]*data.array1d[ 6] - data.array1d[ 2]*data.array1d[ 4];
		TypeT fA2 = data.array1d[ 0]*data.array1d[ 7] - data.array1d[ 3]*data.array1d[ 4];
		TypeT fA3 = data.array1d[ 1]*data.array1d[ 6] - data.array1d[ 2]*data.array1d[ 5];
		TypeT fA4 = data.array1d[ 1]*data.array1d[ 7] - data.array1d[ 3]*data.array1d[ 5];
		TypeT fA5 = data.array1d[ 2]*data.array1d[ 7] - data.array1d[ 3]*data.array1d[ 6];
		TypeT fB0 = data.array1d[ 8]*data.array1d[13] - data.array1d[ 9]*data.array1d[12];
		TypeT fB1 = data.array1d[ 8]*data.array1d[14] - data.array1d[10]*data.array1d[12];
		TypeT fB2 = data.array1d[ 8]*data.array1d[15] - data.array1d[11]*data.array1d[12];
		TypeT fB3 = data.array1d[ 9]*data.array1d[14] - data.array1d[10]*data.array1d[13];
		TypeT fB4 = data.array1d[ 9]*data.array1d[15] - data.array1d[11]*data.array1d[13];
		TypeT fB5 = data.array1d[10]*data.array1d[15] - data.array1d[11]*data.array1d[14];

		TypeT fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
		if ((TypeT)fabs(fDet) <= 0.00000001)
		{
			Matrix4x4<TypeT> zero;
			memset(&zero, 0, sizeof(TypeT)*16);
			return zero;
		}

		Matrix4x4<TypeT> matInv;
		matInv.data.array2d[0][0] = + data.array1d[ 5]*fB5 - data.array1d[ 6]*fB4 + data.array1d[ 7]*fB3;
		matInv.data.array2d[1][0] = - data.array1d[ 4]*fB5 + data.array1d[ 6]*fB2 - data.array1d[ 7]*fB1;
		matInv.data.array2d[2][0] = + data.array1d[ 4]*fB4 - data.array1d[ 5]*fB2 + data.array1d[ 7]*fB0;
		matInv.data.array2d[3][0] = - data.array1d[ 4]*fB3 + data.array1d[ 5]*fB1 - data.array1d[ 6]*fB0;
		matInv.data.array2d[0][1] = - data.array1d[ 1]*fB5 + data.array1d[ 2]*fB4 - data.array1d[ 3]*fB3;
		matInv.data.array2d[1][1] = + data.array1d[ 0]*fB5 - data.array1d[ 2]*fB2 + data.array1d[ 3]*fB1;
		matInv.data.array2d[2][1] = - data.array1d[ 0]*fB4 + data.array1d[ 1]*fB2 - data.array1d[ 3]*fB0;
		matInv.data.array2d[3][1] = + data.array1d[ 0]*fB3 - data.array1d[ 1]*fB1 + data.array1d[ 2]*fB0;
		matInv.data.array2d[0][2] = + data.array1d[13]*fA5 - data.array1d[14]*fA4 + data.array1d[15]*fA3;
		matInv.data.array2d[1][2] = - data.array1d[12]*fA5 + data.array1d[14]*fA2 - data.array1d[15]*fA1;
		matInv.data.array2d[2][2] = + data.array1d[12]*fA4 - data.array1d[13]*fA2 + data.array1d[15]*fA0;
		matInv.data.array2d[3][2] = - data.array1d[12]*fA3 + data.array1d[13]*fA1 - data.array1d[14]*fA0;
		matInv.data.array2d[0][3] = - data.array1d[ 9]*fA5 + data.array1d[10]*fA4 - data.array1d[11]*fA3;
		matInv.data.array2d[1][3] = + data.array1d[ 8]*fA5 - data.array1d[10]*fA2 + data.array1d[11]*fA1;
		matInv.data.array2d[2][3] = - data.array1d[ 8]*fA4 + data.array1d[ 9]*fA2 - data.array1d[11]*fA0;
		matInv.data.array2d[3][3] = + data.array1d[ 8]*fA3 - data.array1d[ 9]*fA1 + data.array1d[10]*fA0;

		TypeT fInvDet = ((TypeT)1.0)/fDet;
		for (uint_t iRow = 0; iRow < 4; iRow++)
		{
			for (uint_t iCol = 0; iCol < 4; iCol++)
				matInv.data.array2d[iRow][iCol] *= fInvDet;
		}

		return matInv;
	}

public:
	union
	{
		TypeT array1d[16];
		TypeT array2d[4][4];
		struct
		{
			TypeT _11, _12, _13, _14;
			TypeT _21, _22, _23, _24;
			TypeT _31, _32, _33, _34;
			TypeT _41, _42, _43, _44;
		} elements;
	} data;
};

typedef Matrix4x4<float_t> Matrix4x4f;
typedef Matrix4x4<double_t> Matrix4x4d;

}} // namespace x::math

#endif // GLOBAL_MATH_MATRIX4X4_H

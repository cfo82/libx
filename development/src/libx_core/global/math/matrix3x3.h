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

#ifndef GLOBAL_MATH_MATRIX3X3_H
#define GLOBAL_MATH_MATRIX3X3_H

#include "../global.h"
#include "./matrix4x4.h"

namespace x { namespace math {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Matrix3x3 class
*/
template <
	typename TypeT
>
class Matrix3x3
{
public:
	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief default constructor initialises the matrix to an identity matrix.
	*/
	Matrix3x3() 
	{
		ToIdentity();
	}
	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief transpose operation
	*/
	void Transpose()
	{
		Matrix3x3<TypeT> copy;
		memcpy(&copy, this, sizeof(TypeT)*9);

		data.elements._11 = copy.data.elements._11; data.elements._12 = copy.data.elements._21;  data.elements._13 = copy.data.elements._31;
		data.elements._21 = copy.data.elements._12; data.elements._22 = copy.data.elements._22;  data.elements._23 = copy.data.elements._32;
		data.elements._31 = copy.data.elements._13; data.elements._32 = copy.data.elements._23;  data.elements._33 = copy.data.elements._33;
	}
	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief reset this matrix to an identity matrix
	*/
	void ToIdentity()
	{
		data.elements._11 = 1.0;data.elements. _12 = 0.0; data.elements. _13 = 0.0;
		data.elements._21 = 0.0; data.elements._22 = 1.0;  data.elements._23 = 0.0;
		data.elements._31 = 0.0; data.elements._32 = 0.0;  data.elements._33 = 1.0;
	}
	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief multiplies this matrix with the given matrix.
		\param rhs
		\return multiplication result
	*/
	inline Matrix3x3<TypeT> operator * (
		const Matrix3x3<TypeT>& rhs
	)
	{
		Matrix3x3<TypeT> retval;
		retval.Mul(*this, rhs);
		return retval;
	}
	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief multiplies two matrices
		\param lhs left hand side operator
		\param rhs right hand side operator
		
		multiplies lhs and rhs and assigns the result to this.
	*/
	inline void Mul(
		const Matrix3x3<TypeT>& lhs, 
		const Matrix3x3<TypeT>& rhs
	)
	{
		data.elements._11 = lhs.data.elements._11 * rhs.data.elements._11 + lhs.data.elements._12 * rhs.data.elements._21 + lhs.data.elements._13 * rhs.data.elements._31;
		data.elements._12 = lhs.data.elements._11 * rhs.data.elements._12 + lhs.data.elements._12 * rhs.data.elements._22 + lhs.data.elements._13 * rhs.data.elements._32;
		data.elements._13 = lhs.data.elements._11 * rhs.data.elements._13 + lhs.data.elements._12 * rhs.data.elements._23 + lhs.data.elements._13 * rhs.data.elements._33;

		data.elements._21 = lhs.data.elements._21 * rhs.data.elements._11 + lhs.data.elements._22 * rhs.data.elements._21 + lhs.data.elements._23 * rhs.data.elements._31;
		data.elements._22 = lhs.data.elements._21 * rhs.data.elements._12 + lhs.data.elements._22 * rhs.data.elements._22 + lhs.data.elements._23 * rhs.data.elements._32;
		data.elements._23 = lhs.data.elements._21 * rhs.data.elements._13 + lhs.data.elements._22 * rhs.data.elements._23 + lhs.data.elements._23 * rhs.data.elements._33;

		data.elements._31 = lhs.data.elements._31 * rhs.data.elements._11 + lhs.data.elements._32 * rhs.data.elements._21 + lhs.data.elements._33 * rhs.data.elements._31;
		data.elements._32 = lhs.data.elements._31 * rhs.data.elements._12 + lhs.data.elements._32 * rhs.data.elements._22 + lhs.data.elements._33 * rhs.data.elements._32;
		data.elements._33 = lhs.data.elements._31 * rhs.data.elements._13 + lhs.data.elements._32 * rhs.data.elements._23 + lhs.data.elements._33 * rhs.data.elements._33;
	}
	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief returns an identity 3x3 matrix.
		\return identity matrix
	*/
	static const Matrix3x3<TypeT>& IDENTITY()
	{
		static Matrix3x3<TypeT> mat;
		mat.ToIdentity();
		return mat;
	}
	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief zeroes out the matrix
	*/
	inline void ToZero()
	{
		data.elements._11 = data.elements._12 = data.elements._13 = 0.0f;
		data.elements._21 = data.elements._22 = data.elements._23 = 0.0f;
		data.elements._31 = data.elements._32 = data.elements._33 = 0.0f;
	}
	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief creates a scale-matrix.
		\param factor scale-factor
		
		creates a scale-matrix with uniform scaling.
	*/
	inline void ToScale(
		TypeT factor
	)
	{
		ToZero();

		data.elements._11 = factor;
		data.elements._22 = factor;
		data.elements._33 = factor;
	}
	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief creates a scale-matrix.
		\param v scaling vector
		
		creates a scale-matrix with potential non-uniform scaling. scale-factors
		along x/y/z axes are given by v.
	*/
	inline void ToScale(
		const Vector3<TypeT>& v
	)
	{
		ToZero();

		data.elements._11 = v.x;
		data.elements._22 = v.y;
		data.elements._33 = v.z;
	}
	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief implicit conversion to a 4x4 matrix.
		\return 4x4 matrix representation of this matrix.
		
		this operator is used for more comfort. object transformations in 3d space
		are usually done using 3x3 matrices (easier to handle and faster). But to 
		multiply it with object-to-screen-space transformations a 4x4 matrix is needed.
		This conversion is implicitly done using this cast-operator.
	*/
	inline Matrix4x4<TypeT> ToMatrix4x4()
	{
		Matrix4x4<TypeT> mat;
		mat.data.elements._11 = data.elements._11;	mat.data.elements._12 = data.elements._12;	mat.data.elements._13 = data.elements._13;	mat.data.elements._14 = 0.0;
		mat.data.elements._21 = data.elements._21;	mat.data.elements._22 = data.elements._22;	mat.data.elements._23 = data.elements._23;	mat.data.elements._24 = 0.0;
		mat.data.elements._31 = data.elements._31;	mat.data.elements._32 = data.elements._32;	mat.data.elements._33 = data.elements._33;	mat.data.elements._34 = 0.0;
		mat.data.elements._41 = 0.0;	mat.data.elements._42 = 0.0;	mat.data.elements._43 = 0.0;	mat.data.elements._44 = 1.0;
		return mat;
	}

public:
	union
	{
		//! data as a single array
		TypeT array1d[9];
		//! data as a two-dimensional array with [rows][cols]
		TypeT array2d[3][3];
		struct
		{
			TypeT _11, _12, _13;
			TypeT _21, _22, _23;
			TypeT _31, _32, _33;
		} elements;
	} data;
};

/// typedef for matrices with single precision
typedef Matrix3x3<float_t> Matrix3x3f;
/// typedef for matrices with double precision
typedef Matrix3x3<double_t> Matrix3x3d;

}} // namespace x::math

#endif // GLOBAL_MATH_MATRIX3X3_H

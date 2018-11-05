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

#ifndef GLOBAL_MATH_VECTOR2_H
#define GLOBAL_MATH_VECTOR2_H

#include <assert.h>
#include "../global.h"

namespace x { namespace math {

template <
	typename TypeT
>
struct Vector2
{
	inline TypeT& operator[] (const uint_t index)
	{
		assert(index >= 0 && index <= 1);
		switch (index)
		{
		case 0: return x;
		case 1: return y;
		default: return x;
		}
	}

	inline const TypeT& operator[] (const uint_t index) const
	{
		assert(index >= 0 && index <= 1);
		switch (index) 
		{
		case 0:	return x;
		case 1: return y;
		default: return x;
		}
	}

	TypeT x;
	TypeT y;
};

typedef Vector2<float_t> Vector2f;
typedef Vector2<double_t> Vector2d;

}} // namespace x::math

#endif // GLOBAL_MATH_VECTOR2_H

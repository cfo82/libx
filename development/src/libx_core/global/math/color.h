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

#ifndef GLOBAL_MATH_COLOR_H
#define GLOBAL_MATH_COLOR_H

#include "../global.h"

namespace x { namespace math {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief color class
*/
template <
	typename TypeT
>
struct Color
{
	TypeT a; //!< alpha channel
	TypeT r; //!< red channel
	TypeT g; //!< green channel
	TypeT b; //!< blue channel
};

/// typedef for colors with single precision
typedef Color<float_t> Colorf;
/// typedef for colors with double precision
typedef Color<double_t> Colord;

}} // namespace x::math

#endif // GLOBAL_MATH_COLOR_H

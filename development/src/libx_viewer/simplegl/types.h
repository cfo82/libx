//-----------------------------------------------------------------------------------------------------------
/*
 *	LibX - A Library to export/import .x-models
 *
 *	This file is
 *		Copyright (C) 2008 Basil Fierz <basil@gmx.ch>
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

#ifndef SIMPLEGL_TYPES_H
#define SIMPLEGL_TYPES_H

#include "../../libx_core/global/math/color.h"
#include "../../libx_core/global/math/matrix3x3.h"
#include "../../libx_core/global/math/matrix4x4.h"
#include "../../libx_core/global/math/quaternion.h"
#include "../../libx_core/global/math/vector2.h"
#include "../../libx_core/global/math/vector3.h"

namespace simplegl {
	typedef x::math::Colorf Colourf;
	typedef x::math::Matrix3x3f Matrix3x3f;
	typedef x::math::Matrix4x4f Matrix4x4f;
	typedef x::math::Quaternionf Quaternionf;
	typedef x::math::Vector2f Vector2f;
	typedef x::math::Vector2<int> Vector2i;
	typedef x::math::Vector3f Vector3f;
};

#endif /* SIMPLEGL_TYPES_H */
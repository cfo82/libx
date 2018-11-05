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

#ifndef SIMPLEGL_CAMERA_H
#define SIMPLEGL_CAMERA_H

#include "system.h"

#include "types.h"

namespace simplegl {

class Camera
{
public:
	Camera();
	~Camera();
	
public:
	void SetPosition(float x, float y, float z);
	void SetCenter(float x, float y, float z);
	void SetUp(float x, float y, float z);

	void SetFovY(float fovy);
	void SetRatio(float ratio);
	void SetNearPlane(float near_plane);
	void SetFarPlane(float far_plane);

	void ApplyModelView();
	void ApplyProjection();

private:
	Vector3f position_;
	Vector3f center_;
	Vector3f up_;

	float fovy_;
	float ratio_;
	float near_plane_, far_plane_;
};

};

#endif /* SIMPLEGL_CAMERA_H */
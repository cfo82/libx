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

#include "camera.h"

namespace simplegl {

	Camera::Camera()
	{
		SetPosition(1.0f, 1.0f, 1.0f);
		SetCenter(0.0f, 0.0f, 0.0f);
		SetUp(0.0f, 1.0f, 0.0f);

		SetFovY(45.0f);
		SetRatio(1.0f);
		SetNearPlane(1.0f);
		SetFarPlane(100.0f);
	}

	Camera::~Camera()
	{
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		position_.x = x;
		position_.y = y;
		position_.z = z;
	}

	void Camera::SetCenter(float x, float y, float z)
	{
		center_.x = x;
		center_.y = y;
		center_.z = z;
	}

	void Camera::SetUp(float x, float y, float z)
	{
		up_.x = x;
		up_.y = y;
		up_.z = z;
	}

	void Camera::SetFovY(float fovy)
	{
		fovy_ = fovy;
	}

	void Camera::SetRatio(float ratio)
	{
		ratio_ = ratio;
	}

	void Camera::SetNearPlane(float near_plane)
	{
		near_plane_ = near_plane;
	}

	void Camera::SetFarPlane(float far_plane)
	{
		far_plane_ = far_plane;
	}

	void Camera::ApplyModelView()
	{
		gluLookAt(	position_.x, position_.y, position_.z,
					center_.x, center_.y, center_.z,
					up_.x, up_.y, up_.z);
	}

	void Camera::ApplyProjection()
	{
		gluPerspective(fovy_, ratio_, near_plane_, far_plane_);
	}
};
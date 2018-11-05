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

#ifndef WORLD_H
#define WORLD_H

#include "simplegl/camera.h"

#include "light.h"

class World
{
public:
	World();
	~World();

public:
	inline simplegl::Camera& GetCamera() { return camera_; }
	inline DirectLight& GetSunLight() { return sun_light_; }
	inline void GetSunLight(DirectLight& sun_light) const { sun_light = sun_light_; }
	inline float GetTime() const { return time_; }

	inline void UpdateTime(float delta_time) { time_ += delta_time; }

	inline const simplegl::Matrix4x4f& GetWorldTransform() const { return world_transform_; }
	inline void SetWorldTransform(const simplegl::Matrix4x4f& transform) { world_transform_ = transform; }

private:
	simplegl::Camera camera_;
	DirectLight sun_light_;

	float time_;

	simplegl::Matrix4x4f world_transform_;
};

#endif /* WORLD_H */
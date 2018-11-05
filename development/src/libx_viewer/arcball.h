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

#ifndef ARCBALL_H
#define ARCBALL_H

#include "simplegl/system.h"
#include "simplegl/types.h"

class ArcBall
{
public:
	//Create/Destroy
	ArcBall( float width, float height );
	~ArcBall() { /* nothing to do */ };

	//Set new bounds
	void SetBounds(float newWidth, float newHeight);

	//Mouse down
	void MouseDown(const int x, const int y);
	void MouseMove(const int x, const int y);
	void MouseUp();

	void ApplyRotation(simplegl::Matrix4x4f& rotation_matrix);

private:
	simplegl::Vector3f Project(const simplegl::Vector2i& position);
	simplegl::Quaternionf FromPositon(const simplegl::Vector3f& v);

private:
	static const int radius = 1;

protected:
	float window_width_;    //Mouse bounds width
	float window_height_;   //Mouse bounds height
	simplegl::Vector3f last_position_;
	simplegl::Quaternionf last_quat_;
	bool rotate_;
};

#endif /* ARCBALL_H */


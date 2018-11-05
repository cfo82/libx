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

#include "arcball.h"

using namespace simplegl;

ArcBall::ArcBall(
	float width,
	float height
)
{
	//Clear initial values
	last_position_ = Vector3f::ZERO();
	last_quat_ = Quaternionf::IDENTITY();
	rotate_ = false;

	//Set initial bounds
	SetBounds(width, height);
}

void ArcBall::SetBounds(float newWidth, float newHeight)
{
    //Set adjustment factor for width/height
    window_width_  = newWidth;
    window_height_ = newHeight;
}

//Mouse down
void ArcBall::MouseDown(
	const int x,
	const int y
)
{
	rotate_ = true;
	Vector2i pos = {x, y};
	last_position_ = Project(pos);
}

//Mouse drag, calculate rotation
void ArcBall::MouseMove(
	const int x,
	const int y
)
{
	if (!rotate_) return;

	Vector2i pos = {x, y};
	Vector3f newPos = Project(pos);
	Quaternionf newQuat = FromPositon(newPos);

	last_position_ = newPos;
	last_quat_ = newQuat * last_quat_;
	last_quat_.Normalize();
}

void ArcBall::MouseUp()
{
	rotate_ = false;
}

void ArcBall::ApplyRotation(simplegl::Matrix4x4f& rotation_matrix)
{
	x::math::Matrix3x3f inverse_rotation_matrix;
	last_quat_.ToRotationMatrix(inverse_rotation_matrix);
	
	rotation_matrix.data.elements._11 = inverse_rotation_matrix.data.elements._11;
	rotation_matrix.data.elements._12 = inverse_rotation_matrix.data.elements._21;
	rotation_matrix.data.elements._13 = inverse_rotation_matrix.data.elements._31;
	rotation_matrix.data.elements._14 = 0.0f;

	rotation_matrix.data.elements._21 = inverse_rotation_matrix.data.elements._12;
	rotation_matrix.data.elements._22 = inverse_rotation_matrix.data.elements._22;
	rotation_matrix.data.elements._23 = inverse_rotation_matrix.data.elements._32;
	rotation_matrix.data.elements._24 = 0.0f;

	rotation_matrix.data.elements._31 = inverse_rotation_matrix.data.elements._13;
	rotation_matrix.data.elements._32 = inverse_rotation_matrix.data.elements._23;
	rotation_matrix.data.elements._33 = inverse_rotation_matrix.data.elements._33;
	rotation_matrix.data.elements._34 = 0.0f;

	rotation_matrix.data.elements._41 = 0.0f;
	rotation_matrix.data.elements._42 = 0.0f;
	rotation_matrix.data.elements._43 = 0.0f;
	rotation_matrix.data.elements._44 = 1.0f;

	glMultMatrixf(rotation_matrix.data.array1d);
}

Vector3f ArcBall::Project(
	const Vector2i& position
)
{
	float x, y;
	Vector3f projection = Vector3f::ZERO();

	// Scale to [0,0] - [2,2]
	x = position.x / (window_width_ / 2.0f);
	y = position.y / (window_height_ / 2.0f);

	// Translate 0,0 to center
	x = x - 1;
	y = y - 1;

	if (x > 1.0f) x = 1.0f;
	if (x < -1.0f) x = -1.0f;
	if (y > 1.0f) y = 1.0f;
	if (y < -1.0f) y = -1.0f;

	projection.x = x;
	projection.y = y;
	float d = (float)sqrtf(x * x + y * y);
	if (d < radius * 0.70710678118654752440f)
	{
		projection.z = (float)sqrtf(radius * radius - d * d);
	}
	else
	{
		float t = radius * 0.70710678118654752440f;
		projection.z = t * t / d;
	}

	return projection;
}

simplegl::Quaternionf ArcBall::FromPositon(
	const simplegl::Vector3f& v
)
{
	float phi;

	if (
		v.x == last_position_.x && 
		v.y == last_position_.y && 
		v.z == last_position_.z 
	)
	{
		return Quaternionf::IDENTITY();
	}

	Vector3f diff;
	diff.x = v.x - last_position_.x;
	diff.y = v.y - last_position_.y;
	diff.z = v.z - last_position_.z;
	phi = diff.GetLen() / (2.0f * radius);
	if (phi > 1.0f) phi = 1.0f;
	if (phi < -1.0f) phi = -1.0f;
	phi = 2.0f * (float)asinf(phi);

	Vector3f tmp = last_position_.Cross(v);
	tmp.Normalize();
	float scalar = (float)sinf(phi / 2.0f);
	tmp.x = tmp.x * scalar;
	tmp.y = tmp.y * scalar;
	tmp.z = tmp.z * scalar;

	return Quaternionf((float)cosf(phi / 2.0f), tmp.x, tmp.y, tmp.z);
}
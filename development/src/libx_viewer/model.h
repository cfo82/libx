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

#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <stack>
#include <vector>

#include <float.h>
#include <math.h>

#include "../libx_model/model/keyframeanimation/animationcontroller.h"
#include "../libx_model/xmeshgenerator.h"

#include "simplegl/shader.h"
#include "simplegl/system.h"
#include "simplegl/types.h"
#include "simplegl/texture.h"

#include "modelshaders.h"
#include "world.h"

class Model
{
private:
	static x::Interface* x_interface_;

public:
	static void CreateXInterface();
	static void DeleteXInterface();
	static Model* CreateFromFile(const char *filename);

private:
	Model();
	void PostInit();

public:
	~Model();

	void GetAABB(simplegl::Vector3f& min_pos, simplegl::Vector3f& max_pos);

	void Render(const World &world);
	void RenderNormals(const World &world);
	void RenderSkeleton(const World &world);
	void RenderBox(const World &world);

	void UpdatePose(float current_time);

	x::string_t GetAnimationName(int i) const;
	int GetAnimationCount() const;

	void SetAnimation(int i);

private:
	void LoadMaterials(x::model::Frame* frame);
	void BuildBoundingBox(x::model::Frame* frame);
	void GenerateNormals(x::model::Frame* frame);

	void RenderFrame(x::model::Frame* frame, const World &world);
	void RenderFrameNormals(x::model::Frame* frame, const World &world);
	void RenderSkeleton(const x::model::Bone* parent_bone, const x::model::Bone* bone);

private:
	GLint GetSizeFromType(x::model::ValueType type);
	GLenum ConvertPrimitiveType(x::model::PrimitiveType type);
	GLenum ConvertIndexSize(x::uint_t size);

private:
	x::string_t base_path_;
	x::model::Model* model_;

	x::math::Vector3f min_pos_, max_pos_;

	std::map<x::string_t, simplegl::Texture*> texture_maps_;

	x::model::AnimationController* animation_controller_;

private: /* Shaders */
	BoneShader bone_shader_;
	ModelShader model_shader_;
};

#endif /* MODEL_H */
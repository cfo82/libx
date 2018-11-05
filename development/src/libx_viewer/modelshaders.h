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

#ifndef MODELSHADERS_H
#define MODELSHADERS_H

#include "simplegl/application.h"
#include "simplegl/shader.h"
#include "simplegl/shadercore.h"
#include "simplegl/system.h"

struct ModelShader
{
	ModelShader() : VertexShader(NULL), FSDecalShader(NULL), FSNoDecalShader(NULL), FSNoDecalNormalsShader(NULL)
	{
	}

	simplegl::Shader* VertexShader;
	simplegl::Shader* FSDecalShader;
	simplegl::Shader* FSNoDecalShader;
	simplegl::Shader* FSNoDecalNormalsShader;

	// Cg shader parameters
	CGparameter ConstVSModelViewProjectionMatrix;
	CGparameter ConstVSInverseWorldMatrix;
	CGparameter ConstVSLightDirection;
	CGparameter ConstVSLightDiffuseColour;

	CGparameter ConstVSMaterialAmbient;
	CGparameter ConstVSMaterialDiffuse;
	CGparameter ConstVSMaterialSpecular;
	CGparameter ConstVSMaterialEmissive;

	CGparameter ConstFSTextureDiffuse;
};

void CreateModelShaders(ModelShader& shader_desc);
void DeleteModelShaders(ModelShader& shader_desc);

struct BoneShader
{
	BoneShader() : VertexShader(NULL), FragmentShader(NULL)
	{
	}

	simplegl::Shader* VertexShader;
	simplegl::Shader* FragmentShader;

	CGparameter ConstVSViewProjMatrix;
	CGparameter ConstVSWorldMatrix;
	CGparameter ConstVSColour;
};

void CreateBoneShaders(BoneShader& shader_desc);
void DeleteBoneShaders(BoneShader& shader_desc);

#endif /* MODELSHADERS_H */
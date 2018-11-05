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

#include "modelshaders.h"

void CreateModelShaders(ModelShader& shader_desc)
{
	simplegl::ShaderCore* shader_core = simplegl::Application::Current()->GetShaderCore();

	shader_desc.VertexShader = shader_core->CreateVertexShader("./shaders/diffuse_vs.cg", "main");
	shader_desc.FSDecalShader = shader_core->CreateFragmentShader("./shaders/diffuse_fs.cg", "decal");
	shader_desc.FSNoDecalShader = shader_core->CreateFragmentShader("./shaders/diffuse_fs.cg", "no_decal");
	shader_desc.FSNoDecalNormalsShader = shader_core->CreateFragmentShader("./shaders/diffuse_fs.cg", "no_decal_normals");

	shader_desc.ConstVSMaterialAmbient = NULL;
	shader_desc.ConstVSMaterialDiffuse = shader_desc.VertexShader->GetNamedParameter("materialDiffuse");
	shader_desc.ConstVSMaterialSpecular = NULL;
	shader_desc.ConstVSMaterialEmissive = NULL;
	shader_desc.ConstFSTextureDiffuse = shader_desc.FSDecalShader->GetNamedParameter("texture");

	shader_desc.ConstVSModelViewProjectionMatrix = shader_desc.VertexShader->GetNamedParameter("modelViewProj");
	shader_desc.ConstVSInverseWorldMatrix = shader_desc.VertexShader->GetNamedParameter("worldInvTrans");
	shader_desc.ConstVSLightDirection = shader_desc.VertexShader->GetNamedParameter("lightDirection");
	shader_desc.ConstVSLightDiffuseColour = shader_desc.VertexShader->GetNamedParameter("lightDiffuse");
}

void DeleteModelShaders(ModelShader& shader_desc)
{
	delete shader_desc.VertexShader;
	delete shader_desc.FSDecalShader;
	delete shader_desc.FSNoDecalShader;
	delete shader_desc.FSNoDecalNormalsShader;
}

void CreateBoneShaders(BoneShader& shader_desc)
{
	simplegl::ShaderCore* shader_core = simplegl::Application::Current()->GetShaderCore();

	shader_desc.VertexShader = shader_core->CreateVertexShader("./shaders/bones.cg", "vs_main");
	shader_desc.FragmentShader = shader_core->CreateFragmentShader("./shaders/bones.cg", "fs_main");

	shader_desc.ConstVSViewProjMatrix = shader_desc.VertexShader->GetNamedParameter("viewProjMatrix");
	shader_desc.ConstVSWorldMatrix = shader_desc.VertexShader->GetNamedParameter("worldMatrix");
	shader_desc.ConstVSColour = shader_desc.VertexShader->GetNamedParameter("colour");
}

void DeleteBoneShaders(BoneShader& shader_desc)
{
	delete shader_desc.VertexShader;
	delete shader_desc.FragmentShader;
}
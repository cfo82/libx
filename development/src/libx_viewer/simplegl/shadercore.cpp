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

#include "shadercore.h"

#include "stdlib.h"
#include "stdio.h"

namespace simplegl {

inline void CheckCGError(const char* situation)
{
#ifdef _DEBUG
	CGerror error;
	const char *string = cgGetLastErrorString(&error);

	if (error != CG_NO_ERROR)
	{
		printf("%s: %s\n", situation, string);
		exit(1);
	}
#endif
}

ShaderCore::ShaderCore()
{
	context_ = cgCreateContext();
	CheckCGError("Creating the CG context");

	vertex_shader_profile_ = cgGLGetLatestProfile(CG_GL_VERTEX);
	CheckCGError("Querying the latest vertex shader profile");

	fragment_shader_profile_ = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	CheckCGError("Querying the latest fragment shader profile");

	// Init of automatic texture management
	cgGLSetManageTextureParameters(context_, CG_TRUE);
}

ShaderCore::~ShaderCore()
{
	cgDestroyContext(context_);
}

CGcontext ShaderCore::GetCGContext()
{
	return context_;
}

CGprofile ShaderCore::GetCGVertexShaderProfile()
{
	return vertex_shader_profile_;
}

CGprofile ShaderCore::GetCGFragmentShaderProfile()
{
	return fragment_shader_profile_;
}

Shader* ShaderCore::CreateVertexShader(char* program_name, char* entry_point)
{
	return new Shader(*this, program_name, vertex_shader_profile_, entry_point);
}

Shader* ShaderCore::CreateFragmentShader(char* program_name, char* entry_point)
{
	return new Shader(*this, program_name, fragment_shader_profile_, entry_point);
}

void ShaderCore::Enable()
{
	if (vertex_shader_profile_ != CG_PROFILE_UNKNOWN)
	{
		cgGLEnableProfile(vertex_shader_profile_);
		CheckCGError("Enabling the vertex shader profile");
	}

	if (fragment_shader_profile_ != CG_PROFILE_UNKNOWN)
	{
		cgGLEnableProfile(fragment_shader_profile_);
		CheckCGError("Enabling the fragment shader profile");
	}
}

void ShaderCore::Disable()
{
	if (vertex_shader_profile_ != CG_PROFILE_UNKNOWN)
	{
		cgGLDisableProfile(vertex_shader_profile_);
		CheckCGError("Disabling the vertex shader profile");
	}

	if (fragment_shader_profile_ != CG_PROFILE_UNKNOWN)
	{
		cgGLDisableProfile(fragment_shader_profile_);
		CheckCGError("Disabling the fragment shader profile");
	}
}

void ShaderCore::SetTexture(CGparameter sampler, Texture* tex)
{
	cgGLSetTextureParameter(sampler, tex->GetID());
}

}
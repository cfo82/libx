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

#ifndef SIMPLEGL_SHADERCORE_H
#define SIMPLEGL_SHADERCORE_H

#include "system.h"

#include "shader.h"
#include "texture.h"

namespace simplegl {

class ShaderCore
{
	friend class Application;
private:
	ShaderCore();

public:
	~ShaderCore();

public:
	CGcontext GetCGContext();
	CGprofile GetCGVertexShaderProfile();
	CGprofile GetCGFragmentShaderProfile();

	Shader* CreateVertexShader(char* program_name, char* entry_point);
	Shader* CreateFragmentShader(char* program_name, char* entry_point);

	void Enable();
	void Disable();

	void SetTexture(CGparameter sampler, Texture* tex);

private:
	CGcontext context_;
	CGprofile vertex_shader_profile_;
	CGprofile fragment_shader_profile_;
};

};

#endif /* SIMPLEGL_SHADERCORE_H */
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

#include "shader.h"
#include "shadercore.h"

#include "stdlib.h"
#include "stdio.h"

namespace simplegl {

inline void CheckCGCompilerError(CGcontext context, const char* situation)
{
#ifdef _DEBUG
	CGerror error;
	const char *string = cgGetLastErrorString(&error);

	if (error != CG_NO_ERROR)
	{
		printf("%s: %s\n", situation, string);
		if (error == CG_COMPILER_ERROR)
		{
			printf("%s\n", cgGetLastListing(context));
		}
		exit(1);
	}
#endif
}

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

Shader::Shader(ShaderCore& core, char* program_name, CGprofile profile, char* entry_point)
{
	profile_ = profile;

	if (profile_ != CG_PROFILE_UNKNOWN)
	{
		cgGLSetOptimalOptions(profile_);
		CheckCGError("Setting the optimal shader parameters.");
	}

	program_ = cgCreateProgramFromFile(	core.GetCGContext(),
										CG_SOURCE,
										program_name,
										profile_,
										entry_point,
										0);
	CheckCGCompilerError(core.GetCGContext(), "Compiling shader program.");

	cgGLLoadProgram(program_);
	CheckCGError("Loading the shader program.");
}

Shader::~Shader()
{
	Unbind();
	cgDestroyProgram(program_);
	CheckCGError("Destroying the shader program.");
}

CGprogram Shader::GetCGProgram()
{
	return program_;
}

CGprofile Shader::GetCGProfile()
{
	return profile_;
}

void Shader::Bind()
{
	cgGLBindProgram(program_);
	CheckCGError("Binding the shader program.");
}

void Shader::Unbind()
{
	cgGLUnbindProgram(profile_);
	CheckCGError("Unbinding the shader program.");
}

CGparameter Shader::GetNamedParameter(char* name)
{
	return cgGetNamedParameter(program_, name);
	CheckCGError("Accessing the parameter");
}

void Shader::SetStateMatrix(CGparameter param, CGGLenum matrix, CGGLenum transform)
{
	cgGLSetStateMatrixParameter(param, matrix, transform);
	CheckCGError("Setting state matrix parameter");
}

}
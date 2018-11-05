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

#ifndef SIMPLEGL_SHADER_H
#define SIMPLEGL_SHADER_H

#include "system.h"

namespace simplegl {

class Shader
{
	friend class ShaderCore;
private:
	Shader(ShaderCore& core, char* program_name, CGprofile profile, char* entry_point);

public:
	~Shader();

public:
	CGprogram GetCGProgram();
	CGprofile GetCGProfile();

	void Bind();
	void Unbind();

	CGparameter GetNamedParameter(char* name);

	void SetStateMatrix(CGparameter param, CGGLenum matrix, CGGLenum transform);

private:
	CGprogram program_;
	CGprofile profile_;
};

};

#endif /* SIMPLEGL_SHADER_H */
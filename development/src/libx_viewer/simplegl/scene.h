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

#ifndef SIMPLEGL_SCENE_H
#define SIMPLEGL_SCENE_H

#include "system.h"

#include "shadercore.h"

namespace simplegl {

class Scene
{
public:
	Scene();
	virtual ~Scene();

public: /* Virtual callback functions */
	virtual void Display() =0;
	virtual void Update() =0;
	virtual void Reshape(int width, int height) =0;
	virtual void Keyboard(unsigned char key, int x, int y) =0;
	virtual void Mouse(int button, int state, int x, int y) =0;
	virtual void MouseMotion(int x, int y) =0;
	virtual void Menu(int id) =0;

	virtual bool Init() =0;
	virtual bool CheckCapabilities() =0;

protected:
	void Redraw();

protected:
	ShaderCore* shader_core_;
};

};

#endif /* SIMPLEGL_SCENE_H */
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

#ifndef VIEWERSCENE_H
#define VIEWERSCENE_H

#include "simplegl/camera.h"
#include "simplegl/scene.h"

#include "arcball.h"
#include "model.h"
#include "world.h"

class ViewerScene : public simplegl::Scene
{
public:
	ViewerScene(x::string_t model);
	virtual ~ViewerScene();

public: /* Virtual callback functions */
	virtual void Display();
	virtual void Update();
	virtual void Reshape(int width, int height);
	virtual void Keyboard(unsigned char key, int x, int y);
	virtual void Mouse(int button, int state, int x, int y);
	virtual void MouseMotion(int x, int y);
	virtual void Menu(int id);

	virtual bool Init();
	virtual bool CheckCapabilities();

private:
	World world_;
	ArcBall arcball_;

	// The model
	x::string_t model_path_;
	Model* model_;
	simplegl::Vector3f center_;
	float scale_;

	// Debug parameters
	bool render_box_;
	bool render_normals_;
	bool render_wireframe_;
	bool render_skeleton_;

	// Time variables
	int time_base_;
};

#endif /* VIEWERSCENE_H */
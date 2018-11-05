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

#include "simplegl/application.h"

#include "viewerscene.h"

using namespace simplegl;

int main(int argc, char* argv[])
{
	Application* app = Application::Init(argc, argv);

	app->SetTitle("LibX - Viewer - OpenGL Version");
	app->SetSize(512, 512);
	app->CreateWindow(false);

	if (argc != 2)
	{
		std::cerr << "invalid argument count" << std::endl;
		return 1;
	}

	ViewerScene* scene = new ViewerScene(argv[1]);
	if (!app->SetScene(scene, true))
	{
		delete scene;
		return 2;
	}
	if (!scene->Init())
	{
		return 2;
	}

	app->Open();

	return 0;
}
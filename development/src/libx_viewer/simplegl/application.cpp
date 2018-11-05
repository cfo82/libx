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

#include "application.h"

using namespace std;

namespace simplegl {

Application* Application::current_ = 0;

Application* Application::Init(int argc, char* argv[])
{
	current_ = new Application(argc, argv);
	atexit(Application::ShutDown);
	return current_;
}

void Application::ShutDown()
{
	delete current_;
}

Application* Application::Current()
{
	return current_;
}

void Application::DisplayFunc()
{
	Scene* scene = Application::Current()->GetScene();
	if (scene)
	{
		scene->Display();
		Application::Current()->SwapBuffers();
	}
}

void Application::IdleFunc()
{
	Scene* scene = Application::Current()->GetScene();
	if (scene)
	{
		scene->Update();
	}
}

void Application::ReshapeFunc(int w, int h)
{
	Scene* scene = Application::Current()->GetScene();
	if (scene)
	{
		scene->Reshape(w, h);
	}
}

void Application::KeyBoardFunc(unsigned char key, int x, int y)
{
	Scene* scene = Application::Current()->GetScene();
	if (scene)
	{
		scene->Keyboard(key, x, y);
	}
}

void Application::MouseFunc(int button, int state, int x, int y)
{
	Scene* scene = Application::Current()->GetScene();
	if (scene)
	{
		scene->Mouse(button, state, x, y);
	}
}

void Application::MouseMotionFunc(int x, int y)
{
	Scene* scene = Application::Current()->GetScene();
	if (scene)
	{
		scene->MouseMotion(x, y);
	}
}

void Application::MenuFunc(int id)
{
	Scene* scene = Application::Current()->GetScene();
	if (scene)
	{
		scene->Menu(id);
	}
}

/*
 *	Constructor.
 *	Takes the command line arguments
 */
Application::Application(int argc, char** argv)
:	window_handle_(-1),
	shader_core_(NULL)
{
	glutInit(&argc, argv);

	scene_ = NULL;
	cleanUp_ = false;
}

/*
 *	Destructor.
 */
Application::~Application()
{
	if (scene_ && cleanUp_)
		delete scene_;

	if (shader_core_)
		delete shader_core_;

	/*if (window_handle_ != -1)
		glutDestroyWindow(window_handle_);*/
}

void Application::CreateWindow(bool fullscreen)
{
	// Initialise render window
	if (!fullscreen)
		glutInitWindowSize(width_, heigth_);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	window_handle_ = glutCreateWindow(title_.c_str());

	// Init the GLEW library
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		printf("Fatal error: Could not initialise GLEW.");
		exit(1);
	}

	// Initialise the image loader library
	ilInit();
	iluInit();
	if (!ilutRenderer(ILUT_OPENGL))
	{
		printf("Fatal error: Could not initialise ILUT.");
		exit(1);
	}

	if (fullscreen)
	{
		glutFullScreen();
		glutSetCursor(GLUT_CURSOR_NONE);
	}

	glutDisplayFunc(DisplayFunc);
	glutIdleFunc(IdleFunc);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyBoardFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MouseMotionFunc);

	glutCreateMenu(MenuFunc);

	// Create the shader core
	shader_core_ = new ShaderCore();
}

void Application::Open()
{
	glutMainLoop();
}

void Application::SetTitle(string title)
{
	title_ = title;	
}

void Application::SetSize(int width, int height)
{
	width_ = width;
	heigth_ = height;
}

bool Application::SetScene(Scene* scene, bool cleanUp)
{
	if (scene_)
	{
		if (cleanUp)
			delete scene_;
	}
	
	scene_ = scene;
	cleanUp_ = cleanUp;

	if (!scene_->CheckCapabilities())
	{
		scene_ = 0;
		cleanUp_ = false;
		return false;
	}

	return true;
}

Scene* Application::GetScene() const
{
	return scene_;
}

ShaderCore* Application::GetShaderCore()
{
	return shader_core_;
}

void Application::SwapBuffers()
{
	glutSwapBuffers();
}

int Application::GetCurrentTime() const
{
	return glutGet(GLUT_ELAPSED_TIME);
}

}
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

#ifndef SIMPLEGL_APPLICATION_H
#define SIMPLEGL_APPLICATION_H

#include <string>

#include "system.h"

#include "scene.h"
#include "shadercore.h"

#undef CreateWindow

namespace simplegl {

class Application
{
private:
	static Application* current_;
public:
	// Initialize a new application
	static Application* Init(int argc, char* argv[]);

	// Shutdown the application and free the memory
	static void ShutDown();

	// Get the running instance
	static Application* Current();

private:
	static void DisplayFunc();
	static void IdleFunc();
	static void ReshapeFunc(int width, int height);
	static void KeyBoardFunc(unsigned char key, int x, int y);
	static void MouseFunc(int button, int state, int x, int y);
	static void MouseMotionFunc(int x, int y);
	static void MenuFunc(int id);

private:
	Application(int argc, char** argv);
public:
	~Application();

	// Create the window
	void CreateWindow(bool fullscreen);

	// Open the main window
	void Open();

	// Set some properties
	void SetSize(int width, int height);
	void SetTitle(std::string title);

	bool SetScene(Scene* scene, bool cleanUp);
	Scene* GetScene() const;

	ShaderCore* GetShaderCore();

	int GetCurrentTime() const;

	void SwapBuffers();
private:
	int width_;
	int heigth_;
	int window_handle_;

	std::string title_;

	Scene* scene_;
	bool cleanUp_;

	ShaderCore* shader_core_;
};

};

#endif /* SIMPLEGL_APPLICATION_H */
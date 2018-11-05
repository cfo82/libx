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

#include "viewerscene.h"

ViewerScene::ViewerScene(x::string_t model)
:	arcball_(1, 1),
	scale_(1.0f),
	render_box_(false),
	render_normals_(false),
	render_wireframe_(false),
	render_skeleton_(false),
	time_base_(0)
{
	model_path_ = model;

	Model::CreateXInterface();
}

ViewerScene::~ViewerScene()
{
	delete model_;
	Model::DeleteXInterface();
}

bool ViewerScene::CheckCapabilities()
{
	if (!GLEW_ARB_multitexture)
	{
		printf("No multitexturing support.\n");
		return false;
	}

	return true;
}

bool ViewerScene::Init()
{
	using namespace simplegl;

	// Configure the render target
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

	// Configure the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

	// Shading model
	glShadeModel(GL_SMOOTH);

	// Enable culling
	glEnable(GL_CULL_FACE);

	// Antialiasing
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Load a model
	model_ = Model::CreateFromFile(model_path_.c_str());
	if (model_ == NULL)
	{
		return false;
	}

	// Build model positioning, scaling camera
	simplegl::Vector3f min_pos, max_pos;
	model_->GetAABB(min_pos, max_pos);

	simplegl::Vector3f diagonal = max_pos - min_pos;
	float distance = diagonal.GetLen();
	scale_ = 1.0f;// / distance;

	center_.x = min_pos.x + 0.5f * diagonal.x;
	center_.y = min_pos.y + 0.5f * diagonal.y;
	center_.z = min_pos.z + 0.5f * diagonal.z;

	// Configure camera
	Camera& camera = world_.GetCamera();
	camera.SetPosition(0.0f, 0.0f, -1.5*distance);
	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(3*distance);

	// Set up the sun ;)
	DirectLight& sun = world_.GetSunLight();
	sun.Diffuse = Vector3f(1.0f, 1.0f, 1.0f);
	sun.Direction = Vector3f(-1.0f, -1.0f, 1.0f);
	sun.Direction.Normalize();

	for (int i = 0; i < model_->GetAnimationCount(); i++)
	{
		if (strlen(model_->GetAnimationName(i).c_str()) > 0)
			glutAddMenuEntry(model_->GetAnimationName(i).c_str(), i);
		else
		{
			char tmp[512];
			sprintf(tmp, "Animation %d", i);
			glutAddMenuEntry(tmp, i);
		}
	}
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	return true;
}

void ViewerScene::Display()
{
	// Render the scene
	int matrix_mode;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glGetIntegerv(GL_MATRIX_MODE, &matrix_mode);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	world_.GetCamera().ApplyModelView();

	// World transform
	simplegl::Matrix4x4f world_matrix;
	arcball_.ApplyRotation(world_matrix);

	//glScalef(scale_, scale_, scale_);
	glTranslatef(-center_.x, -center_.y, -center_.z);

	world_matrix.Transpose();

	//world_matrix._11 *= scale_;
	//world_matrix._12 *= scale_;
	//world_matrix._13 *= scale_;

	//world_matrix._21 *= scale_;
	//world_matrix._22 *= scale_;
	//world_matrix._23 *= scale_;

	//world_matrix._31 *= scale_;
	//world_matrix._32 *= scale_;
	//world_matrix._33 *= scale_;

	world_matrix.data.elements._14 = world_matrix.data.elements._11 * -center_.x + world_matrix.data.elements._12 * -center_.y + world_matrix.data.elements._13 * -center_.z + world_matrix.data.elements._14;
	world_matrix.data.elements._24 = world_matrix.data.elements._21 * -center_.x + world_matrix.data.elements._22 * -center_.y + world_matrix.data.elements._23 * -center_.z + world_matrix.data.elements._24;
	world_matrix.data.elements._34 = world_matrix.data.elements._31 * -center_.x + world_matrix.data.elements._32 * -center_.y + world_matrix.data.elements._33 * -center_.z + world_matrix.data.elements._34;

	world_.SetWorldTransform(world_matrix);

	shader_core_->Enable();

	if (render_wireframe_)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	model_->Render(world_);

	if (render_wireframe_)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (render_skeleton_)
		model_->RenderSkeleton(world_);

	if (render_normals_)
	{
		glPushMatrix();

		glLoadIdentity();
		world_.GetCamera().ApplyModelView();

		model_->RenderNormals(world_);

		glPopMatrix();
	}

	if (render_box_)
		model_->RenderBox(world_);

	shader_core_->Disable();

	glMatrixMode(matrix_mode);
}

void ViewerScene::Update()
{
	int time = simplegl::Application::Current()->GetCurrentTime();

	int diff = time - time_base_;

	time_base_ = time;

	// Update the scene
	world_.UpdateTime((float) diff / 1000.0f);

	model_->UpdatePose(world_.GetTime());

	this->Redraw();
}

void ViewerScene::Reshape(int width, int height)
{
	int matrix_mode;
	
	glViewport(0, 0, width, height);
	
	glGetIntegerv(GL_MATRIX_MODE, &matrix_mode);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	world_.GetCamera().SetRatio((float) width/(float) height);
	world_.GetCamera().ApplyProjection();

	glMatrixMode(matrix_mode);

	arcball_.SetBounds(width, height);
}

void ViewerScene::Keyboard(unsigned char key, int x, int y)
{
	switch (key)
    {
    case 27: // Escape key
        exit(0);
		break;

	case 'b':
		render_box_ = !render_box_;
		Redraw();
		break;

	case 'n':
		render_normals_ = !render_normals_;
		Redraw();
		break;

	case 'w':
		render_wireframe_ = !render_wireframe_;
		Redraw();
		break;

	case 's':
		render_skeleton_ = !render_skeleton_;
		Redraw();
		break;
    }
}

void ViewerScene::Menu(int id)
{
	model_->SetAnimation(id);
	Redraw();
}

void ViewerScene::Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			arcball_.MouseDown(x, y);
		}
		else if (state == GLUT_UP)
		{
			arcball_.MouseUp();
		}
	}
}

void ViewerScene::MouseMotion(int x, int y)
{
	arcball_.MouseMove(x, y);
	Redraw();
}
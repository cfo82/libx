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

#include <iostream>
#include "model.h"

x::Interface* Model::x_interface_ = 0;

x::math::Vector3f mul(const x::math::Matrix4x4f& mat, const x::math::Vector3f& vec)
{
	x::math::Vector3f res;

	res.x = mat.data.elements._11 * vec.x + mat.data.elements._12 * vec.y + mat.data.elements._13 * vec.z + mat.data.elements._14;
	res.y = mat.data.elements._21 * vec.x + mat.data.elements._22 * vec.y + mat.data.elements._23 * vec.z + mat.data.elements._24;
	res.z = mat.data.elements._31 * vec.x + mat.data.elements._32 * vec.y + mat.data.elements._33 * vec.z + mat.data.elements._34;

	return res;
}

void mul(const x::math::Matrix4x4f& mat1, const x::math::Matrix4x4f& mat2, x::math::Matrix4x4f& out)
{
	out.data.elements._11 = (mat1.data.elements._11*mat2.data.elements._11 + mat1.data.elements._12*mat2.data.elements._21 + mat1.data.elements._13*mat2.data.elements._31 + mat1.data.elements._14*mat2.data.elements._41);
	out.data.elements._12 = (mat1.data.elements._11*mat2.data.elements._12 + mat1.data.elements._12*mat2.data.elements._22 + mat1.data.elements._13*mat2.data.elements._32 + mat1.data.elements._14*mat2.data.elements._42);
	out.data.elements._13 = (mat1.data.elements._11*mat2.data.elements._13 + mat1.data.elements._12*mat2.data.elements._23 + mat1.data.elements._13*mat2.data.elements._33 + mat1.data.elements._14*mat2.data.elements._43);
	out.data.elements._14 = (mat1.data.elements._11*mat2.data.elements._14 + mat1.data.elements._12*mat2.data.elements._24 + mat1.data.elements._13*mat2.data.elements._34 + mat1.data.elements._14*mat2.data.elements._44);

	out.data.elements._21 = (mat1.data.elements._21*mat2.data.elements._11 + mat1.data.elements._22*mat2.data.elements._21 + mat1.data.elements._23*mat2.data.elements._31 + mat1.data.elements._24*mat2.data.elements._41);
	out.data.elements._22 = (mat1.data.elements._21*mat2.data.elements._12 + mat1.data.elements._22*mat2.data.elements._22 + mat1.data.elements._23*mat2.data.elements._32 + mat1.data.elements._24*mat2.data.elements._42);
	out.data.elements._23 = (mat1.data.elements._21*mat2.data.elements._13 + mat1.data.elements._22*mat2.data.elements._23 + mat1.data.elements._23*mat2.data.elements._33 + mat1.data.elements._24*mat2.data.elements._43);
	out.data.elements._24 = (mat1.data.elements._21*mat2.data.elements._14 + mat1.data.elements._22*mat2.data.elements._24 + mat1.data.elements._23*mat2.data.elements._34 + mat1.data.elements._24*mat2.data.elements._44);

	out.data.elements._31 = (mat1.data.elements._31*mat2.data.elements._11 + mat1.data.elements._32*mat2.data.elements._21 + mat1.data.elements._33*mat2.data.elements._31 + mat1.data.elements._34*mat2.data.elements._41);
	out.data.elements._32 = (mat1.data.elements._31*mat2.data.elements._12 + mat1.data.elements._32*mat2.data.elements._22 + mat1.data.elements._33*mat2.data.elements._32 + mat1.data.elements._34*mat2.data.elements._42);
	out.data.elements._33 = (mat1.data.elements._31*mat2.data.elements._13 + mat1.data.elements._32*mat2.data.elements._23 + mat1.data.elements._33*mat2.data.elements._33 + mat1.data.elements._34*mat2.data.elements._43);
	out.data.elements._34 = (mat1.data.elements._31*mat2.data.elements._14 + mat1.data.elements._32*mat2.data.elements._24 + mat1.data.elements._33*mat2.data.elements._34 + mat1.data.elements._34*mat2.data.elements._44);

	out.data.elements._41 = (mat1.data.elements._41*mat2.data.elements._11 + mat1.data.elements._42*mat2.data.elements._21 + mat1.data.elements._43*mat2.data.elements._31 + mat1.data.elements._44*mat2.data.elements._41);
	out.data.elements._42 = (mat1.data.elements._41*mat2.data.elements._12 + mat1.data.elements._42*mat2.data.elements._22 + mat1.data.elements._43*mat2.data.elements._32 + mat1.data.elements._44*mat2.data.elements._42);
	out.data.elements._43 = (mat1.data.elements._41*mat2.data.elements._13 + mat1.data.elements._42*mat2.data.elements._23 + mat1.data.elements._43*mat2.data.elements._33 + mat1.data.elements._44*mat2.data.elements._43);
	out.data.elements._44 = (mat1.data.elements._41*mat2.data.elements._14 + mat1.data.elements._42*mat2.data.elements._24 + mat1.data.elements._43*mat2.data.elements._34 + mat1.data.elements._44*mat2.data.elements._44);
}

Model::Model()
:	model_(new x::model::Model),
	min_pos_(FLT_MAX, FLT_MAX, FLT_MAX),
	max_pos_(-FLT_MAX, -FLT_MAX, -FLT_MAX),
	animation_controller_(NULL)
{
}

Model::~Model()
{
	while (!texture_maps_.empty())
	{
		delete texture_maps_.begin()->second;
		texture_maps_.erase(texture_maps_.begin());
	}

	DeleteModelShaders(model_shader_);
	DeleteBoneShaders(bone_shader_);

	delete animation_controller_;
	delete model_;
}

void Model::CreateXInterface()
{
	if (x_interface_ == 0)
		x_interface_ = new x::Interface;
}

void Model::DeleteXInterface()
{
	if (x_interface_)
		delete x_interface_;
}

Model* Model::CreateFromFile(const char *filename)
{
	using namespace x;
	using namespace x::templates;
	using namespace std;

	Model *model = new Model();

	vector<Template*> Instances;

	x::string_t path(filename);

	if (x_interface_->Import(path, Instances))
	{
		x_interface_->Export("D:/test_out.txt", Instances, x::ExportText);

		try {
			XMeshGenerator::Generate(model->model_, Instances);
		}
		catch (x::Exception& e) {
			cerr << "Exception occured! (" << e.GetDescription() << ")" << endl;
			delete model;
			return NULL;
		}

		x_interface_->ReleaseInstances(Instances);
	}
	else
	{
		delete model;
		return NULL;
	}

	x::string_t::size_type pos = path.find_last_of('\\');
	if (pos == (x::string_t::size_type) -1)
	{
		pos = path.find_last_of('/');

		if (pos == x::string_t::npos)
		{
			model->base_path_ = "./";
		}
		else
		{
			model->base_path_ = path.substr(0, pos + 1);
		}
	}
	else
	{
		if (pos == x::string_t::npos)
		{
			model->base_path_ = ".\\";
		}
		else
		{
			model->base_path_ = path.substr(0, pos + 1);
		}
	}

	model->PostInit();

	return model;
}

void Model::PostInit()
{
	// Load the shaders
	CreateModelShaders(model_shader_);
	CreateBoneShaders(bone_shader_);

	// Initialise the controller
	if (model_->GetAnimationGroupCount() > 0)
	{
		animation_controller_ = model_->GetAnimationGroup(0)->CreateAnimationController();
	}

	// Set the first animation
	if (model_->GetAnimationGroupCount() > 0)
	{
		x::model::AnimationGroup* animation_group = model_->GetAnimationGroup(0);
		animation_controller_->SwitchToAnimation(animation_group->GetAnimation(0));
	}

	// Check if the model has normals
	x::model::Frame* root = model_->GetRootFrame();
	GenerateNormals(root);

	// Load the textures
	LoadMaterials(root);

	// Build the building box of the model
	BuildBoundingBox(root);
}

void Model::GenerateNormals(x::model::Frame* frame)
{
	for (unsigned int i = 0; i < frame->GetFrameCount(); i++)
	{
		GenerateNormals(frame->GetFrame(i));
	}

	x::uint_t entity_count = frame->GetEntityCount();
	for (x::uint_t e = 0; e < entity_count; e++)
	{
		x::model::Entity *entity = frame->GetEntity(e);
		x::model::Geometry *geometry = entity->GetGeometry();
		if (geometry->GetStream(x::model::SemanticTypeNormal, 0) == NULL)
			model_->ComputeNormalVectors();
	}
}

x::string_t Model::GetAnimationName(int i) const
{
	if (model_->GetAnimationGroupCount() > 0)
		return model_->GetAnimationGroup(0)->GetAnimation(i)->GetName();
	else
		return "";
}

int Model::GetAnimationCount() const
{
	if (model_->GetAnimationGroupCount() > 0)
		return model_->GetAnimationGroup(0)->GetAnimationCount();
	else
		return 0;
}

void Model::SetAnimation(int i)
{
	if (animation_controller_)
	{
		animation_controller_->SwitchToAnimation( model_->GetAnimationGroup(0)->GetAnimation(i)->GetName());
	}
}

void Model::GetAABB(simplegl::Vector3f& min_pos, simplegl::Vector3f& max_pos)
{
	min_pos.x = min_pos_.x;
	min_pos.y = min_pos_.y;
	min_pos.z = min_pos_.z;

	max_pos.x = max_pos_.x;
	max_pos.y = max_pos_.y;
	max_pos.z = max_pos_.z;
}

void Model::LoadMaterials(x::model::Frame* frame)
{
	for (unsigned int i = 0; i < frame->GetFrameCount(); i++)
	{
		LoadMaterials(frame->GetFrame(i));
	}

	x::uint_t entity_count = frame->GetEntityCount();
	for (x::uint_t e = 0; e < entity_count; e++)
	{
		x::model::Entity *entity = frame->GetEntity(e);
		x::model::Material* material = entity->GetMaterial();

		if (material == NULL)
			continue;

		for (x::uint_t s = 0; s < material->GetTextureSamplerCount(); s++)
		{
			x::model::TextureSampler& sampler = material->GetTextureSampler(s);
			
			if (texture_maps_.find(sampler.texture) == texture_maps_.end())
			{
				simplegl::Texture* texture = simplegl::Texture::CreateTexture(base_path_ + sampler.texture);
				if (texture)
				{
					texture_maps_[sampler.texture] = texture;
				}
			}
		}
	}
}

void Model::BuildBoundingBox(x::model::Frame* frame)
{
	for (unsigned int i = 0; i < frame->GetFrameCount(); i++)
	{
		BuildBoundingBox(frame->GetFrame(i));
	}

	const x::math::Matrix4x4f transform = frame->GetLocalTransformation();

	x::uint_t entity_count = frame->GetEntityCount();
	for (x::uint_t e = 0; e < entity_count; e++)
	{
		x::model::Entity *entity = frame->GetEntity(e);
		x::model::Geometry *geometry = entity->GetGeometry();

		x::model::Stream* stream = geometry->GetStream(x::model::SemanticTypePosition, 0);

		for (x::uint_t i = 0; i < stream->GetElementCount(); i++)
		{
			x::math::Vector3f position = mul(transform, stream->GetVector3(i));

			min_pos_.x = min(position.x, min_pos_.x);
			min_pos_.y = min(position.y, min_pos_.y);
			min_pos_.z = min(position.z, min_pos_.z);

			max_pos_.x = max(position.x, max_pos_.x);
			max_pos_.y = max(position.y, max_pos_.y);
			max_pos_.z = max(position.z, max_pos_.z);
		}
	}
}

void Model::UpdatePose(float current_time)
{
	if (animation_controller_)
	{
		animation_controller_->Apply(current_time);
		x::model::Bone* root_bone = model_->GetSkeleton()->GetRootBone();
		root_bone->UpdateCombinedTransformationMatrix(NULL);

		// Update the model data
		model_->UpdateSkinnedModel();
	}
}

void Model::Render(const World &world)
{
	model_shader_.VertexShader->Bind();

	// Set up the lighting parameters
	DirectLight sun;
	world.GetSunLight(sun);
	if (model_shader_.ConstVSLightDirection)
		cgSetParameter3f(model_shader_.ConstVSLightDirection, sun.Direction.x, sun.Direction.y, sun.Direction.z);

	if (model_shader_.ConstVSLightDiffuseColour)
		cgSetParameter3f(model_shader_.ConstVSLightDiffuseColour, sun.Diffuse.x, sun.Diffuse.y, sun.Diffuse.z);

	// Setup the matrix stack
	x::model::Frame *frame = model_->GetRootFrame();
	RenderFrame(frame, world);

	for (unsigned int i = 0; i < frame->GetFrameCount(); i++)
	{
		RenderFrame(frame->GetFrame(i), world);
		break;
	}

	model_shader_.VertexShader->Unbind();
}

void Model::RenderNormals(const World &world)
{
	model_shader_.VertexShader->Bind();
	model_shader_.FSNoDecalNormalsShader->Bind();

	if (model_shader_.ConstVSModelViewProjectionMatrix)
		model_shader_.VertexShader->SetStateMatrix(model_shader_.ConstVSModelViewProjectionMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	// Setup the matrix stack
	x::model::Frame *frame = model_->GetRootFrame();
	RenderFrameNormals(frame, world);

	for (unsigned int i = 0; i < frame->GetFrameCount(); i++)
	{
		RenderFrameNormals(frame->GetFrame(i), world);
	}

	model_shader_.FSNoDecalNormalsShader->Unbind();
	model_shader_.VertexShader->Unbind();
}

void Model::RenderBox(const World &world)
{
	model_shader_.VertexShader->Bind();
	model_shader_.FSNoDecalNormalsShader->Bind();

	if (model_shader_.ConstVSModelViewProjectionMatrix)
		model_shader_.VertexShader->SetStateMatrix(model_shader_.ConstVSModelViewProjectionMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	simplegl::Vector3f min_pos, max_pos;
	this->GetAABB(min_pos, max_pos);

	glBegin(GL_LINES);
	{
		glVertex3f(min_pos.x, min_pos.y, min_pos.z); glVertex3f(max_pos.x, min_pos.y, min_pos.z);
		glVertex3f(max_pos.x, min_pos.y, min_pos.z); glVertex3f(max_pos.x, min_pos.y, max_pos.z);
		glVertex3f(max_pos.x, min_pos.y, max_pos.z); glVertex3f(min_pos.x, min_pos.y, max_pos.z);
		glVertex3f(min_pos.x, min_pos.y, max_pos.z); glVertex3f(min_pos.x, min_pos.y, min_pos.z);

		glVertex3f(min_pos.x, min_pos.y, min_pos.z); glVertex3f(min_pos.x, max_pos.y, min_pos.z);
		glVertex3f(max_pos.x, min_pos.y, min_pos.z); glVertex3f(max_pos.x, max_pos.y, min_pos.z);
		glVertex3f(max_pos.x, min_pos.y, max_pos.z); glVertex3f(max_pos.x, max_pos.y, max_pos.z);
		glVertex3f(min_pos.x, min_pos.y, max_pos.z); glVertex3f(min_pos.x, max_pos.y, max_pos.z);

		glVertex3f(min_pos.x, max_pos.y, min_pos.z); glVertex3f(max_pos.x, max_pos.y, min_pos.z);
		glVertex3f(max_pos.x, max_pos.y, min_pos.z); glVertex3f(max_pos.x, max_pos.y, max_pos.z);
		glVertex3f(max_pos.x, max_pos.y, max_pos.z); glVertex3f(min_pos.x, max_pos.y, max_pos.z);
		glVertex3f(min_pos.x, max_pos.y, max_pos.z); glVertex3f(min_pos.x, max_pos.y, min_pos.z);

	} glEnd();

	model_shader_.FSNoDecalNormalsShader->Unbind();
	model_shader_.VertexShader->Unbind();
}

void Model::RenderSkeleton(const World &world)
{
	if (model_->GetSkeleton())
	{
		x::model::Bone* top_bone = model_->GetSkeleton()->GetRootBone();

		bone_shader_.VertexShader->Bind();
		bone_shader_.FragmentShader->Bind();

		bone_shader_.VertexShader->SetStateMatrix(bone_shader_.ConstVSViewProjMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
		cgSetParameter4f(bone_shader_.ConstVSColour, 1.0f, 1.0f, 1.0f, 1.0f);

		float point_size;
		glGetFloatv(GL_POINT_SIZE, &point_size);
		glPointSize(4.0f);

		glBegin(GL_POINTS);
			RenderSkeleton(top_bone, top_bone->GetFirstChildBone());
		glEnd();
		
		glPointSize(point_size);

		glBegin(GL_LINES);
			RenderSkeleton(top_bone, top_bone->GetFirstChildBone());
		glEnd();

		bone_shader_.VertexShader->Unbind();
		bone_shader_.FragmentShader->Unbind();
	}
}

void Model::RenderFrame(x::model::Frame* frame, const World &world)
{
	for (unsigned int i = 0; i < frame->GetFrameCount(); i++)
	{
		RenderFrame(frame->GetFrame(i), world);
	}

	// Set the local transformation
	if (model_shader_.ConstVSModelViewProjectionMatrix)
	{
		glPushMatrix();

		if (this->model_->GetSkeleton() == NULL)
		{
			x::math::Matrix4x4f transform = frame->GetLocalTransformation();
			transform.Transpose();
			glMultMatrixf(transform.data.array1d);
		}

		model_shader_.VertexShader->SetStateMatrix(model_shader_.ConstVSModelViewProjectionMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

		glPopMatrix();
	}

	if (model_shader_.ConstVSInverseWorldMatrix)
	{
		x::math::Matrix4x4f world_transform = world.GetWorldTransform();

		if (this->model_->GetSkeleton() == NULL)
		{
			world_transform = world_transform * frame->GetLocalTransformation();
		}

		world_transform = world_transform.GetInverse();

		// Don't transpose since the we use a row based matrix representation, and not a column based one like OpenGL.

		cgSetMatrixParameterfc(model_shader_.ConstVSInverseWorldMatrix, world_transform.data.array1d);
	}

	x::uint_t entity_count = frame->GetEntityCount();
	for (x::uint_t e = 0; e < entity_count; e++)
	{
		x::model::Entity *entity = frame->GetEntity(e);
		x::model::Material *material = entity->GetMaterial();
		x::model::Geometry *geometry = entity->GetGeometry();

		x::uint_t index_count = entity->GetIndexCount();
		const x::ubyte_t *index_data = entity->GetIndexData();

		// Set the entity material
		if (model_shader_.ConstFSTextureDiffuse && material != NULL && material->GetTextureSamplerCount() > 0)
		{
			model_shader_.FSDecalShader->Bind();
			simplegl::Texture* texture = texture_maps_[material->GetTextureSampler(0).texture];
			cgGLSetTextureParameter(model_shader_.ConstFSTextureDiffuse, texture->GetID());
			cgGLEnableTextureParameter(model_shader_.ConstFSTextureDiffuse);
		}
		else
		{
			model_shader_.FSNoDecalShader->Bind();
		}

		if (material != NULL)
		{
			if (model_shader_.ConstVSMaterialAmbient)
			{
				const x::math::Colorf& colour = material->GetAmbientColor();
				cgSetParameter4f(model_shader_.ConstVSMaterialAmbient, colour.r, colour.g, colour.b, colour.a);
			}
			if (model_shader_.ConstVSMaterialDiffuse)
			{
				const x::math::Colorf& colour = material->GetDiffuseColor();
				cgSetParameter4f(model_shader_.ConstVSMaterialDiffuse, colour.r, colour.g, colour.b, colour.a);
			}
			if (model_shader_.ConstVSMaterialSpecular)
			{
				const x::math::Colorf& colour = material->GetSpecularColor();
				cgSetParameter4f(model_shader_.ConstVSMaterialSpecular, colour.r, colour.g, colour.b, colour.a);
			}
			if (model_shader_.ConstVSMaterialEmissive)
			{
				const x::math::Colorf& colour = material->GetEmissiveColor();
				cgSetParameter4f(model_shader_.ConstVSMaterialEmissive, colour.r, colour.g, colour.b, colour.a);
			}
		}

		// Draw the entity
		for (x::uint_t i = 0; i < geometry->GetStreamCount(); i++)
		{
			x::model::Stream *stream = geometry->GetStream(i);

			// Lock the buffer
			x::uint_t stride;
			x::ubyte_t *data = stream->LockData(stride);
			x::model::ValueType type = stream->GetValueType();

			x::uint_t size = GetSizeFromType(stream->GetValueType());

			switch (stream->GetSemantics())
			{
			case x::model::SemanticTypePosition:
				if (stream->GetSemanticInfo() == 0)
				{
					glVertexPointer(size, GL_FLOAT, stride, data);
					glEnableClientState(GL_VERTEX_ARRAY);
				}
				else
				{
				#ifdef _DEBUG
					printf("Stream count not supported.\n");
				#endif
				}
				break;
			case x::model::SemanticTypeNormal:
				if (stream->GetSemanticInfo() == 0)
				{
					glNormalPointer(GL_FLOAT, stride, data);
					glEnableClientState(GL_NORMAL_ARRAY);
				}
				else
				{
				#ifdef _DEBUG
					printf("Stream count not supported.\n");
				#endif
				}
				break;
			case x::model::SemanticTypeTextureCoordinate:
				glClientActiveTextureARB(GL_TEXTURE0_ARB);

				glTexCoordPointer(size, GL_FLOAT, stride, data);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				break;
			default:
			#ifdef _DEBUG
				//printf("Stream count not supported.\n");
			#endif
				break;
			}
		}

		GLenum mode = ConvertPrimitiveType(entity->GetPrimitiveType());
		GLenum type = ConvertIndexSize(entity->GetIndexSize());

		glDrawElements(mode, index_count, type, index_data);

		for (x::uint_t i = 0; i < geometry->GetStreamCount(); i++)
		{
			x::model::Stream *stream = geometry->GetStream(i);
			stream->UnlockData();
			
			switch (stream->GetSemantics())
			{
			case x::model::SemanticTypePosition:
				if (stream->GetSemanticInfo() == 0)
				{
					glDisableClientState(GL_VERTEX_ARRAY);
				}
				else
				{
				#ifdef _DEBUG
					printf("Stream count not supported.\n");
				#endif
				}
				break;
			case x::model::SemanticTypeNormal:
				if (stream->GetSemanticInfo() == 0)
				{
					glDisableClientState(GL_NORMAL_ARRAY);
				}
				else
				{
				#ifdef _DEBUG
					printf("Stream count not supported.\n");
				#endif
				}
				break;
			case x::model::SemanticTypeTextureCoordinate:
				glClientActiveTextureARB(GL_TEXTURE0_ARB);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				
				break;
			default:
			#ifdef _DEBUG
				//printf("Stream count not supported.\n");
			#endif
				break;
			}
		}

		if (model_shader_.ConstFSTextureDiffuse && material != NULL && material->GetTextureSamplerCount() > 0)
		{
			model_shader_.FSDecalShader->Unbind();
			cgGLDisableTextureParameter(model_shader_.ConstFSTextureDiffuse);
		}
		else
		{
			model_shader_.FSNoDecalShader->Unbind();
		}
	}

	if (model_shader_.ConstVSModelViewProjectionMatrix)
	{
		glPopMatrix();
	}
}

void Model::RenderFrameNormals(x::model::Frame* frame, const World &world)
{
	for (unsigned int i = 0; i < frame->GetFrameCount(); i++)
	{
		RenderFrameNormals(frame->GetFrame(i), world);
	}

	x::math::Matrix4x4f world_transform = world.GetWorldTransform();
	if (this->model_->GetSkeleton() == NULL)
	{
		world_transform = world_transform * frame->GetLocalTransformation();
	}
	
	x::math::Matrix4x4f inv_trans_world_transform = world_transform;
	inv_trans_world_transform = inv_trans_world_transform.GetInverse();
	inv_trans_world_transform.Transpose();

	simplegl::Vector3f diagonal = max_pos_ - min_pos_;
	float scale = diagonal.GetLen() / 20.0f;

	glBegin(GL_LINES);
	{
		x::uint_t entity_count = frame->GetEntityCount();
		for (x::uint_t e = 0; e < entity_count; e++)
		{
			x::model::Entity *entity = frame->GetEntity(e);
			x::model::Material *material = entity->GetMaterial();
			x::model::Geometry *geometry = entity->GetGeometry();

			x::uint_t index_count = entity->GetIndexCount();
			const x::ubyte_t *index_data = entity->GetIndexData();

			x::model::Stream* positions = geometry->GetStream(x::model::SemanticTypePosition, 0);
			x::model::Stream* normals = geometry->GetStream(x::model::SemanticTypeNormal, 0);

			for (x::uint_t i = 0; i < positions->GetElementCount(); i++)
			{
				const x::math::Vector3f start = mul(world_transform, positions->GetVector3(i));
				x::math::Vector3f normal = normals->GetVector3(i);
				normal.Normalize();
				normal = mul(inv_trans_world_transform, normal);
				normal.Normalize();
				const x::math::Vector3f end = start + normal * scale;
				//const x::math::Vector3f end = mul(world_transform, positions->GetVector3(i) + normal * scale);

				glVertex3f(start.x, start.y, start.z);
				glVertex3f(end.x, end.y, end.z);
			}
		}
	} glEnd();
}

void Model::RenderSkeleton(const x::model::Bone* parent_bone, const x::model::Bone* bone)
{
	x::math::Vector3f zero(0.0f, 0.0f, 0.0f);
	x::math::Vector3f parent_pos = mul(parent_bone->GetCombinedTransformationMatrix(), zero);
	x::math::Vector3f pos = mul(bone->GetCombinedTransformationMatrix(), zero);

	glVertex3f(parent_pos.x, parent_pos.y, parent_pos.z);
	glVertex3f(pos.x, pos.y, pos.z);
	
	if (bone->GetNextBone())
	{
		RenderSkeleton(parent_bone, bone->GetNextBone());
	}
	if (bone->GetFirstChildBone())
	{
		RenderSkeleton(bone, bone->GetFirstChildBone());
	}
}

GLint Model::GetSizeFromType(x::model::ValueType type)
{
	GLint size = -1;
	switch(type)
	{
	case x::model::ValueColor:
		size = 4;
		break;
	case x::model::ValueFloat:
		size = 1;
		break;
	case x::model::ValueVector2:
		size = 2;
		break;
	case x::model::ValueVector3:
		size = 3;
		break;
	case x::model::ValueQuaternion:
		size = 4;
		break;
	}

	return size;
}

GLenum Model::ConvertPrimitiveType(x::model::PrimitiveType type)
{
	GLenum mode = GL_INVALID_ENUM;
	switch (type)
	{
	case x::model::PrimitiveTriangleList:
		mode = GL_TRIANGLES;
		break;
	default:
		printf("Primitive type not supported.\n");
	}

	return mode;
}

GLenum Model::ConvertIndexSize(x::uint_t size)
{
	GLenum type = GL_INVALID_ENUM;
	switch(size)
	{
	case 8:
		type = GL_UNSIGNED_BYTE;
		break;
	case 16:
		type = GL_UNSIGNED_SHORT;
		break;
	case 32:
		type = GL_UNSIGNED_INT;
		break;
	default:
		printf("Index element size not supported.\n");
	}

	return type;
}
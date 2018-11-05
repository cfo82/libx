//-----------------------------------------------------------------------------------------------------------
/*
 *	LibX - A Library to export/import .x-models
 *
 *	This file is
 *		Copyright (C) 2008 Christian Oberholzer <coberholzer@gmx.ch>
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

//-----------------------------------------------------------------------------------------------------------
/*!	\file templates/mesh.h
	\brief Definition von x::templates::Mesh
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::templates::Mesh
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPLATES_MESH_H
#define TEMPLATES_MESH_H

#include "../global/global.h"
#include "../temporalinfo.h"
#include "./template.h"

namespace x { namespace templates {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Mesh-Template
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Implementiert das X-File-Mesh-Template.
*/
class Mesh : public Template
{
public:
	typedef vector<Template*> children_container_t;

public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	Mesh();
protected:
	virtual ~Mesh();

public:
	//-------------------------------------------------------------------------------------------------------
	virtual void PrintFileStructure(const uint_t level) const;
	virtual Template* Clone() const;
	virtual void ReadDefinition(TemporalInfo& info);
	virtual void ReadData(tokens::TokenName* name_token, tokens::TokenGuid* guid_token, TemporalInfo& info, bool_t read_cbrace);
	virtual void WriteData(writer::Writer* writer, bool_t* error_flag, string_t* error_message);

	//-------------------------------------------------------------------------------------------------------
	virtual const string_t& GetName() const;
	virtual const uint_t GetVertexCount() const;
	virtual const ubyte_t* GetVertices() const;
	virtual const uint_t GetFaceCount() const;
	virtual const ubyte_t* GetFaces() const;
	virtual const children_container_t& GetChildren() const;

protected:
	string_t name_;
	udword_t vertex_count_;
	ubyte_t* vertices_;
	udword_t face_count_;
	udword_t faces_size_;
	ubyte_t* faces_;
	children_container_t children_;
};

}} // namespace x::templates

#endif // TEMPLATES_MESH_H

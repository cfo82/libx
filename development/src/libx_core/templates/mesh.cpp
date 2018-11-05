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
/*!	\file templates/mesh.cpp
	\brief Implementation von x::templates::Mesh
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::Mesh
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../global/global.h"
#include "../tokens/tokens.h"
#include "../serializer.h"
#include "./templates.h"

namespace x { namespace templates {

using namespace x::tokens;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Mesh::Mesh()
:	Template(TemplateTypeMesh),
	vertex_count_(0),
	vertices_(NULL),
	face_count_(0),
	faces_size_(0),
	faces_(NULL),
	children_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Mesh::~Mesh()
{
	SAFE_DELETE_ARRAY(vertices_);
	SAFE_DELETE_ARRAY(faces_);

	for (
		children_container_t::iterator begin = children_.begin();
		begin != children_.end();
		++begin
	)
	{
		Template* current_template = (*begin);
		current_template->RemRef();
	}
	children_.clear();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template geh�rt aus.
*/
void Mesh::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "Mesh" << endl;
	for (
		children_container_t::const_iterator begin = children_.begin();
		begin != children_.end();
		++begin
	)
	{
		(*begin)->PrintFileStructure(level + 1);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zur�ck.
*/
Template* Mesh::Clone() const
{
	Mesh* clone = new Mesh;
	clone->name_ = name_;
	clone->vertex_count_ = vertex_count_;
	clone->vertices_ = new ubyte_t[sizeof(float_t) * vertex_count_ * 3];
	memcpy(clone->vertices_, vertices_, sizeof(float_t) * vertex_count_ * 3);

	clone->face_count_ = face_count_;
	clone->faces_size_ = faces_size_;
	clone->faces_ = new ubyte_t[faces_size_];
	memcpy(clone->faces_, faces_, faces_size_);

	for (
		children_container_t::const_iterator begin = children_.begin();
		begin != children_.end();
		++begin
	)
	{
		clone->children_.push_back((*begin)->Clone());
	}

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem �ber info gegebenen Input.
*/
void Mesh::ReadDefinition(
	TemporalInfo& info
)
{
	// DWORD nVertices;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nVertices;"));
	}

	// array Vector vertices[nVertices];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeName, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array Vector vertices[nVertices];"));
	}

	// DWORD nFaces;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nFaces;"));
	}

	// array MeshFace faces[nFaces];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeName, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array MeshFace faces[nFaces];"));
	}

	// [...]
	{
		Token::Type decl[] = { Token::TypeOBracket, Token::TypeDot, Token::TypeDot, Token::TypeDot, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 5, UNICODE_STRING("array MeshFace faces[nFaces];"));
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template-Daten lesen
	\param name_token
	\param guid_token
	\param info
	\param read_cbrace

	Liest Daten dieses Templates.
*/
void Mesh::ReadData(
	TokenName* name_token,
	TokenGuid*,
	TemporalInfo& info,
	bool_t read_cbrace
)
{
	if (!read_cbrace)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("template 'Mesh' cannot be included without reading the final CBRACKET-token!");
		return;
	}

	/*
	 *	Ev. Name auslesen falls vorhanden
	 */
	if (name_token != NULL)
		name_ = name_token->GetName();


	/*
	 *	Anzahl Vertices auslesen. 
	 */
	vertex_count_ = get_next_dword(info);
	SAFE_DELETE_ARRAY(vertices_);
	vertices_ = new ubyte_t[sizeof(float_t) * vertex_count_ * 3];
	float_t* vertex_data_ptr = (float_t*)vertices_;
	for (udword_t i = 0; i < vertex_count_; ++i)
	{
		vertex_data_ptr[i*3+0] = get_next_float32(info);
		vertex_data_ptr[i*3+1] = get_next_float32(info);
		vertex_data_ptr[i*3+2] = get_next_float32(info);
	}

	/*
	 *	Anzahl faces auslesen. Danach gleich alle faces direkt auch auslesen
	 */
	face_count_ = get_next_dword(info);
	vector<udword_t> faces;
	for (udword_t i = 0; i < face_count_; ++i)
	{
		udword_t index_count = get_next_dword(info);
		faces.push_back(index_count);
		for (udword_t j = 0; j < index_count; ++j)
			faces.push_back(get_next_dword(info));
	}
	faces_size_ = sizeof(udword_t) * (udword_t)faces.size();
	SAFE_DELETE_ARRAY(faces_);
	faces_ = new ubyte_t[faces_size_];
	memcpy(faces_, &faces[0], faces_size_);

	/*
	 *	[...] bedeutet "open-end", d.h. es kann eigentlich alles noch als Child-Element
	 *	dieses Templates kommen...
	 */
	bool_t do_continue = true;
	while (!info.error_flag && do_continue)
	{
		Token* next_token = info.scanner->ReadToken(info);

		switch (next_token->GetType())
		{
		case Token::TypeName:
			children_.push_back(read_template_data(next_token, info));
			break;

		case Token::TypeCBrace:
			do_continue = false;
			break;

		default:
			info.error_flag = true;
			info.error_message += UNICODE_STRING("Unexpected Token!");
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void Mesh::WriteData(
	writer::Writer* writer,
	bool_t* error_flag,
	string_t* error_message
)
{
	if (*error_flag)
		{ return; }

	// write vertices and faces as embedded templates

	bool_t write_error = false;

	write_error = write_error || !writer->WriteNameToken("Mesh");
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeOBrace);
	write_error = write_error || !writer->WriteNewLineHint();
	
	// write array
	write_error = write_error || !writer->WriteTokenDWord(vertex_count_);
	write_error = write_error || !writer->WriteNewLineHint();

	float_t* vertex_data_ptr = (float_t*)vertices_;
	for (udword_t i = 0; i < vertex_count_; ++i)
	{
		write_error = write_error || !writer->WriteTokenFloat(vertex_data_ptr[i*3+0]);
		write_error = write_error || !writer->WriteTokenFloat(vertex_data_ptr[i*3+1]);
		write_error = write_error || !writer->WriteTokenFloat(vertex_data_ptr[i*3+2]);
		if (i < (vertex_count_ - 1))
		{
			write_error = write_error || !writer->WriteStandaloneToken(Token::TypeComma);
			write_error = write_error || !writer->WriteNewLineHint();
		}
	}
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeSemicolon); // close array
	write_error = write_error || !writer->WriteNewLineHint();

	write_error = write_error || !writer->WriteTokenDWord(face_count_);
	write_error = write_error || !writer->WriteNewLineHint();

	udword_t faces_index = 0;
	udword_t* faces = reinterpret_cast<udword_t*>(faces_);
	vector<udword_t> buffer;
	for (udword_t i = 0; i < face_count_; ++i)
	{
		udword_t index_count = faces[faces_index];
		
		while (buffer.size() < index_count)
			{ buffer.push_back(0); }

		for (udword_t j = 0; j < index_count; ++j)
			{ buffer[j] = faces[faces_index + 1 + j]; }

		write_error = write_error || !writer->WriteTokenDWord(index_count);
		write_error = write_error || !writer->WriteTokenDWordList(&buffer[0], index_count);

		if (i < face_count_ - 1)
		{
			write_error = write_error || !writer->WriteStandaloneToken(Token::TypeComma);
			write_error = write_error || !writer->WriteNewLineHint();
		}

		faces_index += (index_count + 1);
	}
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeSemicolon); // close array
	write_error = write_error || !writer->WriteNewLineHint();

	for (uint_t i = 0; i < children_.size() && !write_error; ++i)
	{
		if (children_[i])
			{ children_[i]->WriteData(writer, error_flag, error_message); }

		write_error = write_error || (*error_flag);
	}

	// close this template
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeCBrace);
	write_error = write_error || !writer->WriteNewLineHint();

	if (write_error)
	{
		*error_flag = true;
		*error_message += UNICODE_STRING("error while writing frame template.\n");
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Namen zur�ckgeben
	\return Name

	Gibt den Namen dieses Meshes zur�ck.
*/
const string_t& Mesh::GetName() const
{
	return name_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Anzahl Vertices zur�ckgeben
	\return Anzahl

	Gibt die Anzahl Vertices dieses Meshes zur�ck.
*/
const uint_t Mesh::GetVertexCount() const
{
	return (uint_t)vertex_count_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Vertices zur�ckgeben
	\return Referenz auf den Vektor mit den Vertices

	Gibt die Vertices dieses Meshes zur�ck. Jeder Vertex besteht aus einem
	Positionsvektor.
*/
const ubyte_t* Mesh::GetVertices() const
{
	return vertices_;
}





//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Anzahl faces zur�ckgeben
	\return Anzahl

	Gibt die Anzahl faces dieses meshes zur�ck.
*/
const uint_t Mesh::GetFaceCount() const
{
	return (uint_t)face_count_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief faces zur�ckgeben
	\return Referenz auf den Vektor mit den Fl�chen

	Gibt die Oberfl�chenangaben dieses Meshes zur�ck.
*/
const ubyte_t* Mesh::GetFaces() const
{
	return faces_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Children zur�ckgeben
	\return Referenz auf den Vektor mit den Children

	Gibt alle Child-Elemente dieses Meshes zur�ck. Dies k�nnten z.B. Normalvektoren, etc. sein
*/
const Mesh::children_container_t& Mesh::GetChildren() const
{
	return children_;
}

}} // namespace x::templates


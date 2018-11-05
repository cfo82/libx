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
/*!	\file templates/xskinmeshheader.cpp
	\brief Implementation von x::templates::XSkinMeshHeader
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::XSkinMeshHeader
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
XSkinMeshHeader::XSkinMeshHeader()
:	Template(TemplateTypeXSkinMeshHeader),
	max_skin_weights_per_vertex_(0),
	max_skin_weights_per_face_(0),
	num_bones_(0)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
XSkinMeshHeader::~XSkinMeshHeader()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void XSkinMeshHeader::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << UNICODE_STRING("XSkinMeshHeader") << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* XSkinMeshHeader::Clone() const
{
	XSkinMeshHeader* clone = new XSkinMeshHeader();
	clone->max_skin_weights_per_vertex_ = max_skin_weights_per_vertex_;
	clone->max_skin_weights_per_face_ = max_skin_weights_per_face_;
	clone->num_bones_ = num_bones_;
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input. Definition 
	des Templates gemäss DirectX-Dokumentation:

	\verbatim
		template XSkinMeshHeader
		{
			< 3CF169CE-FF7C-44ab-93C0-F78F62D172E2 >
			WORD nMaxSkinWeightsPerVertex;
			WORD nMaxSkinWeightsPerFace;
			WORD nBones;
		}
	\endverbatim
*/
void XSkinMeshHeader::ReadDefinition(
	TemporalInfo& info
)
{
	// WORD nMaxSkinWeightsPerVertex;
	{
		Token::Type decl[] = { Token::TypeWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("WORD nMaxSkinWeightsPerVertex;"));
	}

	// WORD nMaxSkinWeightsPerFace;
	{
		Token::Type decl[] = { Token::TypeWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("WORD nMaxSkinWeightsPerFace;"));
	}

	// WORD nBones;
	{
		Token::Type decl[] = { Token::TypeWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("WORD nBones;"));
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
void XSkinMeshHeader::ReadData(
	TokenName*,
	TokenGuid*,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	max_skin_weights_per_vertex_ = get_next_dword(info);
	max_skin_weights_per_face_ = get_next_dword(info);
	num_bones_ = get_next_dword(info);

	if (read_cbrace && !info.error_flag)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}




//-----------------------------------------------------------------------------------------------------------
void XSkinMeshHeader::WriteData(
	writer::Writer* writer,
	bool_t* error_flag,
	string_t* error_message
)
{
	if (*error_flag)
		{ return; }

	// write vertices and faces as embedded templates

	bool_t write_error = false;

	write_error = write_error || !writer->WriteNameToken("XSkinMeshHeader");
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeOBrace);
	write_error = write_error || !writer->WriteNewLineHint();
	
	// write array
	write_error = write_error || !writer->WriteTokenDWord(max_skin_weights_per_vertex_);
	write_error = write_error || !writer->WriteNewLineHint();
	write_error = write_error || !writer->WriteTokenDWord(max_skin_weights_per_face_);
	write_error = write_error || !writer->WriteNewLineHint();
	write_error = write_error || !writer->WriteTokenDWord(num_bones_);
	write_error = write_error || !writer->WriteNewLineHint();

	// close this template
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeCBrace);
	write_error = write_error || !writer->WriteNewLineHint();

	if (write_error)
	{
		*error_flag = true;
		*error_message += UNICODE_STRING("error while writing frame template.\n");
	}
}

}} // namespace x::templates


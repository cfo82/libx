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
/*!	\file templates/meshtexturecoords.cpp
	\brief Implementation von x::templates::MeshTextureCoords
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::MeshTextureCoords
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
MeshTextureCoords::MeshTextureCoords()
:	Template(TemplateTypeMeshTextureCoords),
	texture_coords_count_(0),
	texture_coords_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
MeshTextureCoords::~MeshTextureCoords()
{
	for (
		coords_container_t::iterator begin = texture_coords_.begin();
		begin != texture_coords_.end();
		++begin
	)
	{
		Coords2d* coords = (*begin);
		coords->RemRef();
	}
	texture_coords_.clear();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void MeshTextureCoords::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "MeshTextureCoords" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* MeshTextureCoords::Clone() const
{
	MeshTextureCoords* clone = new MeshTextureCoords;

	// Koordinaten klonen
	clone->texture_coords_count_ = texture_coords_count_;
	for (
		vector<Coords2d*>::const_iterator begin = texture_coords_.begin();
		begin != texture_coords_.end();
		++begin
	)
	{
		clone->texture_coords_.push_back(static_cast<Coords2d*>((*begin)->Clone()));
	}

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void MeshTextureCoords::ReadDefinition(
	TemporalInfo& info
)
{
	// DWORD nTextureCoords;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nTextureCoords;"));
	}

	// array Coords2d textureCoords[nTextureCoords];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeName, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array Coords2d textureCoords[nTextureCoords];"));
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
void MeshTextureCoords::ReadData(
	TokenName*,
	TokenGuid*,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	/*
	 *	Anzahl Normalvektoren auslesen. Danach alle Normalvektoren direkt
	 *	auslesen
	 */
	texture_coords_.clear();
	texture_coords_count_ = get_next_dword(info);
	for (udword_t i = 0; i < texture_coords_count_; ++i)
	{
		Coords2d* coords = static_cast<Coords2d*>(instanciate_template_by_name(info, UNICODE_STRING("Coords2d")));
		coords->ReadData(NULL, NULL, info, false);
		texture_coords_.push_back(coords);
	}

	/*
	 *	Das nächste Token muss die Daten abschliessen, falls es überhaupt gelesen werden muss
	 */
	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
		if (info.error_flag)
		{
			// TODO: ERROR CASE
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void MeshTextureCoords::WriteData(
	writer::Writer* writer,
	bool_t* error_flag,
	string_t* error_message
)
{
	if (*error_flag)
		{ return; }

	// write vertices and faces as embedded templates

	bool_t write_error = false;

	write_error = write_error || !writer->WriteNameToken("MeshTextureCoords");
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeOBrace);
	write_error = write_error || !writer->WriteNewLineHint();
	
	// write array
	write_error = write_error || !writer->WriteTokenDWord(texture_coords_count_);
	write_error = write_error || !writer->WriteNewLineHint();

	for (udword_t i = 0; i < texture_coords_count_; ++i)
	{
		Coords2d* coords = texture_coords_[i];
		const math::Vector2f& v = coords->GetVector();

		write_error = write_error || !writer->WriteTokenFloat(v.x);
		write_error = write_error || !writer->WriteTokenFloat(v.y);

		if (i < (texture_coords_count_ - 1))
		{
			write_error = write_error || !writer->WriteStandaloneToken(Token::TypeComma);
			write_error = write_error || !writer->WriteNewLineHint();
		}
	}
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeSemicolon); // close array
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




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Anzahl Texturkoordinaten zurückgeben
	\return Anzahl

	Gibt die Anzahl Texturkoordinaten dieses Templates zurück.
*/
const uint_t MeshTextureCoords::GetTextureCoordsCount() const
{
	return (uint_t)texture_coords_count_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Texturkoordinaten zurückgeben
	\return Referenz auf den Vektor mit den Koordinaten

	Gibt die Texturkoordinaten dieses Templates zurück. 
*/
const vector<Coords2d*>& MeshTextureCoords::GetTextureCoords() const
{
	return texture_coords_;
}

}} // namespace x::templates


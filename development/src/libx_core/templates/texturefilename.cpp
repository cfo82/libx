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
/*!	\file templates/texturefilename.cpp
	\brief Implementation von x::templates::TextureFilename
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::TextureFilename
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
TextureFilename::TextureFilename()
:	Template(TemplateTypeTextureFilename),
	value_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
TextureFilename::~TextureFilename()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void TextureFilename::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "TextureFilename" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* TextureFilename::Clone() const
{
	TextureFilename* clone = new TextureFilename;
	clone->value_ = value_;
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void TextureFilename::ReadDefinition(
	TemporalInfo& info
)
{
	// string filename;
	{
		Token::Type decl[] = { Token::TypeLpStr, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("string filename;"));
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
void TextureFilename::ReadData(
	TokenName*,
	TokenGuid*,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	Token* value_token = info.scanner->ReadExpectedToken(info, Token::TypeString);
	if (!value_token)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Token::TypeString expected!\n");
	}
	value_ = static_cast<TokenString*>(value_token)->GetValue();

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
void TextureFilename::WriteData(
	writer::Writer* writer,
	bool_t* error_flag,
	string_t* error_message
)
{
	if (*error_flag)
		{ return; }

	bool_t write_error = false;

	write_error = write_error || !writer->WriteNameToken("TextureFilename");
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeOBrace);
	write_error = write_error || !writer->WriteNewLineHint();

	write_error = write_error || !writer->WriteTokenString(value_);
	write_error = write_error || !writer->WriteNewLineHint();

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
	\brief Dateinamen zurückgeben
	\return Dateiname der Textur

	Gibt den Dateinamen der Textur zurück.
*/
const string_t& TextureFilename::GetFilename() const
{
	return value_;
}

}} // namespace x::templates


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
/*!	\file templates/headertemplate.cpp
	\brief Implementation von x::templates::HeaderTemplate
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::HeaderTemplate
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
HeaderTemplate::HeaderTemplate()
:	Template(TemplateTypeHeader),
	major_(0),
	minor_(0),
	flags_(0)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
HeaderTemplate::~HeaderTemplate()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void HeaderTemplate::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "Header" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* HeaderTemplate::Clone() const
{
	HeaderTemplate* clone = new HeaderTemplate;
	clone->major_ = major_;
	clone->minor_ = minor_;
	clone->flags_ = flags_;
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void HeaderTemplate::ReadDefinition(
	TemporalInfo& info
) 
{
	// WORD major;
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("WORD major;"));
	}

	// WORD minor;
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("WORD minor;"));
	}

	// DWORD flags;
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD flags;"));
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
void HeaderTemplate::ReadData(
	TokenName*,
	TokenGuid*,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	major_ = get_next_dword(info);
	minor_ = get_next_dword(info);
	flags_ = get_next_dword(info);

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
		if (info.error_flag)
		{
			// TODO: ERROR CASE
		}
	}
}

}} // namespace x::templates


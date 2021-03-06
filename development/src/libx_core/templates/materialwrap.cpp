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
/*!	\file templates/materialwrap.cpp
	\brief Implementation von x::templates::MaterialWrap
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::MaterialWrap
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
MaterialWrap::MaterialWrap()
:	Template(TemplateTypeMaterialWrap),
	u_(NULL),
	v_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
MaterialWrap::~MaterialWrap()
{
	SAFE_REMREF(u_);
	SAFE_REMREF(v_);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template geh�rt aus.
*/
void MaterialWrap::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "MaterialWrap" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zur�ck.
*/
Template* MaterialWrap::Clone() const
{
	MaterialWrap* clone = new MaterialWrap;
	if (u_)
		clone->u_ = static_cast<Boolean*>(u_->Clone());
	if (v_)
		clone->v_ = static_cast<Boolean*>(v_->Clone());
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem �ber info gegebenen Input.
*/
void MaterialWrap::ReadDefinition(
	TemporalInfo& info
)
{
	// Boolean u;
	{
		Token::Type decl[] = { Token::TypeName, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("Boolean u;"));
	}

	// Boolean v;
	{
		Token::Type decl[] = { Token::TypeName, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("Boolean v;"));
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
void MaterialWrap::ReadData(
	TokenName*,
	TokenGuid*,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	u_ = static_cast<Boolean*>(instanciate_template_by_name(info, UNICODE_STRING("Boolean")));
	u_->ReadData(NULL, NULL, info, false);
	v_ = static_cast<Boolean*>(instanciate_template_by_name(info, UNICODE_STRING("Boolean")));
	v_->ReadData(NULL, NULL, info, false);

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

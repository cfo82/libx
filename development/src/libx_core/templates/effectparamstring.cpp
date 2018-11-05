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
/*!	\file templates/effectparamstring.cpp
	\brief Implementation von x::templates::EffectParamString
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::EffectParamString
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
EffectParamString::EffectParamString()
:	Template(TemplateTypeEffectParamString),
	param_name_(),
	value_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
EffectParamString::~EffectParamString()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void EffectParamString::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "EffectParamString" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* EffectParamString::Clone() const
{
	EffectParamString* clone = new EffectParamString;
	clone->param_name_ = param_name_;
	clone->value_ = value_;
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void EffectParamString::ReadDefinition(
	TemporalInfo& info
) 
{
	// STRING ParamName;
	{
		Token::Type decl[] = { Token::TypeLpStr, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("STRING ParamName;"));
	}

	// STRING Value;
	{
		Token::Type decl[] = { Token::TypeLpStr, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("STRING Value;"));
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
void EffectParamString::ReadData(
	TokenName*,
	TokenGuid*,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	Token* param_name_token = info.scanner->ReadToken(info);
	if (!param_name_token || param_name_token->GetType() != Token::TypeString)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Token::TypeString expected!");
		return;
	}
	param_name_ = static_cast<TokenString*>(param_name_token)->GetValue();

	Token* value_token = info.scanner->ReadToken(info);
	if (!value_token || value_token->GetType() != Token::TypeString)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Token::TypeString expected!");
		return;
	}
	value_ = static_cast<TokenString*>(value_token)->GetValue();

	if (read_cbrace && !info.error_flag)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}

}} // namespace x::templates


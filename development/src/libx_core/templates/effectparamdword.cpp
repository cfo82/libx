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
/*!	\file templates/effectparamdword.cpp
	\brief Implementation von x::templates::EffectParamDWord
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::EffectParamDWord
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
EffectParamDWord::EffectParamDWord()
:	Template(TemplateTypeEffectParamDWord),
	param_name_(),
	value_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
EffectParamDWord::~EffectParamDWord()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void EffectParamDWord::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "EffectParamDWord" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* EffectParamDWord::Clone() const
{
	EffectParamDWord* clone = new EffectParamDWord;
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
void EffectParamDWord::ReadDefinition(
	TemporalInfo& info
) 
{
	// STRING ParamName;
	{
		Token::Type decl[] = { Token::TypeLpStr, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("STRING ParamName;"));
	}

	// DWORD Value;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD Value;"));
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
void EffectParamDWord::ReadData(
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
	value_ = get_next_dword(info);

	if (read_cbrace && !info.error_flag)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}

}} // namespace x::templates


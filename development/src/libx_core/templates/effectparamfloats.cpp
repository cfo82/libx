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
/*!	\file templates/effectparamfloats.cpp
	\brief Implementation von x::templates::EffectParamFloats
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::EffectParamFloats
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
EffectParamFloats::EffectParamFloats()
:	Template(TemplateTypeEffectParamFloats),
	param_name_(),
	float_count_(0),
	floats_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
EffectParamFloats::~EffectParamFloats()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void EffectParamFloats::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "EffectParamFloats" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* EffectParamFloats::Clone() const
{
	EffectParamFloats* clone = new EffectParamFloats;

	// Koordinaten klonen
	clone->param_name_ = param_name_;
	clone->float_count_ = float_count_;
	for (
		vector<float_t>::const_iterator begin = floats_.begin();
		begin != floats_.end();
		++begin
	)
	{
		clone->floats_.push_back(*begin);
	}

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void EffectParamFloats::ReadDefinition(
	TemporalInfo& info
) 
{
	// STRING ParamName;
	{
		Token::Type decl[] = { Token::TypeLpStr, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("STRING ParamName;"));
	}

	// DWORD nFloats;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nFloats;"));
	}

	// array float Floats[nFloats];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeFloat, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("DWORD nFloats;"));
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
void EffectParamFloats::ReadData(
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
	float_count_ = get_next_dword(info);
	for (udword_t i = 0; i < float_count_; ++i)
		floats_.push_back(get_next_float32(info));

	/*
	 *	Das nächste Token muss die Daten abschliessen, falls es überhaupt gelesen werden muss
	 */
	if (read_cbrace && !info.error_flag)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}

}} // namespace x::templates


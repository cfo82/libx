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
/*!	\file templates/effectinstance.cpp
	\brief Implementation von x::templates::EffectInstance
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::EffectInstance
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
EffectInstance::EffectInstance()
:	Template(TemplateTypeEffectInstance),
	effect_filename_(),
	children_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
EffectInstance::~EffectInstance()
{
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

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void EffectInstance::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "EffectInstance" << endl;
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

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* EffectInstance::Clone() const
{
	EffectInstance* clone = new EffectInstance;

	/*
	 *	Zuerst werden alle Attribute sofern vorhanden kopiert
	 */
	clone->effect_filename_ = effect_filename_;

	/*
	 *	Danach alle Children
	 */
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

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void EffectInstance::ReadDefinition(
	TemporalInfo& info
)
{
	// STRING EffectFilename;
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeLpStr, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("STRING EffectFilename;"));
	}

	// [...]
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeOBracket, Token::TypeDot, Token::TypeDot, Token::TypeDot, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 5, UNICODE_STRING("[...]"));
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
void EffectInstance::ReadData(
	TokenName*,
	TokenGuid*,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	if (!read_cbrace)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("template 'EffectInstance' cannot be included without reading the final CBRACKET-token!");
		return;
	}

	Token* value_token = info.scanner->ReadToken(info);
	if (value_token == NULL || value_token->GetType() != Token::TypeString)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Token::TypeString expected!");
		return;
	}

	effect_filename_ = static_cast<TokenString*>(value_token)->GetValue();

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
			info.error_message += UNICODE_STRING("Unexpected Token");
		}
	}
}

}} // namespace x::templates


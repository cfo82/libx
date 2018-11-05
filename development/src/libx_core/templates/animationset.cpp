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
/*!	\file templates/animationset.cpp
	\brief Implementation von x::templates::AnimationSet
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::AnimationSet
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
AnimationSet::AnimationSet()
:	Template(TemplateTypeAnimationSet),
	animations_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
AnimationSet::~AnimationSet()
{
	typedef animation_container_t::iterator iterator;
	for (iterator begin = animations_.begin(); begin != animations_.end(); ++begin)
	{
		Animation* animation = (*begin);
		animation->RemRef();
	}
	animations_.clear();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* AnimationSet::Clone() const
{
	AnimationSet* clone = new AnimationSet;

	typedef animation_container_t::const_iterator iterator;
	for (iterator begin = animations_.begin(); begin != animations_.end(); ++begin)
	{
		clone->animations_.push_back(static_cast<Animation*>((*begin)->Clone()));
	}

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input. Definition 
	des Templates gemäss DirectX-Dokumentation:

	\verbatim
		template AnimationSet
		{
			< 3D82AB50-62DA-11cf-AB39-0020AF71E433 >
			[ Animation < 3D82AB4F-62DA-11cf-AB39-0020AF71E433 > ]
		}
	\endverbatim
*/
void AnimationSet::ReadDefinition(
	TemporalInfo& info
)
{
	// [ Animation < 3D82AB4F-62DA-11cf-AB39-0020AF71E433 > ]
	{
		Token* obracket_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);
		Token* guid_token = NULL;
		Token* cbracket_token = info.scanner->ReadToken(info);
		if (cbracket_token->GetType() == Token::TypeGuid)
		{
			guid_token = cbracket_token;
			cbracket_token = info.scanner->ReadToken(info);
		}

		if (obracket_token->GetType() != Token::TypeOBracket ||
			name_token->GetType() != Token::TypeName ||
			(guid_token != NULL && guid_token->GetType() != Token::TypeGuid) ||
			cbracket_token->GetType() != Token::TypeCBracket)
		{
			info.error_flag = true;
			info.error_message += UNICODE_STRING("Error in definition of '[ Animation < 3D82AB4F-62DA-11cf-AB39-0020AF71E433 > ]'.\n");
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void AnimationSet::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "AnimationSet" << endl;
	typedef animation_container_t::const_iterator iterator;
	for (iterator begin = animations_.begin(); begin != animations_.end(); ++begin)
	{
		(*begin)->PrintFileStructure(level + 1);
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
void AnimationSet::ReadData(
	TokenName* name_token,
	TokenGuid*,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	if (!read_cbrace)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("template 'AnimationSet' cannot be included without reading the final CBRACKET-token!");
		return;
	}

	if (name_token)
	{
		name_ = name_token->GetName();
	}
	
	/*
	 *	[ Animation < 3D82AB4F-62DA-11cf-AB39-0020AF71E433 > ]
	 */
	bool_t coninue_flag = true;
	while (coninue_flag)
	{
		Token* next_token = info.scanner->ReadToken(info);
		if (!next_token)
		{
			info.error_flag = true;
			info.error_message += UNICODE_STRING("Missing next token.\n");
			return;
		}

		switch (next_token->GetType())
		{
		case Token::TypeName: {
			Template* animation = read_template_data(next_token, info);
			if (animation == NULL || animation->GetType() != TemplateTypeAnimation)
			{
				info.error_flag = true;
				info.error_message += UNICODE_STRING("'Animation'-template expected.\n");
				return;
			}
			animations_.push_back(static_cast<Animation*>(animation));
			break;
		}

		case Token::TypeCBrace:
			coninue_flag = false;
			break;

		default:
			info.error_flag = true;
			info.error_message += UNICODE_STRING("Unexpected token occured.\n");
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void AnimationSet::WriteData(
	writer::Writer* writer,
	bool_t* error_flag,
	string_t* error_message
)
{
	if (*error_flag)
		{ return; }

	bool_t write_error = false;

	write_error = write_error || !writer->WriteNameToken("AnimationSet");
	if (name_.length() > 0)
		{ write_error = write_error || !writer->WriteNameToken(name_); }
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeOBrace);
	write_error = write_error || !writer->WriteNewLineHint();

	for (uint_t i = 0; i < animations_.size() && !write_error; ++i)
	{
		if (animations_[i])
			{ animations_[i]->WriteData(writer, error_flag, error_message); }

		write_error = write_error || (*error_flag);
	}

	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeCBrace);
	write_error = write_error || !writer->WriteNewLineHint();

	if (write_error)
	{
		*error_flag = true;
		*error_message += UNICODE_STRING("error while writing frame template.\n");
	}
}




//-----------------------------------------------------------------------------------------------------------
const string_t& AnimationSet::GetName() const
{
	return name_;
}





//-----------------------------------------------------------------------------------------------------------
const AnimationSet::animation_container_t& AnimationSet::GetAnimations() const
{
	return animations_;
}

}} // namespace x::templates


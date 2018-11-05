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
/*!	\file templates/animationoptions.cpp
	\brief Implementation von x::templates::AnimationOptions
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::AnimationOptions
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
AnimationOptions::AnimationOptions()
:	Template(TemplateTypeAnimationOptions),
	open_closed_(0),
	position_quality_(0)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
AnimationOptions::~AnimationOptions()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void AnimationOptions::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "AnimationOptions" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* AnimationOptions::Clone() const
{
	AnimationOptions* clone = new AnimationOptions;
	clone->open_closed_ = open_closed_;
	clone->position_quality_ = position_quality_;
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input. Definition 
	des Templates gemäss DirectX-Dokumentation:

	\verbatim
		template AnimationOptions
		{
			< E2BF56C0-840F-11cf-8F52-0040333594A3 >
			DWORD openclosed;
			DWORD positionquality;
		} 
	\endverbatim
*/
void AnimationOptions::ReadDefinition(
	TemporalInfo& info
)
{
	// DWORD openclosed;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD openclosed;"));
	}

	// DWORD nKeys;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nKeys;"));
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
void AnimationOptions::ReadData(
	TokenName*,
	TokenGuid*,
	TemporalInfo& info,
	bool_t read_cbrace
)
{
	// Ev. Name auslesen falls vorhanden
	open_closed_ = get_next_dword(info);
	position_quality_ = get_next_dword(info);

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
const dword_t AnimationOptions::GetOpenClosed() const
{
	return open_closed_;
}




//-----------------------------------------------------------------------------------------------------------
const dword_t AnimationOptions::GetPositionQuality() const
{
	return position_quality_;
}

}} // namespace x::templates


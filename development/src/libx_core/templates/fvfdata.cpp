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
/*!	\file templates/fvfdata.cpp
	\brief Implementation von x::templates::FVFData
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::FVFData
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
FVFData::FVFData()
:	Template(TemplateTypeFVFData)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
FVFData::~FVFData()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void FVFData::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "FVFData" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses TemplateTypes und gibt sie zurück.
*/
Template* FVFData::Clone() const
{
	FVFData* clone = new FVFData;
	clone->fvf_ = fvf_;
	clone->dword_count_ = dword_count_;
	for (
		vector<udword_t>::const_iterator begin = data_.begin();
		begin != data_.end();
		++begin
	)
	{
		clone->data_.push_back(*begin);
	}
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void FVFData::ReadDefinition(
	TemporalInfo& info
) 
{
	// DWORD dwFVF;
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD dwFVF;"));
	}

	// DWORD nDWords;
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nDWords;"));
	}

	// array DWORD data[nDWords];
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeDWord, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array DWORD data[nDWords];"));
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template-Daten lesen
	\param name_token
	\param guid_token
	\param info
	\param read_cbrace

	Liest Daten dieses TemplateTypes.
*/
void FVFData::ReadData(
	TokenName*,
	TokenGuid*,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	fvf_ = get_next_dword(info);
	dword_count_ = get_next_dword(info);
	data_.clear();
	for (udword_t i = 0; i < dword_count_; ++i)
		data_.push_back(get_next_dword(info));

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


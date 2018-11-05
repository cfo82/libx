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
/*!	\file scanner/scanner.cpp
	\brief Implementation von x::scanner::Scanner
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::scanner::Scanner
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "./scanner.h"

namespace x { namespace scanner {

using namespace x::tokens;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Scanner::Scanner()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Scanner::~Scanner()
{
}




//-------------------------------------------------------------------------------------------------------
Token* Scanner::ReadExpectedToken(
	TemporalInfo& info,
	const Token::Type type
)
{
	Token* token = ReadToken(info);
	if (token->GetType() != type)
	{
		string_t type_string;
		switch (type)
		{
		case Token::TypeName: type_string = UNICODE_STRING("Token::TypeName"); break;
		case Token::TypeString: type_string = UNICODE_STRING("Token::TypeString"); break;
		case Token::TypeInteger: type_string = UNICODE_STRING("Token::TypeInteger"); break;
		case Token::TypeGuid: type_string = UNICODE_STRING("Token::TypeGuid"); break;
		case Token::TypeIntegerList: type_string = UNICODE_STRING("Token::TypeIntegerList"); break;
		case Token::TypeFloatList: type_string = UNICODE_STRING("Token::TypeFloatList"); break;
		case Token::TypeOBrace: type_string = UNICODE_STRING("Token::TypeOBrace"); break;
		case Token::TypeCBrace: type_string = UNICODE_STRING("Token::TypeCBrace"); break;
		case Token::TypeOParen: type_string = UNICODE_STRING("Token::TypeOParen"); break;
		case Token::TypeCParen: type_string = UNICODE_STRING("Token::TypeCParen"); break;
		case Token::TypeOBracket: type_string = UNICODE_STRING("Token::TypeOBracket"); break;
		case Token::TypeCBracket: type_string = UNICODE_STRING("Token::TypeCBracket"); break;
		case Token::TypeOAngle: type_string = UNICODE_STRING("Token::TypeOAngle"); break;
		case Token::TypeCAngle: type_string = UNICODE_STRING("Token::TypeCAngle"); break;
		case Token::TypeDot: type_string = UNICODE_STRING("Token::TypeDot"); break;
		case Token::TypeComma: type_string = UNICODE_STRING("Token::TypeComma"); break;
		case Token::TypeSemicolon: type_string = UNICODE_STRING("Token::TypeSemicolon"); break;
		case Token::TypeTemplate: type_string = UNICODE_STRING("Token::TypeTemplate"); break;
		case Token::TypeWord: type_string = UNICODE_STRING("Token::TypeWord"); break;
		case Token::TypeDWord: type_string = UNICODE_STRING("Token::TypeDWord"); break;
		case Token::TypeFloat: type_string = UNICODE_STRING("Token::TypeFloat"); break;
		case Token::TypeDouble: type_string = UNICODE_STRING("Token::TypeDouble"); break;
		case Token::TypeChar: type_string = UNICODE_STRING("Token::TypeChar"); break;
		case Token::TypeUChar: type_string = UNICODE_STRING("Token::TypeUChar"); break;
		case Token::TypeSWord: type_string = UNICODE_STRING("Token::TypeSWord"); break;
		case Token::TypeSDWord: type_string = UNICODE_STRING("Token::TypeSDWord"); break;
		case Token::TypeVoid: type_string = UNICODE_STRING("Token::TypeVoid"); break;
		case Token::TypeLpStr: type_string = UNICODE_STRING("Token::TypeLpStr"); break;
		case Token::TypeUnicode: type_string = UNICODE_STRING("Token::TypeUnicode"); break;
		case Token::TypeCString: type_string = UNICODE_STRING("Token::TypeCString"); break;
		case Token::TypeArray: type_string = UNICODE_STRING("Token::TypeArray"); break;
		}

		info.error_flag = true;
		info.error_message += type_string + UNICODE_STRING("-token expected.\n");
		token = NULL;
	}
	return token;
}




//-------------------------------------------------------------------------------------------------------
void Scanner::ReadDeclaration(
	TemporalInfo& info,
	const Token::Type decl[],
	const uint_t decl_size,
	const string_t& decl_string
)
{
	for (uint_t i = 0; i < decl_size; ++i)
	{
		ReadExpectedToken(info, decl[i]);
	}

	if (info.error_flag)
	{
		info.error_message += string_t(UNICODE_STRING("error in definition of '")) + decl_string + UNICODE_STRING("'.\n");
	}
}

}} // namespace x::scanner


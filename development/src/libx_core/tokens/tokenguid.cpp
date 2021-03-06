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
/*!	\file tokens/tokenguid.cpp
	\brief Implementation von x::TokenGuid
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::TokenGuid
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../serializer.h"
#include "./tokenguid.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenGuid::TokenGuid()
:	Token(Token::TypeGuid),
	guid_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenGuid::TokenGuid(
	const uint_t line
)
:	Token(Token::TypeGuid, line),
	guid_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenGuid::TokenGuid(
	const Guid& guid
)
:	Token(Token::TypeGuid),
	guid_(guid)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenGuid::TokenGuid(
	const Guid& guid,
	const uint_t line
)
:	Token(Token::TypeGuid, line),
	guid_(guid)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
TokenGuid::~TokenGuid()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief GUID zur�ckgeben
	\return Guid

	Gibt die diesem Token zugeh�rige GUID zur�ck.
*/
const Guid& TokenGuid::GetGuid() const
{
	return guid_;
}

}} // namespace x::tokens


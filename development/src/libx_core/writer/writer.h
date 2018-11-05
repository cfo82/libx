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
/*!	\file writer/writer.h
	\brief Definition von x::writer::Writer
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::writer::Writer
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef WRITER_WRITER_H
#define WRITER_WRITER_H

#include "../global/global.h"
#include "../tokens/tokens.h"

namespace x { namespace writer {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief X-File-Scanner
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Klasse fürs Handling von Tokens
*/
class Writer
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	Writer();
	virtual ~Writer();

	//-------------------------------------------------------------------------------------------------------
	virtual bool_t WriteHeader() = 0;
	virtual bool_t WriteNameToken(const string_t& name) = 0;
	virtual bool_t WriteStandaloneToken(const tokens::Token::Type type) = 0;
	virtual bool_t WriteTokenDWord(const udword_t value) = 0;
	virtual bool_t WriteTokenDWordList(const udword_t* list, const uint_t element_count) = 0;
	virtual bool_t WriteTokenFloat(const float_t value) = 0;
	virtual bool_t WriteTokenFloatList(const float_t* list, const uint_t element_count) = 0;
	virtual bool_t WriteTokenString(const string_t& value) = 0;
	virtual bool_t WriteNewLineHint() = 0;
	//virtual tokens::Token* ReadToken(TemporalInfo& info) = 0;
	//virtual tokens::Token* ReadExpectedToken(TemporalInfo& info, const tokens::Token::Type type);
	//virtual tokens::Token* ReadOptionalToken(TemporalInfo& info, const tokens::Token::Type type) = 0;
	//virtual void ReadDeclaration(TemporalInfo& info, const tokens::Token::Type decl[], const uint_t decl_size, const string_t& decl_string);
};

}} // namespace x::writer

#endif // WRITER_WRITER_H

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
/*!	\file writer/textwriter.cpp
	\brief Implementation von x::writer::TextWriter
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::writer::TextWriter
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "./textwriter.h"
#include <sstream>

namespace x { namespace writer {

using namespace x::tokens;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TextWriter::TextWriter(
	io_write_function_t write_function,
	void* io_token
)
:	write_function_(write_function),
	io_token_(io_token),
	indent_(),
	needs_indent_(true)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
TextWriter::~TextWriter()
{
}




//-------------------------------------------------------------------------------------------------------
bool_t TextWriter::WriteHeader()
{
	bool_t error = !WriteText("xof ", 4);   // magic identifier
	error = error || !WriteText("0303", 4); // version 3.03
	error = error || !WriteText("txt ", 4); // text encoding
	error = error || !WriteText("0032", 4); // 32 bit floats
	error = error || !WriteText("\r\n", 2); // newline
	needs_indent_ = true;
	return !error;
}




//-------------------------------------------------------------------------------------------------------
bool_t TextWriter::WriteNameToken(
	const string_t& name
)
{
	if (name.length() == 0)
		{ return true; }

	std::basic_string<char> ascii_name = name;

	bool_t error = !WriteText(ascii_name.c_str(), static_cast<int_t>(ascii_name.length()));

	error = error || !WriteText(" ", 1);

	return !error;
}




//-------------------------------------------------------------------------------------------------------
bool_t TextWriter::WriteStandaloneToken(
	const tokens::Token::Type type
)
{
	bool_t error = false;

	switch (type)
	{
	case tokens::Token::TypeOBrace: {
		indent_ += UNICODE_STRING("  ");
		error = error || !WriteText("{", 1);
		break;
	}
	case tokens::Token::TypeCBrace: {
		if (indent_.length() >= 2)
			{ indent_ = indent_.substr(0, indent_.length()-2); }
		error = error || !WriteText("}", 1);
		break;
	}
	case tokens::Token::TypeComma: {
		error = error || !WriteText(",", 1);
		break;
	}
	case tokens::Token::TypeSemicolon: {
		error = error || !WriteText(";", 1);
		break;
	}
	}

	error = error || !WriteText(" ", 1);

	return !error;
}




//-------------------------------------------------------------------------------------------------------
bool_t TextWriter::WriteTokenDWord(
	const udword_t value
)
{
	return WriteTokenDWordList(&value, 1);
}




//-------------------------------------------------------------------------------------------------------
bool_t TextWriter::WriteTokenDWordList(
	const udword_t* list,
	const uint_t element_count
)
{
	std::stringstream ss;

	bool_t add_linebreaks = element_count > 16;

	for (uint_t i = 0; i < element_count; ++i)
	{
		ss << std::fixed << list[i];
		if (i < (element_count - 1))
		{
			ss << ",";
			if (add_linebreaks)
				{ ss << "\r\n" << indent_; }
		}
	}
	ss << ";";

	return WriteNameToken(ss.str());
}




//-------------------------------------------------------------------------------------------------------
bool_t TextWriter::WriteTokenFloat(
	const float_t value
)
{
	return WriteTokenFloatList(&value, 1);
}




//-------------------------------------------------------------------------------------------------------
bool_t TextWriter::WriteTokenFloatList(
	const float_t* list,
	const uint_t element_count
)
{
	std::stringstream ss;

	bool_t add_linebreaks = element_count > 16;

	for (uint_t i = 0; i < element_count; ++i)
	{
		ss << std::fixed << list[i];
		if (i < (element_count - 1))
		{
			ss << ",";
			if (add_linebreaks)
				{ ss << "\r\n" << indent_; }
		}
	}
	ss << ";";

	return WriteNameToken(ss.str());
}




//-------------------------------------------------------------------------------------------------------
bool_t TextWriter::WriteTokenString(
	const string_t& value
)
{
	bool_t error = !WriteText("\"", 1);

	if (value.length() > 0)
	{
		std::basic_string<char> ascii_value = value;
		error = error || !WriteText(ascii_value.c_str(), static_cast<int_t>(ascii_value.length()));
	}

	error = error || !WriteText("\"; ", 3);

	return !error;
}




//-------------------------------------------------------------------------------------------------------
bool_t TextWriter::WriteNewLineHint()
{
	bool_t success = WriteText("\r\n", 2);
	// don't set the flag before writing the newline, else it won't write the indent to the
	// new but to the old line!
	needs_indent_ = true;
	return success; 
}





//-------------------------------------------------------------------------------------------------------
bool_t TextWriter::WriteText(
	const char* text,
	const int_t length
)
{
	assert(length >= 0);

	if (needs_indent_)
	{
		needs_indent_ = false;
		if (indent_.length() > 0)
		{
			int_t written_length = write_function_(
				reinterpret_cast<const x::ubyte_t*>(indent_.c_str()),
				static_cast<uint_t>(indent_.length()),
				io_token_
				);
			if (written_length != static_cast<int_t>(indent_.length()))
				{ return false; }
		}
	}

	int_t written_length = write_function_(
		reinterpret_cast<const x::ubyte_t*>(text),
		static_cast<uint_t>(length),
		io_token_
		);

	return written_length == length;
}

}} // namespace x::writer


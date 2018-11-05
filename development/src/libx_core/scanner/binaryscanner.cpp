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
/*!	\file scanner/binaryscanner.cpp
	\brief Implementation von x::scanner::BinaryScanner
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::scanner::BinaryScanner
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "./binaryscanner.h"

namespace x { namespace scanner {

using namespace x::tokens;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
BinaryScanner::BinaryScanner()
:	lookahead_token_(NULL),
	token_pool_(),
	buffer_(NULL)
{
	buffer_ = new ubyte_t[TemporalBufferSize];
	buffer_index_ = TemporalBufferSize;
	buffer_size_ = TemporalBufferSize;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
BinaryScanner::~BinaryScanner()
{
	while (token_pool_.size() > 0)
	{
		Token* token = token_pool_.back();
		token_pool_.pop_back();
		delete token;
	}

	delete buffer_;
}




//-------------------------------------------------------------------------------------------------------
bool_t BinaryScanner::Eof(
	TemporalInfo& info
)
{
	return (info.eof_function(info.io_token) && buffer_index_ >= buffer_size_);
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadToken(
	TemporalInfo& info
)
{
	Token* token = NULL;

	if (lookahead_token_ != NULL)
	{
		token = lookahead_token_;
		lookahead_token_ = NULL;
	}

	if (token == NULL)
	{
		token = ReadTokenFromFile(info);
		token_pool_.push_back(token);
	}

	bool_t accepted = false;
	while(!accepted && token != NULL)
	{
		if (token->GetType() != Token::TypeComma && token->GetType() != Token::TypeSemicolon)
		{
			accepted = true;
		}
		else
		{
			token = ReadTokenFromFile(info);
			if (token != NULL)
			{
				token_pool_.push_back(token);
			}
		}
	}

	info.last_token_index = 0;
	info.last_token = token;
	return token;
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadOptionalToken(
	TemporalInfo& info,
	const Token::Type type
)
{
	if (lookahead_token_ == NULL)
	{
		lookahead_token_ = ReadToken(info);
	}

	if (lookahead_token_->GetType() == type)
	{
		Token* token = lookahead_token_;
		lookahead_token_ = NULL;
		return token;
	}
	else
	{
		info.last_token = NULL;
		info.last_token_index = 0;
		return NULL;
	}
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadNameTokenFromFile(
	TemporalInfo& info
)
{
	char static_buffer[128];

	udword_t length;
	if (!ReadData(info, reinterpret_cast<ubyte_t*>(&length), sizeof(udword_t)))
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("error while reading data\n");
		return NULL;
	}

	char* buffer = (length > 127) ? new char[length + 1] : static_buffer;
	if (!ReadData(info, reinterpret_cast<ubyte_t*>(buffer), length))
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("error while reading data\n");
		return NULL;
	}
	buffer[length] = 0;

	string_t name = buffer;
	if (length > 127) { delete[] buffer; }

	return new TokenName(buffer);
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadStringTokenFromFile(
	TemporalInfo& info
)
{
	char static_buffer[128];

	udword_t length;
	if (!ReadData(info, reinterpret_cast<ubyte_t*>(&length), sizeof(udword_t)))
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("unable to read length field for next token.\n");
		return NULL;
	}

	char* buffer = (length > 127) ? new char[length + 1] : static_buffer;
	if (!ReadData(info, reinterpret_cast<ubyte_t*>(buffer), length))
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Unable to read content for next token.\n");
		return NULL;
	}
	buffer[length] = 0;

	string_t value = buffer;
	if (length > 127) { delete[] buffer; }

	uword_t terminator;
	if (!ReadData(info, reinterpret_cast<ubyte_t*>(&terminator), sizeof(uword_t)))
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Unable to read terminator for next token.\n");
		return NULL;
	}

	if (terminator != Token::TypeSemicolon &&
		terminator != Token::TypeComma)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Invalid token-terminator detected. Expected Token::TypeSemicolon or Token::TypeComma.\n");
		return NULL;
	}

	return new TokenString(value);
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadIntegerTokenFromFile(
	TemporalInfo& info
)
{
	udword_t value;
	if (!ReadData(info, reinterpret_cast<ubyte_t*>(&value), sizeof(udword_t)))
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Unable to read another integer.\n");
		return NULL;
	}

	TokenInteger* token = new TokenInteger();
	token->SetDWord(value);
	return token;
}




//-----------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadGuidTokenFromFile(
	TemporalInfo& info
)
{
	udword_t field1;
	uword_t field2;
	uword_t field3;
	ubyte_t field4[8];
	

	ubyte_t buffer[16];
	if (!ReadData(info, buffer, 16))
	{
		// TODO: ERROR CASE
	}

	memcpy(&field1, &buffer[0], sizeof(udword_t));
	memcpy(&field2, &buffer[4], sizeof(uword_t));
	memcpy(&field3, &buffer[6], sizeof(uword_t));
	memcpy(&field4, &buffer[8], 8);

	return new TokenGuid(Guid(
		field1,
		field2, field3, 
		field4[0], field4[1], field4[2], field4[3],
		field4[4], field4[5], field4[6], field4[7]
	));
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadIntegerListTokenFromFile(
	TemporalInfo& info
)
{
	udword_t count;
	if (!ReadData(info, reinterpret_cast<ubyte_t*>(&count), sizeof(udword_t)))
	{
		// TODO: ERROR CASE
	}
	if (count == 0)
	{
		count = 5;
	}
	udword_t* array = new udword_t[count];
	if (!ReadData(info, reinterpret_cast<ubyte_t*>(array), sizeof(udword_t)*count))
	{
		// TODO: ERROR CASE
	}
	Token* token = new TokenIntegerList(array, count);
	delete[] array;
	return token;
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadFloatListTokenFromFile(
	TemporalInfo& info
)
{
	udword_t count;
	if (!ReadData(info, reinterpret_cast<ubyte_t*>(&count), sizeof(udword_t)))
	{
		// TODO: ERROR CASE
	}

	if (info.header.UseFloat32())
	{
		float_t* array = new float_t[count];
		if (!ReadData(info, reinterpret_cast<ubyte_t*>(array), sizeof(float_t)*count))
		{
			// TODO: ERROR CASE
		}
		Token* token = new TokenFloatList(array, count);
		delete[] array;
		return token;
	}
	else
	{
		double_t* array = new double_t[count];
		if (!ReadData(info, reinterpret_cast<ubyte_t*>(array), sizeof(double_t)*count))
		{
			// TODO: ERROR CASE
		}
		Token* token =  new TokenFloatList(array, count);
		delete[] array;
		return token;
	}
}




//-----------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadTokenFromFile(
	TemporalInfo& info
)
{
	uword_t token_type;
	if (!ReadData(info, reinterpret_cast<ubyte_t*>(&token_type), sizeof(uword_t)))
	{
		return NULL;
	}

	Token* token;
	switch (token_type)
	{
	case Token::TypeName: token = ReadNameTokenFromFile(info); break;
	case Token::TypeString: token = ReadStringTokenFromFile(info); break;
	case Token::TypeInteger: token = ReadIntegerTokenFromFile(info); break;
	case Token::TypeGuid: token = ReadGuidTokenFromFile(info); break;
	case Token::TypeIntegerList: token = ReadIntegerListTokenFromFile(info); break;
	case Token::TypeFloatList: token = ReadFloatListTokenFromFile(info); break;
	default: token = new StandaloneToken(static_cast<Token::Type>(token_type)); break;
	}

	// info nachführen
	return token;
}




//-----------------------------------------------------------------------------------------------------------
bool_t BinaryScanner::ReadData(
	TemporalInfo& info,
	ubyte_t* const buffer,
	const uint_t count
	)
{
	uint_t current_index = 0;
	uint_t data_left = count;

	while (data_left > 0)
	{
		uint_t current_amount = buffer_size_ - buffer_index_;
		if (current_amount > data_left)
			{ current_amount = data_left; }
		if (current_amount > 0)
		{
			memcpy(&buffer[current_index], &buffer_[buffer_index_], current_amount);
			current_index += current_amount;
			data_left -= current_amount;
			buffer_index_ += current_amount;
		}

		if (current_amount == 0)
		{
			if (!FillBuffer(info))
			{
				return false;
			}

			if (buffer_size_ == 0)
			{
				return false;
			}
		}
	}

	return true;
}




//-----------------------------------------------------------------------------------------------------------
bool_t BinaryScanner::FillBuffer(
	TemporalInfo& info
)
{
	int_t bytes_read = info.read_function(buffer_, TemporalBufferSize, info.io_token);
	if (bytes_read < 0)
	{
		info.error_flag = true;
		info.error_message = UNICODE_STRING("Error while reading data from stream.\n");
		buffer_index_ = 0;
		buffer_size_ = 0;
		return false;
	}

	buffer_index_ = 0;
	buffer_size_ = (uint_t)bytes_read;
	return true;
}

}} // namespace x::scanner


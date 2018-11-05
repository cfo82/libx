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
/*!	\file scanner/textscanner.cpp
	\brief Implementation von x::scanner::TextScanner
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::scanner::TextScanner
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "./textscanner.h"

namespace x { namespace scanner {

using namespace x::tokens;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TextScanner::TextScanner()
:	lookahead_pool_(),
	token_pool_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
TextScanner::~TextScanner()
{
	while (token_pool_.size() > 0)
	{
		Token* token = token_pool_.back();
		token_pool_.pop_back();
		delete token;
	}
}




//-------------------------------------------------------------------------------------------------------
bool_t TextScanner::Eof(
	TemporalInfo& info
)
{
	SkipWhitespaces(info);
	return (
		info.eof_function(info.io_token) &&
		info.buffer_index >= info.buffer_size &&
		info.lookahead == 0
		);
}




//-------------------------------------------------------------------------------------------------------
Token* TextScanner::ReadToken(
	TemporalInfo& info
)
{
	if (info.lookahead == 0)
	{
		info.lookahead = ReadCharacter(info);
	}

	Token* token = NULL;
	bool_t accepted = false;
	while(!accepted)
	{
		if (token != NULL && token->GetType() != Token::TypeComma && token->GetType() != Token::TypeSemicolon)
		{
			accepted = true;
		}
		else
		{
			if (lookahead_pool_.size() > 0)
			{
				token = lookahead_pool_.front();
				lookahead_pool_.erase(lookahead_pool_.begin());
			}

			if (token == NULL)
			{
				ReadTokenFromFile(info);
				if (lookahead_pool_.size() > 0)
				{
					token = lookahead_pool_.front();
					lookahead_pool_.erase(lookahead_pool_.begin());
				}
				else
				{
					return NULL;
				}
			}
		}
	}

	info.last_token = token;
	info.last_token_index = 0;
	return token;
}




//-------------------------------------------------------------------------------------------------------
Token* TextScanner::ReadOptionalToken(
	TemporalInfo& info,
	const Token::Type type
)
{
	if (lookahead_pool_.size() == 0)
	{
		ReadTokenFromFile(info);
	}

	if (lookahead_pool_.size() > 0 &&
		lookahead_pool_[0]->GetType() == type)
	{
		return ReadToken(info);
	}
	else
	{
		info.last_token = NULL;
		info.last_token_index = 0;
		return NULL;
	}
}




//-------------------------------------------------------------------------------------------------------
/*!
	\brief read a single character
	\param info

	reads the next character from the input-file.
*/
char TextScanner::ReadCharacter(TemporalInfo& info)
{
	if (info.buffer_index >= TemporalInfo::TemporalBufferSize)
	{
		memset(info.buffer, 0, TemporalInfo::TemporalBufferSize);
		int_t num_bytes_read = info.read_function(reinterpret_cast<ubyte_t*>(info.buffer), TemporalInfo::TemporalBufferSize, info.io_token);
		info.buffer_index = 0;
		info.buffer_size = TemporalInfo::TemporalBufferSize;

		if (num_bytes_read == 0)
		{
			// TODO: ERROR case!

		}
		else if (num_bytes_read < TemporalInfo::TemporalBufferSize)
		{
			// inside the last buffer we need to append another whitespace-character to 
			// help the system read the lookahead symbol which may be a last closing 
			// '}'-character for example
			info.buffer[num_bytes_read] = ' ';
			info.buffer_size = num_bytes_read+1;
		}

		// if this function is reading the first character after the header then lookahead needs
		// to be initialised
		if (info.lookahead == 0)
		{
			info.lookahead = info.buffer[info.buffer_index];
			++info.buffer_index;
		}
	}

	// return current character and advance lookahead
	char retval = info.lookahead;
	info.lookahead = info.buffer[info.buffer_index];
	++info.buffer_index;

	// count lines
	if (retval == '\n')
	{
		++info.line;
	}

	return retval;
}




//-------------------------------------------------------------------------------------------------------
void TextScanner::SkipWhitespaces(
	TemporalInfo& info
)
{
	if (info.eof_function(info.io_token) &&
		info.buffer_index >= info.buffer_size &&
		info.lookahead == 0
		)
	{
		return;
	}

	while (
		!(
			info.eof_function(info.io_token) &&
			info.buffer_index >= info.buffer_size &&
			info.lookahead == 0
		) &&
		(
			info.lookahead == ',' ||
			info.lookahead == ';' ||
			info.lookahead == ' ' ||
			info.lookahead == '\t' ||
			info.lookahead == '\n' ||
			info.lookahead == '\r' ||
			info.lookahead == '/' ||
			info.lookahead == '#'
		)
	)
	{
		if (info.lookahead == '/' ||
			info.lookahead == '#')
		{
			EatComment(info);
		}
		else
		{
			ReadCharacter(info);
		}
	}
}




//-------------------------------------------------------------------------------------------------------
void TextScanner::EatComment(
	TemporalInfo& info
)
{
	while (!(info.eof_function(info.io_token) && info.buffer_index >= info.buffer_size && info.lookahead == 0) && info.lookahead != '\n')
	{
		ReadCharacter(info);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief read guid-field
	\param info
	\param field target string into which the field will be read
	\param length

	reads a field from the input.
*/
void TextScanner::ReadGuidField(
	TemporalInfo& info,
	char* field,
	const uint_t length
)
{
	for (uint_t i = 0; i < length; ++i)
	{
		if (info.lookahead == '0' || info.lookahead == '1' || info.lookahead == '2' ||
			info.lookahead == '3' || info.lookahead == '4' || info.lookahead == '5' ||
			info.lookahead == '6' || info.lookahead == '7' || info.lookahead == '8' ||
			info.lookahead == '9' || info.lookahead == 'a' || info.lookahead == 'b' ||
			info.lookahead == 'c' || info.lookahead == 'd' || info.lookahead == 'e' ||
			info.lookahead == 'f' || info.lookahead == 'A' || info.lookahead == 'B' ||
			info.lookahead == 'C' || info.lookahead == 'D' || info.lookahead == 'E' ||
			info.lookahead == 'F'
		)
		{
			field[i] = ReadCharacter(info);
		}
		else
		{
			info.error_flag = true;
			info.error_message += UNICODE_STRING("hexadecimal character expected!");
			return;
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief read guid-separator
	\param info

	reads a gui-separator und throws if it's not a '-'-character.
*/
void TextScanner::ReadGuidSeparator(
	TemporalInfo& info
)
{
	if (info.lookahead != '-')
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("expected '-'-character.\n");
	}
	else
	{
		ReadCharacter(info);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief convert hex-string to integer-value
	\param field
	\param length
	\return integer-value

	converts the hex-value from the string-representation to an integer-value.
*/
udword_t TextScanner::HexStringToInt(
	const char* field,
	const uint_t length
)
{
	udword_t val = 0;
	for (uint_t i = 0; i < length; ++i)
	{
		val *= 16;
		switch (field[i])
		{
		case '0': val += 0; break;
		case '1': val += 1; break;
		case '2': val += 2; break;
		case '3': val += 3; break;
		case '4': val += 4; break;
		case '5': val += 5; break;
		case '6': val += 6; break;
		case '7': val += 7; break;
		case '8': val += 8; break;
		case '9': val += 9; break;
		case 'a':
		case 'A': val += 10; break;
		case 'b':
		case 'B': val += 11; break;
		case 'c':
		case 'C': val += 12; break;
		case 'd':
		case 'D': val += 13; break;
		case 'e':
		case 'E': val += 14; break;
		case 'f':
		case 'F': val += 15; break;
		}
	}
	return val;
}




//-----------------------------------------------------------------------------------------------------------
void TextScanner::ReadGuidTokenFromFile(
	TemporalInfo& info
)
{
	uint_t line = info.line;

	// savely ignore the first '<' because we won't use it!
	if (info.lookahead != '<')
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Expected '<'-character.\n");
		return;
	}
	else
	{
		ReadCharacter(info);
	}

	// read forward until the closing '>' element
	char field1_string[8];
	char field2_string[4];
	char field3_string[4];
	char field4_string[8][2];
	// field 1
	if (!info.error_flag)
		{ ReadGuidField(info, field1_string, 8); }
	if (!info.error_flag)
		{ ReadGuidSeparator(info); }
	if (!info.error_flag)
		{ ReadGuidField(info, field2_string, 4); }
	if (!info.error_flag)
		{ ReadGuidSeparator(info); }
	if (!info.error_flag)
		{ ReadGuidField(info, field3_string, 4); }
	if (!info.error_flag)
		{ ReadGuidSeparator(info); }
	if (!info.error_flag)
		{ ReadGuidField(info, field4_string[0], 2); }
	if (!info.error_flag)
		{ ReadGuidField(info, field4_string[1], 2); }
	if (!info.error_flag)
		{ ReadGuidSeparator(info); }
	if (!info.error_flag)
		{ ReadGuidField(info, field4_string[2], 2); }
	if (!info.error_flag)
		{ ReadGuidField(info, field4_string[3], 2); }
	if (!info.error_flag)
		{ ReadGuidField(info, field4_string[4], 2); }
	if (!info.error_flag)
		{ ReadGuidField(info, field4_string[5], 2); }
	if (!info.error_flag)
		{ ReadGuidField(info, field4_string[6], 2); }
	if (!info.error_flag)
		{ ReadGuidField(info, field4_string[7], 2); }

	if (info.error_flag)
	{
		info.error_message += UNICODE_STRING("Invalid GUID format detected.\n");
		return;
	}

	// check for closing angle
	if (info.lookahead != '>')
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("'>'-character expected!");
	}
	else
	{
		ReadCharacter(info);
	}

	// unwandeln
	udword_t field1;
	uword_t field2, field3;
	ubyte_t field4[8];
	field1 = HexStringToInt(field1_string, 8);
	field2 = (uword_t)HexStringToInt(field2_string, 4);
	field3 = (uword_t)HexStringToInt(field3_string, 4);
	field4[0] = (ubyte_t)HexStringToInt(field4_string[0], 2);
	field4[1] = (ubyte_t)HexStringToInt(field4_string[1], 2);
	field4[2] = (ubyte_t)HexStringToInt(field4_string[2], 2);
	field4[3] = (ubyte_t)HexStringToInt(field4_string[3], 2);
	field4[4] = (ubyte_t)HexStringToInt(field4_string[4], 2);
	field4[5] = (ubyte_t)HexStringToInt(field4_string[5], 2);
	field4[6] = (ubyte_t)HexStringToInt(field4_string[6], 2);
	field4[7] = (ubyte_t)HexStringToInt(field4_string[7], 2);

	Token* token = new TokenGuid(Guid(
		field1, field2, field3,
		field4[0], field4[1], field4[2], field4[3],
		field4[4], field4[5], field4[6], field4[7]
	), line);
	lookahead_pool_.push_back(token);
	token_pool_.push_back(token);
}




//-----------------------------------------------------------------------------------------------------------
void TextScanner::ReadNumber(
	TemporalInfo& info,
	string_t& number_string,
	bool_t& with_sign,
	bool_t& integral
)
{
	if (info.lookahead == '-' ||
		info.lookahead == '+')
	{
		with_sign = true;
		number_string += ReadCharacter(info);
	}

	while (isdigit(info.lookahead))
		number_string += ReadCharacter(info);

	integral = true;
	if (info.lookahead == '.')
	{
		integral = false;
		number_string += ReadCharacter(info);
	}

	while (isdigit(info.lookahead))
		number_string += ReadCharacter(info);
}




//-----------------------------------------------------------------------------------------------------------
void TextScanner::ReadNumberTokenFromFile(
	TemporalInfo& info
)
{
	// read a block of numbers...

	// read first number
	bool_t stop = false;
	uint_t line = info.line;
	string_t number_string;
	bool_t with_sign;
	bool_t integral;
	ReadNumber(info, number_string, with_sign, integral);

	while (!stop)
	{
		vector<float_t> float_list;
		vector<udword_t> udword_list;
		bool_t current_block_integral = integral;
		while (!stop && current_block_integral == integral)
		{
			if (integral)
			{
				udword_t value = 0;
				sscanf(number_string.c_str(), "%d", &value);
				udword_list.push_back(value);
			}
			else
			{
				float_t value = 0;
				sscanf(number_string.c_str(), "%f", &value);
				float_list.push_back(value);
			}

			// skip all whitespaces
			SkipWhitespaces(info);

			if (isdigit(info.lookahead) || info.lookahead == '-' || info.lookahead == '+')
			{
				line = info.line;
				number_string = "";
				ReadNumber(info, number_string, with_sign, integral);
			}
			else
			{
				stop = true;
			}
		}

		Token* token = NULL;
		if (current_block_integral)
		{
			if (udword_list.size() == 1)
			{
				TokenInteger* int_token = new TokenInteger(line);
				int_token->SetDWord(udword_list[0]);
				token = int_token;
			}
			else
			{
				token = new TokenIntegerList(&udword_list[0], (uint_t)udword_list.size(), line);
			}
		}
		else
		{
			token = new TokenFloatList(&float_list[0], (uint_t)float_list.size(), line);
		}
		lookahead_pool_.push_back(token);
		token_pool_.push_back(token);
	}
}




//-----------------------------------------------------------------------------------------------------------
void TextScanner::ReadStringTokenFromFile(
	TemporalInfo& info
)
{
	uint_t line = info.line;

	if (info.lookahead != '\"') return;
	ReadCharacter(info);

	string_t value;
	while (info.lookahead != '\"')
		value += ReadCharacter(info);

	// skip '"'-character
	ReadCharacter(info);

	Token* token = new TokenString(value, line);
	lookahead_pool_.push_back(token);
	token_pool_.push_back(token);
}




//-----------------------------------------------------------------------------------------------------------
void TextScanner::ReadTokenFromFile(
	TemporalInfo& info
)
{
	/*
	 *	test for skip
	 */
	SkipWhitespaces(info);
	if (Eof(info)) return;


	// if character is alphanumeric it's either a reserved keyword or a name-token.
	if (isalpha(info.lookahead))
	{
		// buffer to hold longest reserved keyword
		char buffer[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
		char lower_buffer[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		uint_t buffer_length = 0;
		for (uint_t i = 0; i < 8; ++i)
		{
			if (isalpha(info.lookahead))
			{
				buffer[i] = ReadCharacter(info);
				lower_buffer[i] = static_cast<char>(tolower(buffer[i]));
				buffer_length = i+1;
			}
		}

		// ARRAY
		if (buffer_length == 5 &&
			lower_buffer[0] == 'a' &&
			lower_buffer[1] == 'r' &&
			lower_buffer[2] == 'r' &&
			lower_buffer[3] == 'a' &&
			lower_buffer[4] == 'y')
		{
			Token* token = new StandaloneToken(Token::TypeArray, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// CHAR
		else if (buffer_length == 4 &&
			lower_buffer[0] == 'c' &&
			lower_buffer[1] == 'h' &&
			lower_buffer[2] == 'a' &&
			lower_buffer[3] == 'r')
		{
			Token* token = new StandaloneToken(Token::TypeChar, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// CSTRING
		else if (buffer_length == 7 &&
			lower_buffer[0] == 'c' &&
			lower_buffer[1] == 's' &&
			lower_buffer[2] == 't' &&
			lower_buffer[3] == 'r' &&
			lower_buffer[4] == 'i' &&
			lower_buffer[5] == 'n' &&
			lower_buffer[6] == 'g')
		{
			Token* token = new StandaloneToken(Token::TypeCString, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// DOUBLE
		else if (buffer_length == 6 &&
			lower_buffer[0] == 'd' &&
			lower_buffer[1] == 'o' &&
			lower_buffer[2] == 'u' &&
			lower_buffer[3] == 'b' &&
			lower_buffer[4] == 'l' &&
			lower_buffer[5] == 'e')
		{
			Token* token = new StandaloneToken(Token::TypeDouble, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// DWORD
		else if (buffer_length == 5 &&
			lower_buffer[0] == 'd' &&
			lower_buffer[1] == 'w' &&
			lower_buffer[2] == 'o' &&
			lower_buffer[3] == 'r' &&
			lower_buffer[4] == 'd')
		{
			Token* token = new StandaloneToken(Token::TypeDWord, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// FLOAT
		else if (buffer_length == 5 &&
			lower_buffer[0] == 'f' &&
			lower_buffer[1] == 'l' &&
			lower_buffer[2] == 'o' &&
			lower_buffer[3] == 'a' &&
			lower_buffer[4] == 't')
		{
			Token* token = new StandaloneToken(Token::TypeFloat, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// SDWORD
		else if (buffer_length == 6 &&
			lower_buffer[0] == 's' &&
			lower_buffer[1] == 'd' &&
			lower_buffer[2] == 'w' &&
			lower_buffer[3] == 'o' &&
			lower_buffer[4] == 'r' &&
			lower_buffer[5] == 'd')
		{
			Token* token = new StandaloneToken(Token::TypeSDWord, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// STRING
		else if (buffer_length == 6 &&
			lower_buffer[0] == 's' &&
			lower_buffer[1] == 't' &&
			lower_buffer[2] == 'r' &&
			lower_buffer[3] == 'i' &&
			lower_buffer[4] == 'n' &&
			lower_buffer[5] == 'g')
		{
			Token* token = new StandaloneToken(Token::TypeLpStr, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// SWORD
		else if (buffer_length == 5 &&
			lower_buffer[0] == 's' &&
			lower_buffer[1] == 'w' &&
			lower_buffer[2] == 'o' &&
			lower_buffer[3] == 'r' &&
			lower_buffer[4] == 'd')
		{
			Token* token = new StandaloneToken(Token::TypeSWord, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// TEMPLATE
		else if (buffer_length == 8 &&
			lower_buffer[0] == 't' &&
			lower_buffer[1] == 'e' &&
			lower_buffer[2] == 'm' &&
			lower_buffer[3] == 'p' &&
			lower_buffer[4] == 'l' &&
			lower_buffer[5] == 'a' &&
			lower_buffer[6] == 't' &&
			lower_buffer[7] == 'e')
		{
			Token* token = new StandaloneToken(Token::TypeTemplate, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// UCHAR
		else if (buffer_length == 5 &&
			lower_buffer[0] == 'u' &&
			lower_buffer[1] == 'c' &&
			lower_buffer[2] == 'h' &&
			lower_buffer[3] == 'a' &&
			lower_buffer[4] == 'r')
		{
			Token* token = new StandaloneToken(Token::TypeUChar, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// UNICODE
		else if (buffer_length == 7 &&
			lower_buffer[0] == 'u' &&
			lower_buffer[1] == 'n' &&
			lower_buffer[2] == 'i' &&
			lower_buffer[3] == 'c' &&
			lower_buffer[4] == 'o' &&
			lower_buffer[5] == 'd' &&
			lower_buffer[6] == 'e')
		{
			Token* token = new StandaloneToken(Token::TypeUnicode, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// VOID
		else if (buffer_length == 4 &&
			lower_buffer[0] == 'v' &&
			lower_buffer[1] == 'o' &&
			lower_buffer[2] == 'i' &&
			lower_buffer[3] == 'd')
		{
			Token* token = new StandaloneToken(Token::TypeVoid, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// WORD
		else if (buffer_length == 4 &&
			lower_buffer[0] == 'w' &&
			lower_buffer[1] == 'o' &&
			lower_buffer[2] == 'r' &&
			lower_buffer[3] == 'd')
		{
			Token* token = new StandaloneToken(Token::TypeWord, info.line);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
		// name-token
		else
		{
			string_t name = buffer;
			while (isalpha(info.lookahead) || isdigit(info.lookahead) || info.lookahead == '_')
			{
				name += ReadCharacter(info);
			}
			Token* token =  new TokenName(name);
			lookahead_pool_.push_back(token);
			token_pool_.push_back(token);
			return;
		}
	}
	// one-character-tokens
	else if (info.lookahead == '{')
	{
		ReadCharacter(info);
		Token* token = new StandaloneToken(Token::TypeOBrace, info.line);
		lookahead_pool_.push_back(token);
		token_pool_.push_back(token);
		return;
	}
	else if (info.lookahead == '}')
	{
		ReadCharacter(info);
		Token* token = new StandaloneToken(Token::TypeCBrace, info.line);
		lookahead_pool_.push_back(token);
		token_pool_.push_back(token);
		return;
	}
	else if (info.lookahead == '(')
	{
		ReadCharacter(info);
		Token* token = new StandaloneToken(Token::TypeOParen, info.line);
		lookahead_pool_.push_back(token);
		token_pool_.push_back(token);
		return;
	}
	else if (info.lookahead == ')') 
	{
		ReadCharacter(info);
		Token* token = new StandaloneToken(Token::TypeCParen, info.line);
		lookahead_pool_.push_back(token);
		token_pool_.push_back(token);
		return;
	}
	else if (info.lookahead == '[')
	{
		ReadCharacter(info);
		Token* token = new StandaloneToken(Token::TypeOBracket, info.line);
		lookahead_pool_.push_back(token);
		token_pool_.push_back(token);
		return;
	}
	else if (info.lookahead == ']')
	{
		ReadCharacter(info);
		Token* token = new StandaloneToken(Token::TypeCBracket, info.line);
		lookahead_pool_.push_back(token);
		token_pool_.push_back(token);
		return;
	}
	//else if (info.lookahead == '<') -> won't work together with Token::TypeGuid...
	//{
	//	ReadCharacter(info);
	//	Token* token = new StandaloneToken(Token::TypeOAngle, info.line);
	//	lookahead_pool_.push_back(token);
	//	token_pool_.push_back(token);
	//	return;
	//}
	//else if (info.lookahead == '>')
	//{
	//	ReadCharacter(info);
	//	Token* token = new StandaloneToken(Token::TypeCAngle, info.line);
	//	lookahead_pool_.push_back(token);
	//	token_pool_.push_back(token);
	//	return;
	//}
	else if (info.lookahead == '.')
	{
		ReadCharacter(info);
		Token* token = new StandaloneToken(Token::TypeDot, info.line);
		lookahead_pool_.push_back(token);
		token_pool_.push_back(token);
		return;
	}
	/*else if (info.lookahead == ',')
	{
		ReadCharacter(info);
		Token* token = new StandaloneToken(Token::TypeComma, info.line);
		lookahead_pool_.push_back(token);
		token_pool_.push_back(token);
		return;
	}
	else if (info.lookahead == ';')
	{
		ReadCharacter(info);
		Token* token = new StandaloneToken(Token::TypeSemicolon, info.line);
		lookahead_pool_.push_back(token);
		token_pool_.push_back(token);
		return;
	}*/
	// go on with other lexemes
	// '<' this must be a GUID
	if (info.lookahead == '<')
	{
		ReadGuidTokenFromFile(info);
		return;
	}
	// if this is a digit it's either a float value or an integer value
	if (isdigit(info.lookahead) || info.lookahead == '-')
	{
		ReadNumberTokenFromFile(info);
		return;
	}
	// if it's a '"'-character read a string-value
	if (info.lookahead == '\"') 
	{
		ReadStringTokenFromFile(info);
		return;
	}

	/*
	 *	Falls wir an dieser Stelle ankommen wurde ein ungültiger Buchstabe 
	 *	gefunden!
	 */
	info.error_flag = true;
	info.error_message += UNICODE_STRING("invalid character found");
}

}} // namespace x::scanner


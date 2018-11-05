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
/*!	\file header.cpp
	\brief Implementation von x::Header
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::Header
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "./header.h"

namespace x {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Header::Header()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Header::~Header()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Header lesen

	Liest die Informationen aus dem übergebenen Stream
*/
const bool_t Header::Read(
	io_read_function_t read_function,
	void* io_token
)
{
	int_t bytes_read = 0;
	bytes_read = read_function(magic_, 4, io_token);
	if (bytes_read < 0 || bytes_read != 4)
	{
		return false;
	}
	bytes_read = read_function(version_, 4, io_token);
	if (bytes_read < 0 || bytes_read != 4)
	{
		return false;
	}
	bytes_read = read_function(filetype_, 4, io_token);
	if (bytes_read < 0 || bytes_read != 4)
	{
		return false;
	}
	bytes_read = read_function(floattype_, 4, io_token);
	if (bytes_read < 0 || bytes_read != 4)
	{
		return false;
	}

	return true;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Magic-Value überprüfen
	
	Prüft ob das gelesene Magic-Value zum X-File-Format passt.
*/
const bool_t Header::CheckMagic() const
{
	return (
		magic_[0] == 'x' &&
		magic_[1] == 'o' &&
		magic_[2] == 'f' &&
		magic_[3] == ' '
	);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Binary-File?
	\return Flag

	Gibt ein Flag zurück welches anzeigt ob es sich bei diesem File um ein
	Binary X-File handelt.
*/
const bool_t Header::IsBinary() const
{
	return (
		filetype_[0] == 'b' &&
		filetype_[1] == 'i' &&
		filetype_[2] == 'n' && 
		filetype_[3] == ' '
	);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Text-File?
	\return Flag

	Gibt ein Flag zurück welches anzeigt ob es sich bei diesem File um ein Text X-File
	handelt.
*/
const bool_t Header::IsText() const
{
	return (
		filetype_[0] == 't' &&
		filetype_[1] == 'x' &&
		filetype_[2] == 't' &&
		filetype_[3] == ' '
	);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Komprimiertes File?
	\return Flag

	Gibt ein Flag zurück welches anzeigt ob es sich bei diesem File um ein 
	komprimiertes X-File handelt.
*/
const bool_t Header::IsBinaryCompressed() const
{
	return (
		filetype_[0] == 'b' &&
		filetype_[1] == 'z' &&
		filetype_[2] == 'i' &&
		filetype_[3] == 'p'
	);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Komprimiertes File?
	\return Flag

	Gibt ein Flag zurück welches anzeigt ob es sich bei diesem File um ein 
	komprimiertes X-File handelt.
*/
const bool_t Header::IsTextCompressed() const
{
	return (
		filetype_[0] == 't' &&
		filetype_[1] == 'z' &&
		filetype_[2] == 'i' &&
		filetype_[3] == 'p'
	);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief 32-Bit Float-Zahlen?
	\return Flag

	Gibt ein Flag zurück welches anzeigt ob die Float-Zahlen in diesem File
	32-Bit gross sind
*/
const bool_t Header::UseFloat32() const
{
	return (
		floattype_[0] == '0' &&
		floattype_[1] == '0' &&
		floattype_[2] == '3' &&
		floattype_[3] == '2'
	);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief 64-Bit Float-Zahlen?
	\return Flag

	Gibt ein Flag zurück welches anzeigt ob die Float-Zahlen in diesem File 
	64-Bit gross sind
*/
const bool_t Header::UseFloat64() const
{
	return (
		floattype_[0] == '0' &&
		floattype_[1] == '0' &&
		floattype_[2] == '6' &&
		floattype_[3] == '4'
	);
}

} // namespace x

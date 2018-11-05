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
/*!	\file temporalinfo.cpp
	\brief Implementation von x::TemporalInfo
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::TemporalInfo
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "./temporalinfo.h"
#include "./serializer.h"

namespace x {

using namespace x::tokens;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TemporalInfo::TemporalInfo(
	instances_container_t& in_instances,
	io_tell_function_t in_tell_function,
	io_eof_function_t in_eof_function,
	io_read_function_t in_read_function,
	void* in_io_token
)
:	header(),
	scanner(NULL),
	mode(FileModeText),
	buffer(NULL),
	lookahead(0),
	buffer_index(TemporalBufferSize),
	buffer_size(0),
	line(1),
	templates(),
	instances(in_instances),
	named_instances(),
	tell_function(in_tell_function),
	eof_function(in_eof_function),
	read_function(in_read_function),
	io_token(in_io_token),
	last_token(NULL),
	last_token_index(0),
	error_flag(false),
	error_message()
{
	buffer = new char[TemporalBufferSize];
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
TemporalInfo::~TemporalInfo()
{
	delete scanner;
	delete[] buffer;

	/*typedef templates_map_t::iterator iterator;
	for (iterator begin = named_instances.begin(); begin != named_instances.end(); ++begin)
	{
		begin->second->RemRef();
	}*/
	named_instances.clear();
}

} // x


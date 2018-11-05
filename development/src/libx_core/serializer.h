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
/*!	\file serializer.h
	\brief Definition von x::Serializer
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::Serializer
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "./global/global.h"
#include "./tokens/tokens.h"
#include "./templates/templates.h"
#include "./header.h"

namespace x {

enum ExportMode
{
	ExportText,
	ExportBinary,
	ExportCompressedBinary
};

void import_file(
	vector<templates::Template*>& instances,
	io_tell_function_t tell_function,
	io_eof_function_t eof_function,
	io_read_function_t read_function,
	void* io_token,
	bool_t* error_flag,
	string_t* error_message
	);

void export_file(
	const vector<templates::Template*>& instances,
	io_write_function_t write_function,
	void* io_token,
	const ExportMode export_mode,
	bool_t* error_flag,
	string_t* error_message
);



void read_file(TemporalInfo& info);
templates::Template* instanciate_template_by_guid(TemporalInfo& info, const Guid& guid);
templates::Template* instanciate_template_by_name(TemporalInfo& info, const string_t& name);
templates::Template* read_template_definition(TemporalInfo& info);
templates::Template* read_template_data(tokens::Token* token, TemporalInfo& info);
void delete_temporal_info(TemporalInfo& info);
udword_t get_next_dword(TemporalInfo& info);
float_t get_next_float32(TemporalInfo& info);
void print_file_structure(TemporalInfo& info);

} // namespace x

#endif // SERIALIZER_H

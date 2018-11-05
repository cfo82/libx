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

#define XLIB_CORE
#include "./interface.h"

namespace x {

using namespace std;
using namespace x::templates;

uint_t FILE_io_tell_function(void* io_token)
{
	return ftell(reinterpret_cast<FILE*>(io_token));
}

bool_t FILE_io_eof_function(void* io_token)
{
	return feof(reinterpret_cast<FILE*>(io_token)) != 0;
}

int_t FILE_io_read_function(ubyte_t* const buffer, const uint_t size, void* io_token)
{
	return (int_t)fread(buffer, 1, size, reinterpret_cast<FILE*>(io_token));
}

int_t FILE_io_write_function(const ubyte_t* const buffer, const uint_t size, void* io_token)
{
	int_t num_written = (int_t)fwrite(buffer, 1, size, reinterpret_cast<FILE*>(io_token));
	fflush(reinterpret_cast<FILE*>(io_token));
	return num_written;
}

//-----------------------------------------------------------------------------------------------------------
bool_t Interface::Import(
	const string_t& path,
	instances_t& instances
)
{
	FILE* file_pointer = fopen(path.c_str(), "rb");
	if (file_pointer == NULL)
	{
		// TODO: ERROR CASE
		std::cerr << "Unable to open file '" << path << "'" << std::endl;
		return false;
	}

	// additional output data
	bool_t error_flag = false;
	string_t error_string;

	import_file(instances, FILE_io_tell_function, FILE_io_eof_function, FILE_io_read_function, file_pointer, &error_flag, &error_string);

	if (fclose(file_pointer) != 0)
	{
		error_flag = true;
		error_string += UNICODE_STRING("Error while trying to close file pointer.\n");
	}

	if (error_flag)
	{
		cerr << "Errors occured: " << error_string << endl;
	}
	
	return !error_flag;
}




//-----------------------------------------------------------------------------------------------------------
bool_t Interface::Export(
	const string_t& path,
	const instances_t& instances,
	const ExportMode export_mode
)
{
	FILE* file_pointer = fopen(path.c_str(), "wb");
	if (file_pointer == NULL)
	{
		// TODO: ERROR CASE
		std::cerr << "Unable to open file '" << path << "'" << std::endl;
		return false;
	}

	// additional output data
	bool_t error_flag = false;
	string_t error_string;

	export_file(instances, FILE_io_write_function, file_pointer, export_mode, &error_flag, &error_string);

	if (fclose(file_pointer) != 0)
	{
		error_flag = true;
		error_string += UNICODE_STRING("Error while trying to close file pointer.\n");
	}

	if (error_flag)
	{
		std::cerr << "Errors occured: " << error_string << endl;
	}

	return !error_flag;
}




//-----------------------------------------------------------------------------------------------------------
void Interface::ReleaseInstances(
	instances_t& instances
)
{
	for (
		instances_t::iterator begin = instances.begin();
		begin != instances.end();
		++begin
	)
	{
		Template* current_template = (*begin);
		current_template->RemRef();
	}

	instances.clear();
}

} // namespace x

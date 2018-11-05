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

#ifndef INTERFACE_H
#define INTERFACE_H

#include "./global/global.h"
#include "./tokens/tokens.h"
#include "./templates/templates.h"
#include "./header.h"
#include "./temporalinfo.h"
#include "./serializer.h"

namespace x {

class Interface
{
public:
	typedef vector<templates::Template*> instances_t;

	virtual bool_t Import(const string_t& path, instances_t& instances);

	virtual bool_t Export(const string_t& path, const instances_t& instances, const ExportMode export_mode);
	
	virtual void ReleaseInstances(instances_t& instances);

protected:
};

} // namespace x

#endif // INTERFACE_H

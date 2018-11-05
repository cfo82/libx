//-----------------------------------------------------------------------------------------------------------
/*
 *	LibX - A Library to export/import .x-models
 *
 *	This file is
 *		Copyright (C) 2008 Basil Fierz <basil@gmx.ch>
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

#ifndef SIMPLEGL_TEXTURE_H
#define SIMPLEGL_TEXTURE_H

#include <string>

#include "system.h"

namespace simplegl {

class Texture
{
private:
	Texture();

public:
	virtual ~Texture();

	static Texture* CreateTexture(std::string path);

	unsigned int GetID() const { return tex_id_; }

public:
	unsigned char*	imageData_;	// Image Data (Up To 32 Bits)
	unsigned int	bpp_;		// Image Color Depth In Bits Per Pixel
	unsigned int	width_;		// Image Width
	unsigned int	height_;	// Image Height
	unsigned int	tex_id_;	// Texture ID Used To Select A Texture
	unsigned int	type_;		// Image Type (GL_RGB, GL_RGBA)
};

}

#endif /* SIMPLEGL_TEXTURE_H */
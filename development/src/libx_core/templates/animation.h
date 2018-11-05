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
/*!	\file templates/animation.h
	\brief Defines x::templates::Animation
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Defines x::templates::Animation
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPLATES_ANIMATION_H
#define TEMPLATES_ANIMATION_H

#include "../global/global.h"
#include "../temporalinfo.h"
#include "./template.h"

namespace x { namespace templates {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Contains animations referencing a previous frame.
	\version 0.1
	\date 02.04.2004, Christian Oberholzer implementation

	The Animation template is defined as 

	\code
template Animation
{
    < 3D82AB4F-62DA-11cf-AB39-0020AF71E433 >
    [...]
}
 	\endcode

	The use of the template has been kept open with the possibility to 
	add any template into the Animation template. But in practice an
	Animation animtes a Frame that has been previously defined.
	Thus the Animation template usually contains at least a reference to 
	the animated frame, one or more AnimationKey templates
	describing the animation and optionally an AnimationOptions
	template defining the animation. This is functionality exactly what LibX
	targets to support.

	In addition the template is open for any extension by user/client code. 
*/
class Animation : public Template
{
public:
	typedef vector<Template*> children_container_t;

public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	Animation();
protected:
	virtual ~Animation();

public:
	//-------------------------------------------------------------------------------------------------------
	virtual void PrintFileStructure(const uint_t level) const;
	virtual Template* Clone() const;
	virtual void ReadDefinition(TemporalInfo& info);
	virtual void ReadData(tokens::TokenName* name_token, tokens::TokenGuid* guid_token, TemporalInfo& info, bool_t read_cbrace);

	//-------------------------------------------------------------------------------------------------------
	virtual const string_t& GetName() const;
	virtual const string_t& GetReference() const;
	virtual const children_container_t& GetChildren() const;

protected:
	string_t name_;
	string_t reference_;
	children_container_t children_;
};

}} // namespace x::templates

#endif // TEMPLATES_ANIMATION_H

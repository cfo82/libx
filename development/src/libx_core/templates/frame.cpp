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
/*!	\file templates/frame.cpp
	\brief Implementation von x::templates::Frame
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::Frame
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../global/global.h"
#include "../tokens/tokens.h"
#include "../serializer.h"
#include "./templates.h"

namespace x { namespace templates {

using namespace x::tokens;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Frame::Frame()
:	Template(TemplateTypeFrame),
	children_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Frame::~Frame()
{
	for (
		children_container_t::iterator begin = children_.begin();
		begin != children_.end();
		++begin
	)
	{
		Template* current_template = (*begin);
		current_template->RemRef();
	}
	children_.clear();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template geh�rt aus.
*/
void Frame::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "Frame" << endl;
	for (
		children_container_t::const_iterator begin = children_.begin();
		begin != children_.end();
		++begin
	)
	{
		(*begin)->PrintFileStructure(level + 1);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zur�ck.
*/
Template* Frame::Clone() const
{
	Frame* clone = new Frame;
	clone->name_ = name_;

	/*
	 *	Alle Children kopieren
	 */
	for (
		children_container_t::const_iterator begin = children_.begin();
		begin != children_.end();
		++begin
	)
	{
		clone->children_.push_back((*begin)->Clone());
	}

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem �ber info gegebenen Input.
*/
void Frame::ReadDefinition(
	TemporalInfo& info
) 
{
	// [...]
	{
		Token::Type decl[] = { Token::TypeOBracket, Token::TypeDot, Token::TypeDot, Token::TypeDot, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 5, UNICODE_STRING("[...]"));
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template-Daten lesen
	\param name_token
	\param guid_token
	\param info
	\param read_cbrace

	Liest Daten dieses Templates.
*/
void Frame::ReadData(
	TokenName* name_token,
	TokenGuid*,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	if (!read_cbrace)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("template 'Frame' cannot be included without reading the final CBRACKET-token!");
		return;
	}

	/*
	 *	Namen �bernehmen, falls der Name gegeben ist
	 */
	if (name_token != NULL)
		name_ = name_token->GetName();


	/*
	 *	[...] bedeutet "open-end", d.h. es kann eigentlich alles noch als Child-Element
	 *	dieses Templates kommen...
	 */
	bool_t do_continue = true;
	while (!info.error_flag && do_continue)
	{
		Token* next_token = info.scanner->ReadToken(info);

		switch (next_token->GetType())
		{
		case Token::TypeName:
			children_.push_back(read_template_data(next_token, info));
			break;

		case Token::TypeCBrace:
			do_continue = false;
			break;

		default:
			info.error_flag = true;
			info.error_message += UNICODE_STRING("Unexpected Token.\n");
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void Frame::WriteData(
	writer::Writer* writer,
	bool_t* error_flag,
	string_t* error_message
)
{
	if (*error_flag)
		{ return; }

	bool_t write_error = false;

	write_error = write_error || !writer->WriteNameToken("Frame");
	if (name_.length() > 0)
		{ write_error = write_error || !writer->WriteNameToken(name_); }
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeOBrace);
	write_error = write_error || !writer->WriteNewLineHint();

	for (uint_t i = 0; i < children_.size() && !write_error; ++i)
	{
		if (children_[i])
			{ children_[i]->WriteData(writer, error_flag, error_message); }

		write_error = write_error || (*error_flag);
	}

	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeCBrace);
	write_error = write_error || !writer->WriteNewLineHint();

	if (write_error)
	{
		*error_flag = true;
		*error_message += UNICODE_STRING("error while writing frame template.\n");
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Frame-Namen zur�ckgeben
	\return Name

	Gibt den Namen dieses Frames zur�ck.
*/
const string_t& Frame::GetName() const
{
	return name_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Vektor mit den Children zur�ckgeben
	\return Referenz auf den Vektor

	Gibt eine Referenz auf den Vektor mit allen diesem Frame untergeordneten Templates
	zur�ck.
*/
const Frame::children_container_t& Frame::GetChildren() const
{
	return children_;
}

}} // namespace x::templates


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
/*!	\file templates/material.cpp
	\brief Implementation von x::templates::Material
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::Material
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
Material::Material()
:	Template(TemplateTypeMaterial),
	face_color_(NULL),
	power_(0.0f),
	specular_color_(NULL),
	emissive_color_(NULL),
	children_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Material::~Material()
{
	SAFE_REMREF(face_color_);
	SAFE_REMREF(specular_color_);
	SAFE_REMREF(emissive_color_);

	for(
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

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void Material::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "Material" << endl;
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

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* Material::Clone() const
{
	Material* clone = new Material;

	/*
	 *	Zuerst werden alle Attribute sofern vorhanden kopiert
	 */
	if (face_color_ != NULL)
		clone->face_color_ = static_cast<ColorRGBA*>(face_color_->Clone());
	clone->power_ = power_;
	if (specular_color_ != NULL)
		clone->specular_color_ = static_cast<ColorRGB*>(specular_color_->Clone());
	if (emissive_color_ != NULL)
		clone->emissive_color_ = static_cast<ColorRGB*>(emissive_color_->Clone());

	/*
	 *	Danach alle Children
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

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void Material::ReadDefinition(
	TemporalInfo& info
)
{
	// ColorRGBA faceColor;
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeName, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("ColorRGBA faceColor;"));
	}

	// FLOAT power;
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeFloat, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("FLOAT power;"));
	}
	
	// ColorRGB specularColor;
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeName, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("ColorRGB specularColor;"));
	}
	
	// ColorRGB emissiveColor;
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeName, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("ColorRGB emissiveColor;"));
	}
	
	// [...]
	if (!info.error_flag)
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
void Material::ReadData(
	TokenName* name_token,
	TokenGuid*,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	if (!read_cbrace)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("template 'Material' cannot be included without reading the final CBRACKET-token!");
		return;
	}

	/*
	 *	Namen übernehmen, falls der Name gegeben ist
	 */
	if (name_token != NULL)
		{ name_ = name_token->GetName(); }

	/*
	 *	Face-Color auslesen
	 */
	face_color_ = static_cast<ColorRGBA*>(instanciate_template_by_name(info, UNICODE_STRING("ColorRGBA")));
	face_color_->ReadData(NULL, NULL, info, false);

	/*
	 *	power auslesen
	 */
	power_ = get_next_float32(info);

	/*
	 *	Specular-Color und Emissive-Color auslesen
	 */
	specular_color_ = static_cast<ColorRGB*>(instanciate_template_by_name(info, UNICODE_STRING("ColorRGB")));
	specular_color_->ReadData(NULL, NULL, info, false);
	emissive_color_ = static_cast<ColorRGB*>(instanciate_template_by_name(info, UNICODE_STRING("ColorRGB")));
	emissive_color_->ReadData(NULL, NULL, info, false);

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
			info.error_message += UNICODE_STRING("Unexpected Token");
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void Material::WriteData(
	writer::Writer* writer,
	bool_t* error_flag,
	string_t* error_message
)
{
	if (*error_flag)
		{ return; }

	bool_t write_error = false;

	write_error = write_error || !writer->WriteNameToken("Material");
	if (name_.length() > 0)
		{ write_error = write_error || !writer->WriteNameToken(name_); }
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeOBrace);
	write_error = write_error || !writer->WriteNewLineHint();

	// ColorRGBA inline
	const math::Colorf& color = face_color_->GetColor();
	write_error = write_error || !writer->WriteTokenFloat(color.r);
	write_error = write_error || !writer->WriteTokenFloat(color.g);
	write_error = write_error || !writer->WriteTokenFloat(color.b);
	write_error = write_error || !writer->WriteTokenFloat(color.a);
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeSemicolon); // close template
	write_error = write_error || !writer->WriteNewLineHint();

	// write power
	write_error = write_error || !writer->WriteTokenFloat(power_);
	write_error = write_error || !writer->WriteNewLineHint();

	// ColorRGB inline (specular)
	const math::Colorf& specular_color = specular_color_->GetColor();
	write_error = write_error || !writer->WriteTokenFloat(specular_color.r);
	write_error = write_error || !writer->WriteTokenFloat(specular_color.g);
	write_error = write_error || !writer->WriteTokenFloat(specular_color.b);
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeSemicolon); // close template
	write_error = write_error || !writer->WriteNewLineHint();

	// ColorRGB inline (emissive)
	const math::Colorf& emissive_color = emissive_color_->GetColor();
	write_error = write_error || !writer->WriteTokenFloat(emissive_color.r);
	write_error = write_error || !writer->WriteTokenFloat(emissive_color.g);
	write_error = write_error || !writer->WriteTokenFloat(emissive_color.b);
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeSemicolon); // close template
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
	\brief Farbe des Materials zurückgeben
	\return Farbe

	Gibt die Farbe des Materials unter AMbient und Diffuse-Licht zurück.
*/
const ColorRGBA* Material::GetFaceColor() const
{
	return face_color_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Reflektionsstärke zurückgeben
	\return Stärke

	Gibt die Reflektionsstärke des Materials unter Specular-Licht zurück.
*/
const float_t Material::GetPower() const
{
	return power_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Farbe der Specular-Highlights zurückgeben
	\return farbe

	Gibt die Farbe des Materials unter Specular-Licht zurück.
*/
const ColorRGB* Material::GetSpecularColor() const
{
	return specular_color_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Farbe des ausgestrahlten Lichts
	\return Farbe

	Gibt die Farbe des Lichts zurück welches dieses Material selbst ausstrahlt.
*/
const ColorRGB* Material::GetEmissiveColor() const
{
	return emissive_color_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Untergeordnete Templates zurückgeben
	\return Referenz auf den Vektor in welchem die Templates gespeichert sind

	Gibt eine Referenz auf den Vektor zurück in welchem die diesem Material untergeordneten Templates
	gespeichert sind. Dies könnten z.B. Texturen, etc. sein.
*/
const Material::children_container_t& Material::GetChildren() const
{
	return children_;
}

}} // namespace x::templates;


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
/*!	\file templates/frametransformmatrix.cpp
	\brief Implementation von x::templates::FrameTransformMatrix
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::FrameTransformMatrix
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
FrameTransformMatrix::FrameTransformMatrix()
:	Template(TemplateTypeFrameTransformMatrix),
	matrix_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
FrameTransformMatrix::~FrameTransformMatrix()
{
	if (matrix_)
	{
		matrix_->RemRef();
		matrix_ = NULL;
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void FrameTransformMatrix::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "FrameTransformMatrix" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* FrameTransformMatrix::Clone() const
{
	FrameTransformMatrix* clone = new FrameTransformMatrix;

	/*
	 *	Das Attribut kopieren, wenn vorhanden
	 */
	if (matrix_ != NULL)
		clone->matrix_ = static_cast<Matrix4x4*>(matrix_->Clone());

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void FrameTransformMatrix::ReadDefinition(
	TemporalInfo& info
) 
{
	// Matrix4x4 frameMatrix;
	if (!info.error_flag)
	{
		Token::Type decl[] = { Token::TypeName, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("Matrix4x4 frameMatrix;"));
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
void FrameTransformMatrix::ReadData(
	TokenName*,
	TokenGuid*,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	/*
	 *	Face-Color auslesen
	 */
	matrix_ = static_cast<Matrix4x4*>(instanciate_template_by_name(info, UNICODE_STRING("Matrix4x4")));
	matrix_->ReadData(NULL, NULL, info, false);

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
		if (info.error_flag)
		{
			// TODO: ERROR CASE
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void FrameTransformMatrix::WriteData(
	writer::Writer* writer,
	bool_t* error_flag,
	string_t* error_message
)
{
	if (*error_flag)
		{ return; }

	// write the embedded Matrix4x4 template directly (without creating a template frame)

	const math::Matrix4x4f& matrix = matrix_->GetMatrix();
	float data_array[16];
	uint_t index = 0;
	for (uint_t i = 0; i < 4; ++i)
	{
		for (uint_t j = 0; j < 4; ++j)
		{
			data_array[index] = matrix.data.array2d[i][j];
			++index;
		}
	}

	bool_t write_error = false;

	write_error = write_error || !writer->WriteNameToken("FrameTransformMatrix");
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeOBrace);
	write_error = write_error || !writer->WriteNewLineHint();
	
	// write array
	write_error = write_error || !writer->WriteTokenFloatList(data_array, 16);
	// add semicolon as Matrix4x4 template delimiter
	write_error = write_error || !writer->WriteStandaloneToken(Token::TypeSemicolon);
	write_error = write_error || !writer->WriteNewLineHint();

	// close this template
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
	\brief Pointer auf die Transformationsmatrix zurückgeben
	\return Pointer

	Gibt einen Pointer auf das Matrix4x4-Template zurück welches diese Transformation
	speichert.
*/
const Matrix4x4* FrameTransformMatrix::GetMatrix() const
{
	return matrix_;
}

}} // namespace x::templates


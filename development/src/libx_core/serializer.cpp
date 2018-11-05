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
/*!	\file serializer.cpp
	\brief Implementation von x::Serializer
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::Serializer
*/
//-----------------------------------------------------------------------------------------------------------
#include "serializer.h"
#include "scanner/textscanner.h"
#include "scanner/binaryscanner.h"
#include "scanner/compressedbinaryscanner.h"
#include "writer/textwriter.h"

#pragma warning(disable:4996)

namespace x {

using namespace x::scanner;
using namespace x::tokens;
using namespace x::templates;

//-------------------------------------------------------------------------------------------------------
/*!
	\brief Mesh importieren
	\param input
		Quelldaten
	\param instances

	Importiert den Mesh aus dem übergebenen Stream und speichert ihn in der übergebenen 
	Mesh-Struktur.
*/
void import_file(
	vector<templates::Template*>& instances,
	io_tell_function_t tell_function,
	io_eof_function_t eof_function,
	io_read_function_t read_function,
	void* io_token,
	bool_t* error_flag,
	string_t* error_message
)
{
	// reset error
	*error_flag = false;
	*error_message = "";

	TemporalInfo info(instances, tell_function, eof_function, read_function, io_token);

	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Animation"),					new Animation));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("AnimationKey"),				new AnimationKey));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("AnimationOptions"),			new AnimationOptions));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("AnimationSet"),				new AnimationSet));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("AnimTicksPerSecond"),			new AnimTicksPerSecond));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Boolean"),					new Boolean));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Boolean2d"),					new Boolean2d));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("ColorRGB"),					new ColorRGB));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("ColorRGBA"),					new ColorRGBA));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Coords2d"),					new Coords2d));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("DeclData"),					new DeclData));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectDWord"),				new EffectDWord));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectFloats"),				new EffectFloats));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectInstance"),				new EffectInstance));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectParamDWord"),			new EffectParamDWord));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectParamFloats"),			new EffectParamFloats));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectParamString"),			new EffectParamString));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectString"),				new EffectString));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("FloatKeys"),					new FloatKeys));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Frame"),						new Frame));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("FrameTransformMatrix"),		new FrameTransformMatrix));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("FVFData"),					new FVFData));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("IndexedColor"),				new IndexedColor));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Header"),						new HeaderTemplate));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Material"),					new Material));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MaterialWrap"),				new MaterialWrap));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Matrix4x4"),					new Matrix4x4));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Mesh"),						new Mesh));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MeshFace"),					new MeshFace));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MeshFaceWraps"),				new MeshFaceWraps));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MeshMaterialList"),			new MeshMaterialList));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MeshNormals"),				new MeshNormals));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MeshTextureCoords"),			new MeshTextureCoords));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MeshVertexColors"),			new MeshVertexColors));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("SkinWeights"),				new SkinWeights));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("TextureFilename"),			new TextureFilename));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("TimedFloatKeys"),				new TimedFloatKeys));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Vector"),						new Vector));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("VertexDuplicationIndices"),	new VertexDuplicationIndices));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("VertexElement"),				new VertexElement));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("XSkinMeshHeader"),			new XSkinMeshHeader));

	if (!info.header.Read(read_function, io_token))
	{
		// TODO: ERROR CHECK
	}
	if (!info.header.CheckMagic())
	{
		// TODO: ERROR CHECK
	}

	read_file(info);

	//print_file_structure(info);
	delete_temporal_info(info);

	if (info.error_flag)
	{
		*error_flag = info.error_flag;
		*error_message = info.error_message;
		
		switch (info.mode)
		{
		case TemporalInfo::FileModeBinary: {
			char position_string[512];
			sprintf(position_string, "%d", info.tell_function(info.io_token));
			*error_message += string_t(UNICODE_STRING("An error occured during parsing process at position ")) + position_string + UNICODE_STRING(".\n");
			break;
		}
		case TemporalInfo::FileModeText: {
			char line_string[512];
			sprintf(line_string, "%d", info.line);
			*error_message += string_t(UNICODE_STRING("An error occured during parsing process on line ")) + line_string + UNICODE_STRING(".\n");
		}
		}
	}
}




//-------------------------------------------------------------------------------------------------------
/*!
	\brief export template data to a new file
	\param instances instance data to be written
	\param write_function function which can be used to write data to a file (or a buffer or whatever...)
	\param io_token io_token which is passed to the write_function, user-data
	\param export_mode mode in which to write data
	\param error_flag output data: will be set if there is an error
	\param error_message output data: an error message is written to this string if an error occurs
*/
void export_file(
	const vector<templates::Template*>& instances,
	io_write_function_t write_function,
	void* io_token,
	const ExportMode export_mode,
	bool_t* error_flag,
	string_t* error_message
)
{
	writer::Writer* writer = NULL;
	switch (export_mode)
	{
	case ExportText: {
		writer = new writer::TextWriter(write_function, io_token);
		break;
	}
	case ExportBinary:
	case ExportCompressedBinary: 
	default: {
		writer = NULL;
	}
	}

	if (!writer)
	{
		*error_flag = true;
		*error_message += UNICODE_STRING("Error: Unsupported or invalid export mode selected.\n");
		return;
	}

	if (!writer->WriteHeader())
	{
		*error_flag = true;
		*error_message += UNICODE_STRING("Error: Failed to write header.\n");
	}

	for (uint_t i = 0; i < instances.size() && !(*error_flag); ++i)
	{
		if (instances[i] != NULL)
			{ instances[i]->WriteData(writer, error_flag, error_message); }
	}

	if (writer)
		{ delete writer; }
}




//-------------------------------------------------------------------------------------------------------
/*!
	\brief Datei auslesen
	\param info

	Alle Templates und alle Template-instanzen auslesen. Die gelesenen Informationen
	werden in 'info' hierarchisch zwischengespeichert.
*/
void read_file(
	TemporalInfo& info
)
{
	if (info.header.IsText())
	{
		info.mode = TemporalInfo::FileModeText;
		info.scanner = new TextScanner();
	}
	else if (info.header.IsBinary())
	{
		info.mode = TemporalInfo::FileModeBinary;
		info.scanner = new BinaryScanner();
	}
	else if (info.header.IsTextCompressed())
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("compressed-text-format-reader todo!\n");
		return;
	}
	else if (info.header.IsBinaryCompressed())
	{
		info.mode = TemporalInfo::FileModeCompressedBinary;
		info.scanner = new CompressedBinaryScanner();
	}

	while (!info.error_flag && !info.scanner->Eof(info))
	{
		Token* token = info.scanner->ReadToken(info);
		if (!token)
			{ break; }

		switch (token->GetType())
		{
		case Token::TypeTemplate: {
			Template* current_template = read_template_definition(info);
			if (!current_template)
			{
				info.error_flag = true;
				info.error_message += UNICODE_STRING("Error while reading template definition.\n");
				break;
			}

			string_t name = current_template->GetName();
			if (info.templates.find(name) == info.templates.end())
			{
				info.templates.insert(TemporalInfo::templates_value_t(current_template->GetName(), current_template));
			}
			else
			{
				current_template->RemRef();
			}
			break;
		}

		case Token::TypeName: {
			Template* instance = read_template_data(token, info);
			if (!instance)
			{
				info.error_flag = true;
				info.error_message += UNICODE_STRING("Error while reading template data.\n");
				break;
			}

			info.instances.push_back(instance);
			break;
		}
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
Template* instanciate_template_by_guid(
	TemporalInfo& info,
	const Guid& guid
)
{
	Template* current_template = NULL;
	if (guid == g_guids[TemplateTypeAnimation])
		{ current_template = new Animation; }
	else if (guid == g_guids[TemplateTypeAnimationKey])
		{ current_template = new AnimationKey; }
	else if (guid == g_guids[TemplateTypeAnimationOptions])
		{ current_template = new AnimationOptions; }
	else if (guid == g_guids[TemplateTypeAnimationSet])
		{ current_template = new AnimationSet; }
	else if (guid == g_guids[TemplateTypeAnimTicksPerSecond])
		{ current_template = new AnimTicksPerSecond; }
	else if (guid == g_guids[TemplateTypeBoolean])
		{ current_template = new Boolean; }
	else if (guid == g_guids[TemplateTypeBoolean2D])
		{ current_template = new Boolean2d; }
	else if (guid == g_guids[TemplateTypeColorRGB])
		{ current_template = new ColorRGB; }
	else if (guid == g_guids[TemplateTypeColorRGBA])
		{ current_template = new ColorRGBA; }
	else if (guid == g_guids[TemplateTypeCoords2D])
		{ current_template = new Coords2d; }
	else if (guid == g_guids[TemplateTypeDeclData])
		{ current_template = new DeclData; }
	else if (guid == g_guids[TemplateTypeEffectDWord])
		{ current_template = new EffectDWord; }
	else if (guid == g_guids[TemplateTypeEffectFloats])
		{ current_template = new EffectFloats; }
	else if (guid == g_guids[TemplateTypeEffectInstance])
		{ current_template = new EffectInstance; }
	else if (guid == g_guids[TemplateTypeEffectParamDWord])
		{ current_template = new EffectParamDWord; }
	else if (guid == g_guids[TemplateTypeEffectParamFloats])
		{ current_template = new EffectParamFloats; }
	else if (guid == g_guids[TemplateTypeEffectParamString])
		{ current_template = new EffectParamString; }
	else if (guid == g_guids[TemplateTypeEffectString])
		{ current_template = new EffectString; }
	else if (guid == g_guids[TemplateTypeFloatKeys])
		{ current_template = new FloatKeys; }
	else if (guid == g_guids[TemplateTypeFrame])
		{ current_template = new Frame; }
	else if (guid == g_guids[TemplateTypeFrameTransformMatrix])
		{ current_template = new FrameTransformMatrix; }
	else if (guid == g_guids[TemplateTypeFVFData])
		{ current_template = new FVFData; }
	else if (guid == g_guids[TemplateTypeHeader])
		{ current_template = new HeaderTemplate; }
	else if (guid == g_guids[TemplateTypeIndexedColor])
		{ current_template = new IndexedColor; }
	else if (guid == g_guids[TemplateTypeMaterial])
		{ current_template = new Material; }
	else if (guid == g_guids[TemplateTypeMaterialWrap])
		{ current_template = new MaterialWrap; }
	else if (guid == g_guids[TemplateTypeMatrix4x4])
		{ current_template = new Matrix4x4; }
	else if (guid == g_guids[TemplateTypeMesh])
		{ current_template = new Mesh; }
	else if (guid == g_guids[TemplateTypeMeshFace])
		{ current_template = new MeshFace; }
	else if (guid == g_guids[TemplateTypeMeshFaceWraps])
		{ current_template = new MeshFaceWraps; }
	else if (guid == g_guids[TemplateTypeMeshMaterialList])
		{ current_template = new MeshMaterialList; }
	else if (guid == g_guids[TemplateTypeMeshNormals])
		{ current_template = new MeshNormals; }
	else if (guid == g_guids[TemplateTypeMeshTextureCoords])
		{ current_template = new MeshTextureCoords; }
	else if (guid == g_guids[TemplateTypeMeshVertexColors])
		{ current_template = new MeshVertexColors; }
	else if (guid == g_guids[TemplateTypeSkinWeights])
		{ current_template = new SkinWeights; }
	else if (guid == g_guids[TemplateTypeTextureFilename])
		{ current_template = new TextureFilename; }
	else if (guid == g_guids[TemplateTypeTimedFloatKeys])
		{ current_template = new TimedFloatKeys; }
	else if (guid == g_guids[TemplateTypeVector])
		{ current_template = new Vector; }
	else if (guid == g_guids[TemplateTypeVertexDuplicationIndices])
		{ current_template = new VertexDuplicationIndices; }
	else if (guid == g_guids[TemplateTypeVertexElement])
		{ current_template = new VertexElement; }
	else if (guid == g_guids[TemplateTypeXSkinMeshHeader])
		{ current_template = new XSkinMeshHeader; }
	else
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Unable to create instace of undefined template with GUID 'TODO'.\n");
		return NULL;
	}

	return current_template;
}




//-----------------------------------------------------------------------------------------------------------
Template* instanciate_template_by_name(
	TemporalInfo& info,
	const string_t& name
)
{
	TemporalInfo::templates_map_t::iterator found = info.templates.find(name);
	if (found == info.templates.end())
	{
		info.error_flag = true;
		info.error_message += string_t(UNICODE_STRING("Unable to create instance of undefined template '")) + name + UNICODE_STRING("'.\n");
		return NULL;
	}

	return found->second->Clone();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Eine Template-Definition lesen
	\param template_name
	\param info

	Liest ein Template aus dem File.
*/
Template* read_template_definition(
	TemporalInfo& info
)
{
	TokenName* name_token = static_cast<TokenName*>(info.scanner->ReadExpectedToken(info, Token::TypeName));
	if (!name_token)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Expected name token representing the template name.\n");
		return NULL;
	}

	Token* token_obrace = info.scanner->ReadExpectedToken(info, Token::TypeOBrace);
	if (!token_obrace)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Expected an opening brace afther the template name.\n");
		return NULL;
	}

	TokenGuid* token_guid = static_cast<TokenGuid*>(info.scanner->ReadExpectedToken(info, Token::TypeGuid));
	if (!token_guid)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Expected a GUID token identifying the template.\n");
		return NULL;
	}

	Template* current_template = instanciate_template_by_guid(info, token_guid->GetGuid());
	if (!current_template)
		{ current_template = instanciate_template_by_name(info, name_token->GetName()); }
	//if (!current_template)
	//	current_template = new GenericTemplate(name_token, token_guid);
	
	if (!current_template)
	{
		info.error_flag = true;
		info.error_message += string_t(UNICODE_STRING("Template not found. Identifier '")) + name_token->GetName() + UNICODE_STRING("'.\n");
		return NULL;
	}

	current_template->ReadDefinition(info);
	if (info.error_flag)
	{
		info.error_message += string_t(UNICODE_STRING("Error while reading definition of '")) + name_token->GetName() + UNICODE_STRING("' template.\n");
		return NULL;
	}

	info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	if (info.error_flag)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Expected closing brace after template definition.\n");
		return NULL;
	}

	return current_template;
}




//-------------------------------------------------------------------------------------------------------
/*!
	\brief Template-Daten auslesen
	\param token
	\param info

	Liest Template-Daten.
*/
Template* read_template_data(
	Token* token,
	TemporalInfo& info
)
{
	switch (token->GetType())
	{
	case Token::TypeOBrace: {
		TokenName* reference_name_token = static_cast<TokenName*>(info.scanner->ReadExpectedToken(info, Token::TypeName));
		if (!reference_name_token)
		{
			info.error_flag = true;
			info.error_message += UNICODE_STRING("Missing reference name.\n");
			return NULL;
		}

		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
		if (info.error_flag)
		{
			info.error_message += UNICODE_STRING("Missing closing brace of reference.\n");
			return NULL;
		}

		TemporalInfo::templates_map_t::iterator found = info.named_instances.find(reference_name_token->GetName());
		if (found != info.named_instances.end())
		{
			found->second->AddRef();
			return found->second;
		}
		else
		{
			info.error_flag = true;
			info.error_message += UNICODE_STRING("Unresolved named reference.\n");
			return NULL;
		}
	}

	case Token::TypeName: {
		TokenName* template_name_token = static_cast<TokenName*>(token);
		TokenName* optional_name_token = NULL;
		TokenGuid* optional_guid_token = NULL;

		// the name token is optional
		optional_name_token = static_cast<TokenName*>(info.scanner->ReadOptionalToken(info, Token::TypeName));

		info.scanner->ReadExpectedToken(info, Token::TypeOBrace);
		if (info.error_flag)
		{
			info.error_message += UNICODE_STRING("Missing opening brace of template instance.\n");
			return NULL;
		}

		// the guid token is optional again
		optional_guid_token = static_cast<TokenGuid*>(info.scanner->ReadOptionalToken(info, Token::TypeGuid));

		// collect data about the template
		string_t template_name_string = "not-named";
		if (optional_name_token != NULL)
			{ template_name_string = optional_name_token->GetName(); }

		// instanciate a new template instance and read its data
		Template* current_template = instanciate_template_by_name(info, template_name_token->GetName());
		current_template->ReadData(optional_name_token, optional_guid_token, info, true);
		if (info.error_flag)
		{
			info.error_message += string_t(UNICODE_STRING("Error while reading data of template '")) + template_name_string + UNICODE_STRING("'.\n");
			return NULL;
		}

		if (optional_name_token != NULL)
			{ info.named_instances.insert(pair<string_t, Template*>(optional_name_token->GetName(), current_template)); }

		return current_template;
	}

	default: {
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Unexpected token.\n");
		return NULL;
	}

	}
}




//-------------------------------------------------------------------------------------------------------
/*!
	\brief delete all contents from a temporal-info structure
*/
void delete_temporal_info(
	TemporalInfo& info
)
{
	for (
		TemporalInfo::templates_map_t::iterator begin = info.templates.begin();
		begin != info.templates.end();
		++begin
	)
	{
		Template* current_template = begin->second;
		current_template->RemRef();
	}
	info.templates.clear();
}




//-------------------------------------------------------------------------------------------------------
/*!
	\brief Das nächste DWORD zurückgeben
	\return DWORD

	Gibt das nächste im input zu findende DWORD zurück.
*/
udword_t get_next_dword(
	TemporalInfo& info
)
{
	if (info.last_token != NULL && info.last_token->GetType() == Token::TypeIntegerList)
	{
		TokenIntegerList* list = static_cast<TokenIntegerList*>(info.last_token);
		udword_t count = list->GetCount();

		// Im besten Fall hats noch einen Integer im letzten Token
		if (info.last_token_index < count)
		{
			const udword_t* const array = list->GetArray();
			udword_t retval = array[info.last_token_index];
			++info.last_token_index;
			if (info.last_token_index >= count)
			{
				info.last_token = NULL;
				info.last_token_index = 0;
			}
			return retval;
		}
	}

	/*
	 *	Wenn entweder das letzte Token keine Integer-Liste war, oder die Integer-Liste voll
	 *	ausgeschöpft worden ist müssen wir das nächste Token lesen und hoffen, dass sich darin ein 
	 *	Integer befindet. Wenn nicht wird eine Exception geworfen. Achtung: Das nächste Token könnte
	 *	entweder ein einzelner Integer oder eine Integer-Liste sein. Beide Fälle werden akzeptiert.
	 */
	info.scanner->ReadToken(info);
	switch (info.last_token->GetType())
	{
	case Token::TypeInteger:
		{
			TokenInteger* integer_token = static_cast<TokenInteger*>(info.last_token);
			return integer_token->GetDword();
		}
		break;

	case Token::TypeIntegerList:
		// In diesem Fall kann Rekursion zum Ziel führen, da ja jetzt sicher ein 
		// Element der geladenen Liste verfügbar ist!
		return get_next_dword(info);
		break;

	default:
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Token::TypeInteger or Token::TypeIntegerList expected!\n");
		return 0;
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Den nächsten Float zurückgeben
	\return Float

	Gibt den nächsten im input zu findende Float zurück.
*/
float_t get_next_float32(TemporalInfo& info)
{
	if (info.last_token != NULL && info.last_token->GetType() == Token::TypeFloatList)
	{
		TokenFloatList* list = static_cast<TokenFloatList*>(info.last_token);
		udword_t count = list->GetCount();

		// Im besten Fall hats noch einen Float im letzten Token
		float_t retval = 0.0f;
		if (info.last_token_index < count)
		{
			const float_t* const array = list->GetFloat32Array();
			if (array)
			{
				retval = array[info.last_token_index];
			}
			else
			{
				info.error_flag = true;
				info.error_message += UNICODE_STRING("array invalid ptr!\n");
				return 0.0f;
			}
		}

		++info.last_token_index;
		if (info.last_token_index >= count)
		{
			info.last_token = NULL;
			info.last_token_index = 0;
		}
		return retval;
	}

	/*
	 *	Wenn entweder das letzte Token keine Float-Liste war, oder die Float-Liste voll
	 *	ausgeschöpft worden ist müssen wir das nächste Token lesen und hoffen, dass sich darin ein 
	 *	Float befindet. Wenn nicht wird eine Exception geworfen. Achtung: Das nächste Token könnte
	 *	entweder ein einzelner Float oder eine Float-Liste sein. Beide Fälle werden akzeptiert.
	 */
	info.scanner->ReadToken(info);
	switch (info.last_token->GetType())
	{
	case Token::TypeFloatList:
		// In diesem Fall kann Rekursion zum Ziel führen, da ja jetzt sicher ein 
		// Element der geladenen Liste verfügbar ist!
		return get_next_float32(info);
		break;

	default:
		info.error_flag = true;
		info.error_message += UNICODE_STRING("Token::TypeFloatList expected!\n");
		return 0.0f;
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Die gesamte Dateistruktur ausgeben
	\param info

	Gibt die Struktur der in info gespeicherten Datei auf die Konsole aus.
*/
void print_file_structure(TemporalInfo& info)
{
	for (
		TemporalInfo::instances_container_t::iterator begin = info.instances.begin();
		begin != info.instances.end();
		++begin
	)
	{
		(*begin)->PrintFileStructure(0);
	}
}

} // namespace x

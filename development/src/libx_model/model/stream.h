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
/*!	\file model/stream.h
	\brief Definition von x::model::Stream
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::Stream
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_STREAM_H
#define MODEL_STREAM_H

#include "../../libx_core/global/global.h"
#include "../exception.h"
#include "./valuetype.h"
#include "./semanticinfo.h"
#include "./semantictypes.h"

namespace x { namespace model {

//-------------------------------------------------------------------------------------------------------
/*!
	\brief Stream von Daten
	\version 0.1
	\date 10.4.2004
	\author Christian Oberholzer

	Diese Klasse stellt das Interface zu einem Stream von geometrischen Daten dar. Ein Geometry-
	Objekt kann mehrere Streams enthalten die zum Beispiel Position, Normalvektoren etc. enthalten.
	Animationen k�nnen direkt die verschiedenen Streams animieren. Dazu greifen sie auf die Daten
	der Streams zu und ver�ndern sie. 

	Je nach Verwendungszweck des Modells, bzw. der Geometrie kann nun ein Stream verschieden implementiert
	werden:
		- Die erste und einfachste Implementierung ist jeden Stream als ein Array der durch ihn
		repr�sentierten Einzelelemente zu implementieren. 
		- Die zweite Variante w�re beispielsweise mehrere Streams als Adapter auf VertexBuffer zu verwenden.
		  �ber die LockData/UnlockData-Methoden k�nnten die Vertexstreams gesperrt und wieder freigegeben
		  werden. So k�nnten die Animationen direkt auf den Buffern durch die Klassen des Model-Namespace
		  und ohne weitere Zwischenstufen implementiert werden.
		.

	Denkbar sind auch sp�tere Erweiterungen so dass die Animation auf der Grafikhardware durch spezialiserte
	Shaderprogramme durchgef�hrt werden k�nnen. Diese Erweiterungen sollen jedoch sp�ter ins Design einfliessen
	sobald die Software-L�sung funktioniert.
*/
class Stream
{
public:
	Stream();
	virtual ~Stream();

	// Informationen �ber den Stream
	virtual const uint_t GetElementCount() const = 0;
	virtual const ValueType GetValueType() const = 0;
	virtual const SemanticTypes GetSemantics() const = 0;
	virtual const uint_t GetSemanticInfo() const = 0;

	// Bytedaten
	/// Daten zum Zugriff bereit machen
	virtual ubyte_t* LockData(uint_t& stride) = 0;
	/// Zugriff auf Daten. Nur G�ltig innerhalb eins LockData/UnlockData-Paares
	virtual ubyte_t* GetData(uint_t& stride) = 0;
	/// Daten fertig "bearbeitet". Wieder freigeben
	virtual void UnlockData() = 0;

	/// Dirty-Flag
	virtual const bool_t IsDirty() const = 0;
	virtual void ClearDirtyFlag() = 0;

	// duplication methode
	virtual Stream* Clone() const = 0;

	// Specific get-methodes
	template <
		typename TypeT,
		int ValueT
	>
	inline const TypeT& GenericGetMethode(const uint_t index)
	{
		if (GetValueType() != ValueT)
		{
			throw Exception("Invalid Type!");
		}

		uint_t stride = 0;
		ubyte_t* data = GetData(stride);
		data += stride * index;
		return *((TypeT*)data);
	}
	inline const math::Vector4<ubyte_t>& GetUByte4(const uint_t index)
	{
		return GenericGetMethode<math::Vector4<ubyte_t>, ValueUByte4>(index);
	}
	inline const math::Colorf& GetColor(const uint_t index)
	{
		return GenericGetMethode<math::Colorf, ValueColor>(index);
	}
	inline float_t GetFloat(const uint_t index)
	{
		return GenericGetMethode<float_t, ValueFloat>(index);
	}
	inline const math::Vector2f& GetVector2(const uint_t index)
	{
		return GenericGetMethode<math::Vector2f, ValueVector2>(index);
	}
	inline const math::Vector3f& GetVector3(const uint_t index)
	{
		return GenericGetMethode<math::Vector3f, ValueVector3>(index);
	}
	inline const math::Vector4f& GetVector4(const uint_t index)
	{
		return GenericGetMethode<math::Vector4f, ValueVector4>(index);
	}
	inline const math::Quaternionf& GetQuaternion(const uint_t index) 
	{
		return GenericGetMethode<math::Quaternionf, ValueQuaternion>(index);
	}

	// Spezifische Set-Methoden
	template <
		typename TypeT,
		int ValueT
	>
	inline void GenericSetMethode(const uint_t index, const TypeT& v)
	{
		if (GetValueType() != ValueT)
		{
			throw Exception("Invalid Type!");
		}

		uint_t stride = 0;
		ubyte_t* data = GetData(stride);
		data += stride * index;
		*((TypeT*)data) = v;
	}
	inline void SetUByte4(const uint_t index, const math::Vector4<ubyte_t>& v)
	{
		GenericSetMethode<math::Vector4<ubyte_t>, ValueUByte4>(index, v);
	}
	inline void SetColor(const uint_t index, const math::Colorf& v)
	{
		GenericSetMethode<math::Colorf, ValueColor>(index, v);
	}
	inline void SetFloat(const uint_t index, const float_t& v)
	{
		GenericSetMethode<float_t, ValueFloat>(index, v);
	}
	inline void SetVector2(const uint_t index, const math::Vector2f& v)
	{
		GenericSetMethode<math::Vector2f, ValueVector2>(index, v);
	}
	inline void SetVector3(const uint_t index, const math::Vector3f& v)
	{
		GenericSetMethode<math::Vector3f, ValueVector3>(index, v);
	}
	inline void SetVector4(const uint_t index, const math::Vector4f& v)
	{
		GenericSetMethode<math::Vector4f, ValueVector4>(index, v);
	}
	inline void SetQuaternion(const uint_t index, const math::Quaternionf& v) 
	{
		GenericSetMethode<math::Quaternionf, ValueQuaternion>(index, v);
	}
};

}} // namespace x::model

#endif // MODEL_STREAM_H

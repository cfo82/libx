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

/* This file is part of libmspack.
 * (C) 2003-2004 Stuart Caie.
 *
 * The deflate method was created by Phil Katz. MSZIP is equivalent to the
 * deflate method.
 *
 * libmspack is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (LGPL) version 2.1
 *
 * For further details, see the file COPYING.LIB distributed with libmspack
 */

/* MS-ZIP decompression implementation. */

//-----------------------------------------------------------------------------------------------------------
/*!	\file scanner/compressedbinaryscanner.h
	\brief Definition von x::scanner::CompressedBinaryScanner
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::scanner::CompressedBinaryScanner
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef SCANNER_COMPRESSEDBINARYSCANNER_H
#define SCANNER_COMPRESSEDBINARYSCANNER_H

#include "../global/global.h"
#include "../tokens/tokens.h"
#include "./binaryscanner.h"
#include "./mszip.h"

namespace x { namespace scanner {

#define CHUNK 16384
#define MSZIP_FRAME_SIZE          (32768) /* size of LZ history window */
#define MSZIP_LITERAL_MAXSYMBOLS  (288)   /* literal/length huffman tree */
#define MSZIP_DISTANCE_MAXSYMBOLS (32)    /* distance huffman tree */

/* if there are less direct lookup entries than symbols, the longer
 * code pointers will be <= maxsymbols. This must not happen, or we
 * will decode entries badly */
#if (1 << MSZIP_LITERAL_TABLEBITS) < (MSZIP_LITERAL_MAXSYMBOLS * 2)
# define MSZIP_LITERAL_TABLESIZE (MSZIP_LITERAL_MAXSYMBOLS * 4)
#else
# define MSZIP_LITERAL_TABLESIZE ((1 << MSZIP_LITERAL_TABLEBITS) + \
				  (MSZIP_LITERAL_MAXSYMBOLS * 2))
#endif

#if (1 << MSZIP_DISTANCE_TABLEBITS) < (MSZIP_DISTANCE_MAXSYMBOLS * 2)
# define MSZIP_DISTANCE_TABLESIZE (MSZIP_DISTANCE_MAXSYMBOLS * 4)
#else
# define MSZIP_DISTANCE_TABLESIZE ((1 << MSZIP_DISTANCE_TABLEBITS) + \
				  (MSZIP_DISTANCE_MAXSYMBOLS * 2))
#endif

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief X-File-CompressedBinaryScanner
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Klasse fürs Handling von Tokens
*/
class CompressedBinaryScanner : public BinaryScanner
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	CompressedBinaryScanner();
	virtual ~CompressedBinaryScanner();

protected:
	virtual bool_t FillBuffer(TemporalInfo& info);

private:
	bool_t FlushWindow(unsigned int data_flushed);
	bool_t MakeDecodeTable(unsigned int nsyms, unsigned int nbits, unsigned char* length, unsigned short* table);
	bool_t ZipReadLens(TemporalInfo& info);
	bool_t Inflate(TemporalInfo& info);

protected:
	//mspack_system system_;
	
	unsigned int window_posn_; // offset within window
	int bytes_output_;

	ubyte_t* inbuf_;

	ubyte_t* i_ptr_;
	ubyte_t* i_end_;

	ubyte_t* o_ptr_;
	ubyte_t* o_end_;

	uint_t bit_buffer_;
	uint_t bits_left_;
	uint_t inbuf_size_;

	/* huffman code lengths */
	unsigned char  LITERAL_len_[MSZIP_LITERAL_MAXSYMBOLS];
	unsigned char  DISTANCE_len_[MSZIP_DISTANCE_MAXSYMBOLS];

	/* huffman decoding tables */
	unsigned short LITERAL_table_[MSZIP_LITERAL_TABLESIZE];
	unsigned short DISTANCE_table_[MSZIP_DISTANCE_TABLESIZE];

	/* 32kb history window */
	ubyte_t window_[MSZIP_FRAME_SIZE];
};

}} // namespace x::scanner

#endif // SCANNER_COMPRESSEDBINARYSCANNER_H

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
/*!	\file scanner/compressedbinaryscanner.cpp
	\brief Implementation von x::scanner::CompressedBinaryScanner
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::scanner::CompressedBinaryScanner
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "./compressedbinaryscanner.h"

namespace x { namespace scanner {

using namespace x::tokens;

/* match lengths for literal codes 257.. 285 */
static const unsigned short lit_lengths[29] = {
  3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27,
  31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258
};

/* match offsets for distance codes 0 .. 29 */
static const unsigned short dist_offsets[30] = {
  1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385,
  513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577
};

/* extra bits required for literal codes 257.. 285 */
static const ubyte_t lit_extrabits[29] = {
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2,
  2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0
};

/* extra bits required for distance codes 0 .. 29 */
static const ubyte_t dist_extrabits[30] = {
  0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6,
  6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13
};

/* the order of the bit length Huffman code lengths */
static const ubyte_t bitlen_order[19] = {
	16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

/* ANDing with bit_mask[n] masks the lower n bits */
static const unsigned short bit_mask[17] = {
 0x0000, 0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
 0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
};

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
CompressedBinaryScanner::CompressedBinaryScanner()
:	BinaryScanner()
{
	uint_t input_buffer_size = (CHUNK + 1) & -2;

	/* allocate input buffer */
	inbuf_ =(ubyte_t*) malloc(input_buffer_size);

	/* initialise decompression state */
	inbuf_size_ = input_buffer_size;
	i_ptr_ = i_end_ = &inbuf_[0];
	o_ptr_ = o_end_ = NULL;
	bit_buffer_ = 0;
	bits_left_ = 0;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
CompressedBinaryScanner::~CompressedBinaryScanner()
{
	free(inbuf_);
}




//-----------------------------------------------------------------------------------------------------------
bool_t CompressedBinaryScanner::FillBuffer(
	TemporalInfo& info
)
{
	int i;
	int state;

	// easy answers
	if (info.error_flag)
	{
		return false;
	}

	// flush out any stored-up bytes before we begin
	i = (int)(o_end_ - o_ptr_);
	if ((uint_t)i > TemporalBufferSize)
		{ i = (int)TemporalBufferSize; }
	if (i)
	{
		memcpy(buffer_, o_ptr_, i);
		o_ptr_ += i;
		
		buffer_size_ = i;
		buffer_index_ = 0;
		return true;
	}

	// skip to next read 'CK' header
	i = bits_left_ & 7;
	/* align to bytestream */
	bit_buffer_ >>= i;
	bits_left_ -= i;
	state = 0;
	do
	{
		while (bits_left_ < 8)
		{
			if (i_ptr_ >= i_end_)
			{
				int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
				if (data_read < 0)
				{
					info.error_flag = true;
					info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
					return false;
				}
				i_ptr_ = &inbuf_[0];
				i_end_ = &inbuf_[data_read];
			}
			bit_buffer_ |= *i_ptr_++ << bits_left_;
			bits_left_  += 8;
		}
		i = bit_buffer_ & ((1<<8)-1);
		bit_buffer_ >>= 8;
		bits_left_ -= 8;
		if (i == 'C')
		{
			state = 1;
		}
		else if ((state == 1) && (i == 'K'))
		{
			state = 2;
		}
		else
		{
			state = 0;
		}
	} while (state != 2);

	/* inflate a block, repair and realign if necessary */
	window_posn_ = 0;
	bytes_output_ = 0;
	
	if (!Inflate(info))
	{
		/*D(("inflate error %d", i))
		if (zip->repair_mode)
		{
			zip->sys->message(NULL, "MSZIP error, %u bytes of data lost.",
			MSZIP_FRAME_SIZE - zip->bytes_output);
			for (i = zip->bytes_output; i < MSZIP_FRAME_SIZE; i++)
			{
				window_[i] = '\0';
			}
			zip->bytes_output = MSZIP_FRAME_SIZE;
		}
		else
		{
			return zip->error = (error > 0) ? error : MSPACK_ERR_DECRUNCH;
		}*/
		info.error_flag = true;
		info.error_message += UNICODE_STRING("MSZIP Inflate Error.\n");
		return false;
	}
	o_ptr_ = &window_[0];
	o_end_ = &window_[bytes_output_];

	// write a frame
	i = (TemporalBufferSize < (unsigned int)bytes_output_) ? (int)TemporalBufferSize : bytes_output_;
	memcpy(buffer_, o_ptr_, i);
	buffer_index_ = 0;
	buffer_size_ = i;
	o_ptr_ += i;

	return true;
}




//-----------------------------------------------------------------------------------------------------------
bool_t CompressedBinaryScanner::FlushWindow(
	unsigned int data_flushed
	)
{
	bytes_output_ += data_flushed;
	if (bytes_output_ > MSZIP_FRAME_SIZE)
	{
		//D(("overflow: %u bytes flushed, total is now %u", data_flushed, zip->bytes_output))
		return false;
	}
	return true;
}




//-----------------------------------------------------------------------------------------------------------
bool_t CompressedBinaryScanner::MakeDecodeTable(
	unsigned int nsyms,
	unsigned int nbits,
	unsigned char* length,
	unsigned short* table
)
{
  register unsigned int leaf, reverse, fill;
  register unsigned short sym, next_sym;
  register unsigned char bit_num;
  unsigned int pos         = 0; /* the current position in the decode table */
  unsigned int table_mask  = 1 << nbits;
  unsigned int bit_mask    = table_mask >> 1; /* don't do 0 length codes */

  /* fill entries for codes short enough for a direct mapping */
  for (bit_num = 1; bit_num <= nbits; bit_num++) {
    for (sym = 0; sym < nsyms; sym++) {
      if (length[sym] != bit_num) continue;

      /* reverse the significant bits */
      fill = length[sym]; reverse = pos >> (nbits - fill); leaf = 0;
      do {leaf <<= 1; leaf |= reverse & 1; reverse >>= 1;} while (--fill);

      if((pos += bit_mask) > table_mask) return false; /* table overrun */

      /* fill all possible lookups of this symbol with the symbol itself */
      fill = bit_mask; next_sym = 1 << bit_num;
      do { table[leaf] = sym; leaf += next_sym; } while (--fill);
    }
    bit_mask >>= 1;
  }

  /* exit with success if table is now complete */
  if (pos == table_mask) return true;

  /* mark all remaining table entries as unused */
  for (sym = static_cast<unsigned short>(pos); sym < table_mask; sym++) {
    reverse = sym; leaf = 0; fill = nbits;
    do { leaf <<= 1; leaf |= reverse & 1; reverse >>= 1; } while (--fill);
    table[leaf] = 0xFFFF;
  }

  /* where should the longer codes be allocated from? */
  next_sym = static_cast<unsigned short>(((table_mask >> 1) < nsyms) ? nsyms : (table_mask >> 1));

  /* give ourselves room for codes to grow by up to 16 more bits.
   * codes now start at bit nbits+16 and end at (nbits+16-codelength) */
  pos <<= 16;
  table_mask <<= 16;
  bit_mask = 1 << 15;

  for (bit_num = static_cast<unsigned char>(nbits+1); bit_num <= MSZIP_MAX_HUFFBITS; bit_num++) {
    for (sym = 0; sym < nsyms; sym++) {
      if (length[sym] != bit_num) continue;

      /* leaf = the first nbits of the code, reversed */
      reverse = pos >> 16; leaf = 0; fill = nbits;
      do {leaf <<= 1; leaf |= reverse & 1; reverse >>= 1;} while (--fill);

      for (fill = 0; fill < (bit_num - nbits); fill++) {
	/* if this path hasn't been taken yet, 'allocate' two entries */
	if (table[leaf] == 0xFFFF) {
	  table[(next_sym << 1)     ] = 0xFFFF;
	  table[(next_sym << 1) + 1 ] = 0xFFFF;
	  table[leaf] = next_sym++;
	}
	/* follow the path and select either left or right for next bit */
	leaf = (table[leaf] << 1) | ((pos >> (15 - fill)) & 1);
      }
      table[leaf] = sym;

      if ((pos += bit_mask) > table_mask) return false; /* table overflow */
    }
    bit_mask >>= 1;
  }

  /* full table? */
  return (pos != table_mask) ? false : true;
}




//-----------------------------------------------------------------------------------------------------------
bool_t CompressedBinaryScanner::ZipReadLens(
	TemporalInfo& info
)
{
	/* bitlen Huffman codes -- immediate lookup, 7 bit max code length */
	unsigned short bl_table[(1 << 7)];
	unsigned char bl_len[19];

	unsigned char lens[MSZIP_LITERAL_MAXSYMBOLS + MSZIP_DISTANCE_MAXSYMBOLS];
	unsigned int lit_codes, dist_codes, code, last_code=0, bitlen_codes, i, run;

	/* read the number of codes */
	while (bits_left_ < 5)
	{
		if (i_ptr_ >= i_end_)
		{
			int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
			if (data_read < 0)
			{
				info.error_flag = true;
				info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
				return false;
			}
			i_ptr_ = &inbuf_[0];
			i_end_ = &inbuf_[data_read];
		}
		bit_buffer_ |= *i_ptr_++ << bits_left_;
		bits_left_  += 8;
	}
	lit_codes = bit_buffer_ & ((1<<5)-1);
	bit_buffer_ >>= 5;
	bits_left_ -= 5;
	lit_codes    += 257;

	while (bits_left_ < 5)
	{
		if (i_ptr_ >= i_end_)
		{
			int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
			if (data_read < 0)
			{
				info.error_flag = true;
				info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
				return false;
			}
			i_ptr_ = &inbuf_[0];
			i_end_ = &inbuf_[data_read];
		}
		bit_buffer_ |= *i_ptr_++ << bits_left_;
		bits_left_ += 8;
	}
	dist_codes = bit_buffer_ & ((1<<5)-1);
	bit_buffer_ >>= 5;
	bits_left_ -= 5;
	dist_codes   += 1;

	while (bits_left_ < 4)
	{
		if (i_ptr_ >= i_end_)
		{
			int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
			if (data_read < 0)
			{
				info.error_flag = true;
				info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
				return false;
			}
			i_ptr_ = &inbuf_[0];
			i_end_ = &inbuf_[data_read];
		}
		bit_buffer_ |= *i_ptr_++ << bits_left_;
		bits_left_ += 8;
	}
	bitlen_codes = bit_buffer_ & ((1<<4)-1);
	bit_buffer_ >>= 4;
	bits_left_ -= 4;
	bitlen_codes += 4;

	if (lit_codes  > MSZIP_LITERAL_MAXSYMBOLS)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("MSZIP error: too many symbols in blocktype 2 header.\n");
		return false;
	}
	if (dist_codes > MSZIP_DISTANCE_MAXSYMBOLS)
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("MSZIP error: too many symbols in blocktype 2 header.\n");
		return false;
	}

	/* read in the bit lengths in their unusual order */
	for (i = 0; i < bitlen_codes; i++)
	{
		while (bits_left_ < 3) {
			if (i_ptr_ >= i_end_) {
				int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
				if (data_read < 0)
				{
					info.error_flag = true;
					info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
					return false;
				}
				i_ptr_ = &inbuf_[0];
				i_end_ = &inbuf_[data_read];
			}
			bit_buffer_ |= *i_ptr_++ << bits_left_;
			bits_left_ += 8;
		}
		bl_len[bitlen_order[i]] = static_cast<unsigned char>(bit_buffer_ & ((1<<3)-1));
		bit_buffer_ >>= 3;
		bits_left_ -= 3;
	}
	while (i < 19)
	{
		bl_len[bitlen_order[i++]] = 0;
	}

	/* create decoding table with an immediate lookup */
	if (!MakeDecodeTable(19, 7, &bl_len[0], &bl_table[0]))
	{
		info.error_flag = true;
		info.error_message += UNICODE_STRING("MSZIP error: failed to build bitlens huffman table.\n");
		return false;
	}

	/* read literal / distance code lengths */
	for (i = 0; i < (lit_codes + dist_codes); i++)
	{
		/* single-level huffman lookup */
		while (bits_left_ < 7)
		{
			if (i_ptr_ >= i_end_)
			{
				int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
				if (data_read < 0)
				{
					info.error_flag = true;
					info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
					return false;
				}
				i_ptr_ = &inbuf_[0];
				i_end_ = &inbuf_[data_read];
			}
			bit_buffer_ |= *i_ptr_++ << bits_left_;
			bits_left_ += 8;
		}

		code = bl_table[bit_buffer_ & ((1<<7)-1)];
		bit_buffer_ >>= bl_len[code];
		bits_left_ -= bl_len[code];

		if (code < 16)
		{
			last_code = code;
			lens[i] = static_cast<unsigned char>(code);
		}
		else
		{
			switch (code) {
			case 16: {
				while (bits_left_ < 2) {
					if (i_ptr_ >= i_end_) {
						int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
						if (data_read < 0)
						{
							info.error_flag = true;
							info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
							return false;
						}
						i_ptr_ = &inbuf_[0];
						i_end_ = &inbuf_[data_read];
					}
					bit_buffer_ |= *i_ptr_++ << bits_left_;
					bits_left_ += 8;
				}
				run = bit_buffer_ & ((1<<2)-1);
				bit_buffer_ >>= 2;
				bits_left_ -= 2;
				run += 3;
				code = last_code;
				break;
			}
			case 17: {
				while (bits_left_ < 3) {
					if (i_ptr_ >= i_end_) {
						int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
						if (data_read < 0)
						{
							info.error_flag = true;
							info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
							return false;
						}
						i_ptr_ = &inbuf_[0];
						i_end_ = &inbuf_[data_read];
					}
					bit_buffer_ |= *i_ptr_++ << bits_left_;
					bits_left_ += 8;
				}
				run = bit_buffer_ & ((1<<3)-1);
				bit_buffer_ >>= 3;
				bits_left_ -= 3;
				run += 3;
				code = 0;
				break;
			}
			case 18: {
				while (bits_left_ < 7) {
					if (i_ptr_ >= i_end_) {
						int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
						if (data_read < 0)
						{
							info.error_flag = true;
							info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
							return false;
						}
						i_ptr_ = &inbuf_[0];
						i_end_ = &inbuf_[data_read];
					}
					bit_buffer_ |= *i_ptr_++ << bits_left_;
					bits_left_ += 8;
				}
				run = bit_buffer_ & ((1<<7)-1);
				bit_buffer_ >>= 7;
				bits_left_ -= 7;
				run += 11;
				code = 0;
				break;
			}
			default: {
				//D(("bad code!: %u", code))
				info.error_flag = true;
				info.error_message += UNICODE_STRING("MSZIP error: invalid bit-length code.\n");
				return false;
			}
			}
			if ((i + run) > (lit_codes + dist_codes))
			{
				info.error_flag = true;
				info.error_message += UNICODE_STRING("MSZIP error: bitlen RLE code goes over table size.\n");
				return false;
			}
			while (run--)
			{
				lens[i++] = static_cast<unsigned char>(code);
			}
			i--;
		}
	}

	/* copy LITERAL code lengths and clear any remaining */
	i = lit_codes;
	memcpy(LITERAL_len_, lens, i);
	while (i < MSZIP_LITERAL_MAXSYMBOLS)
	{
		LITERAL_len_[i++] = 0;
	}

	i = dist_codes;
	memcpy(DISTANCE_len_, &lens[lit_codes], i);
	while (i < MSZIP_DISTANCE_MAXSYMBOLS)
	{
		DISTANCE_len_[i++] = 0;
	}

	return true;
}




//-----------------------------------------------------------------------------------------------------------
/* a clean implementation of RFC 1951 / inflate */
bool_t CompressedBinaryScanner::Inflate(
	TemporalInfo& info
)
{
	unsigned int last_block;
	unsigned int block_type;
	unsigned int distance;
	unsigned int length;
	unsigned int this_run;
	unsigned int i;
	register unsigned short sym;

	do
	{
		/* read in last block bit */
		while (bits_left_ < 1)
		{
			if (i_ptr_ >= i_end_)
			{
				int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
				if (data_read < 0)
				{
					info.error_flag = true;
					info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
					return false;
				}
				i_ptr_ = &inbuf_[0];
				i_end_ = &inbuf_[data_read];
			}
			bit_buffer_ |= *i_ptr_++ << bits_left_;
			bits_left_  += 8;
		}
		last_block = bit_buffer_ & ((1<<1)-1);
		bit_buffer_ >>= 1;
		bits_left_ -= 1;

		/* read in block type */
		while (bits_left_ < 2)
		{
			if (i_ptr_ >= i_end_)
			{
				int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
				if (data_read < 0)
				{
					info.error_flag = true;
					info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
					return false;
				}
				i_ptr_ = &inbuf_[0];
				i_end_ = &inbuf_[data_read];
			}
			bit_buffer_ |= *i_ptr_++ << bits_left_;
			bits_left_  += 8;
		}
		block_type = bit_buffer_ & ((1<<2)-1); 
		bit_buffer_ >>= 2;
		bits_left_ -= 2;
		//D(("block_type=%u last_block=%u", block_type, last_block))

		if (block_type == 0)
		{
			/* uncompressed block */
			unsigned char lens_buf[4];

			/* go to byte boundary */
			i = bits_left_ & 7;
			bit_buffer_ >>= i;
			bits_left_ -= i;

			/* read 4 bytes of data, emptying the bit-buffer if necessary */
			for (i = 0; (bits_left_ >= 8); i++)
			{
				if (i == 4)
				{
					info.error_flag = true;
					info.error_message += UNICODE_STRING("MSZIP error: too many bits in bit buffer.\n");
					return false;
				}
				lens_buf[i] = static_cast<unsigned char>(bit_buffer_ & ((1<<8)-1));
				bit_buffer_ >>= 8;
				bits_left_ -= 8;
			}
			if (bits_left_ != 0)
			{
				info.error_flag = true;
				info.error_message += UNICODE_STRING("MSZIP error: too many bits in bit buffer.\n");
				return false;
			}
			while (i < 4)
			{
				if (i_ptr_ >= i_end_)
				{
					int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
					if (data_read < 0)
					{
						info.error_flag = true;
						info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
						return false;
					}
					i_ptr_ = &inbuf_[0];
					i_end_ = &inbuf_[data_read];
				}
				lens_buf[i++] = *i_ptr_++;
			}

			/* get the length and its complement */
			length = lens_buf[0] | (lens_buf[1] << 8);
			i      = lens_buf[2] | (lens_buf[3] << 8);
			if (length != (~i & 0xFFFF))
			{
				info.error_flag = true;
				info.error_message += UNICODE_STRING("MSZIP error: block size complement mismatch.\n");
				return false;
			}

			/* read and copy the uncompressed data into the window */
			while (length > 0)
			{
				if (i_ptr_ >= i_end_)
				{
					int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
					if (data_read < 0)
					{
						info.error_flag = true;
						info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
						return false;
					}
					i_ptr_ = &inbuf_[0];
					i_end_ = &inbuf_[data_read];
				}

				this_run = length;
				if (this_run > (unsigned int)(i_end_ - i_ptr_))
				{
					this_run = (int)(i_end_ - i_ptr_);
				}
				if (this_run > (MSZIP_FRAME_SIZE - window_posn_))
				{
					this_run = MSZIP_FRAME_SIZE - window_posn_;
				}

				memcpy(&window_[window_posn_], i_ptr_, this_run);
				window_posn_ += this_run;
				i_ptr_    += this_run;
				length   -= this_run;

				if (window_posn_ == MSZIP_FRAME_SIZE)
				{
					if (!FlushWindow(MSZIP_FRAME_SIZE))
					{
						info.error_flag = true;
						info.error_message += UNICODE_STRING("MSZIP error: error from flush_window() callback.\n");
						return false;
					}
					window_posn_ = 0;
				}
			}
		}
		else if ((block_type == 1) || (block_type == 2))
		{
			/* Huffman-compressed LZ77 block */
			unsigned int window_posn, match_posn, code;

			if (block_type == 1)
			{
				/* block with fixed Huffman codes */
				i = 0;
				while (i < 144) LITERAL_len_[i++] = 8;
				while (i < 256) LITERAL_len_[i++] = 9;
				while (i < 280) LITERAL_len_[i++] = 7;
				while (i < 288) LITERAL_len_[i++] = 8;
				for (i = 0; i < 32; i++)
				{
					DISTANCE_len_[i] = 5;
				}
			}
			else
			{
				/* block with dynamic Huffman codes */
				if (!ZipReadLens(info))
				{
					return false;
				}
			}

			/* now huffman lengths are read for either kind of block, 
			* create huffman decoding tables */
			if (!MakeDecodeTable(MSZIP_LITERAL_MAXSYMBOLS, MSZIP_LITERAL_TABLEBITS,
				&LITERAL_len_[0], &LITERAL_table_[0]))
			{
				info.error_flag = true;
				info.error_message += UNICODE_STRING("MSZIP error: failed to build literals huffman table.\n");
				return false;
			}

			if (!MakeDecodeTable(MSZIP_DISTANCE_MAXSYMBOLS,MSZIP_DISTANCE_TABLEBITS,
				&DISTANCE_len_[0], &DISTANCE_table_[0]))
			{
				info.error_flag = true;
				info.error_message += UNICODE_STRING("MSZIP error: failed to build distance huffman table.\n");
				return false;
			}

			/* decode forever until end of block code */
			window_posn = window_posn_;
			for(;;)
			{
				/* huffman symbols can be up to 16 bits long */
				while (bits_left_ < MSZIP_MAX_HUFFBITS) {
					if (i_ptr_ >= i_end_) {
						int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
						if (data_read < 0)
						{
							info.error_flag = true;
							info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
							return false;
						}
						i_ptr_ = &inbuf_[0];
						i_end_ = &inbuf_[data_read];
					}
					bit_buffer_ |= *i_ptr_++ << bits_left_;
					bits_left_ += 8;
				}
				/* immediate table lookup of [tablebits] bits of the code */
				sym = LITERAL_table_[bit_buffer_ & ((1<<MSZIP_LITERAL_TABLEBITS)-1)];
				/* is the symbol is longer than [tablebits] bits? (i=node index) */
				if (sym >= MSZIP_LITERAL_MAXSYMBOLS) {
					/* decode remaining bits by tree traversal */
					i = MSZIP_LITERAL_TABLEBITS - 1;
					do {
						/* check next bit. error if we run out of bits before decode */
						if (i++ > MSZIP_MAX_HUFFBITS)
						{
							//D(("out of bits in huffman decode"))
							info.error_flag = true;
							info.error_message += UNICODE_STRING("MSZIP error: out of bits decoding huffman symbol.\n");
							return false;
						}
						/* double node index and add 0 (left branch) or 1 (right) */
						sym = LITERAL_table_[(sym << 1) | ((bit_buffer_ >> i) & 1)];
						/* while we are still in node indicies, not decoded symbols */
					} while (sym >= MSZIP_LITERAL_MAXSYMBOLS);
				}
				/* result */
				code = sym;
				/* look up the code length of that symbol and discard those bits */
				i = LITERAL_len_[sym];
				bit_buffer_ >>= i;
				bits_left_ -= i;

				if (code < 256)
				{
					window_[window_posn++] = (unsigned char) code;
					if (window_posn == MSZIP_FRAME_SIZE)
					{
						if (!FlushWindow(MSZIP_FRAME_SIZE))
						{
							info.error_flag = true;
							info.error_message += UNICODE_STRING("MSZIP error: error from flush_window() callback.\n");
							return false;
						}
						window_posn = 0;
					}
				}
				else if (code == 256)
				{
					/* END OF BLOCK CODE: loop break point */
					break;
				}
				else
				{
					code -= 257;
					if (code > 29)
					{
						info.error_flag = true;
						info.error_message += UNICODE_STRING("MSZIP error: out-of-range literal code.\n");
						return false;
					}
					while (bits_left_ < lit_extrabits[code]) {
						if (i_ptr_ >= i_end_) {
							int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
							if (data_read < 0)
							{
								info.error_flag = true;
								info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
								return false;
							}
							i_ptr_ = &inbuf_[0];
							i_end_ = &inbuf_[data_read];
						}
						bit_buffer_ |= *i_ptr_++ << bits_left_;
						bits_left_ += 8;
					}
					length = bit_buffer_ & bit_mask[lit_extrabits[code]];
					bit_buffer_ >>= lit_extrabits[code];
					bits_left_ -= lit_extrabits[code];
					length += lit_lengths[code];

					/* huffman symbols can be up to 16 bits long */
					while (bits_left_ < MSZIP_MAX_HUFFBITS) {
						if (i_ptr_ >= i_end_) {
							int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
							if (data_read < 0)
							{
								info.error_flag = true;
								info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
								return false;
							}
							i_ptr_ = &inbuf_[0];
							i_end_ = &inbuf_[data_read];
						}
						bit_buffer_ |= *i_ptr_++ << bits_left_;
						bits_left_ += 8;
					}
					/* immediate table lookup of [tablebits] bits of the code */
					sym = DISTANCE_table_[bit_buffer_ & ((1<<MSZIP_DISTANCE_TABLEBITS)-1)];
					/* is the symbol is longer than [tablebits] bits? (i=node index) */
					if (sym >= MSZIP_DISTANCE_MAXSYMBOLS) {
						/* decode remaining bits by tree traversal */
						i = MSZIP_DISTANCE_TABLEBITS - 1;
						do {
							/* check next bit. error if we run out of bits before decode */
							if (i++ > MSZIP_MAX_HUFFBITS) {
								//D(("out of bits in huffman decode"))
								info.error_flag = true;
								info.error_message += UNICODE_STRING("MSZIP error: out of bits decoding huffman symbol.\n");
								return false;
							}
							/* double node index and add 0 (left branch) or 1 (right) */
							sym = DISTANCE_table_[(sym << 1) | ((bit_buffer_ >> i) & 1)];
							/* while we are still in node indicies, not decoded symbols */
						} while (sym >= MSZIP_DISTANCE_MAXSYMBOLS);
					}
					/* result */
					code = sym;
					/* look up the code length of that symbol and discard those bits */
					i = DISTANCE_len_[sym];
					bit_buffer_ >>= i;
					bits_left_ -= i;
					if (code > 30)
					{
						info.error_flag = true;
						info.error_message += UNICODE_STRING("MSZIP error: out-of-range distance code.\n");
						return false;
					}
					while (bits_left_ < dist_extrabits[code]) {
						if (i_ptr_ >= i_end_) {
							int data_read = info.read_function(inbuf_, inbuf_size_, info.io_token);
							if (data_read < 0)
							{
								info.error_flag = true;
								info.error_message += UNICODE_STRING("Error while reading from input stream.\n");
								return false;
							}
							i_ptr_ = &inbuf_[0];
							i_end_ = &inbuf_[data_read];
						}
						bit_buffer_ |= *i_ptr_++ << bits_left_;
						bits_left_ += 8;
					}
					distance = bit_buffer_ & bit_mask[dist_extrabits[code]];
					bit_buffer_ >>= dist_extrabits[code];
					bits_left_ -= dist_extrabits[code];
					distance += dist_offsets[code];

					/* match position is window position minus distance. If distance
					* is more than window position numerically, it must 'wrap
					* around' the frame size. */ 
					match_posn = ((distance > window_posn) ? MSZIP_FRAME_SIZE : 0)
					+ window_posn - distance;

					/* copy match */
					if (length < 12)
					{
						/* short match, use slower loop but no loop setup code */
						while (length--)
						{
							window_[window_posn++] = window_[match_posn++];
							match_posn &= MSZIP_FRAME_SIZE - 1;

							if (window_posn == MSZIP_FRAME_SIZE)
							{
								if (!FlushWindow(MSZIP_FRAME_SIZE))
								{
									info.error_flag = true;
									info.error_message += UNICODE_STRING("MSZIP error: error from flush_window() callback.\n");
									return false;
								}
								window_posn = 0;
							}
						}
					}
					else
					{
						/* longer match, use faster loop but with setup expense */
						unsigned char *runsrc, *rundest;
						do
						{
							this_run = length;
							if ((match_posn + this_run) > MSZIP_FRAME_SIZE)
							this_run = MSZIP_FRAME_SIZE - match_posn;
							if ((window_posn + this_run) > MSZIP_FRAME_SIZE)
							this_run = MSZIP_FRAME_SIZE - window_posn;

							rundest = &window_[window_posn]; window_posn += this_run;
							runsrc  = &window_[match_posn];  match_posn  += this_run;
							length -= this_run;
							while (this_run--) *rundest++ = *runsrc++;

							/* flush if necessary */
							if (window_posn == MSZIP_FRAME_SIZE)
							{
								if (!FlushWindow(MSZIP_FRAME_SIZE))
								{
									info.error_flag = true;
									info.error_message += UNICODE_STRING("MSZIP error: error from flush_window() callback.\n");
									return false;
								}
								window_posn = 0;
							}
							if (match_posn == MSZIP_FRAME_SIZE) match_posn = 0;
						} while (length > 0);
					}
				} /* else (code >= 257) */
			} /* while (forever) -- break point at 'code == 256' */
			window_posn_ = window_posn;
		}
		else
		{
			/* block_type == 3 -- bad block type */
			info.error_flag = true;
			info.error_message += UNICODE_STRING("MSZIP error: unknown block type.\n");
		}
	} while (!last_block);

	/* flush the remaining data */
	if (window_posn_)
	{
		if (!FlushWindow(window_posn_))
		{
			info.error_flag = true;
			info.error_message += UNICODE_STRING("MSZIP error: error from flush_window() callback.\n");
			return false;
		}
	}

	// return success
	return true;
}

}} // namespace x::scanner


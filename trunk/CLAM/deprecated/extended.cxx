/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*
** This code is taken from aiff.c from libsndfile by Erik de Castro Lopo
** http://www.zip.com.au/~erikd/libsndfile/libsndfile-1.0.1.tar.gz
** 
** This is a rough hack at converting from 80 bit IEEE float in AIFF header to an
** int and back again. It assumes that all sample rates are between 1 and 800MHz,
** which should be OK as other sound file formats use a 32 bit integer to store
** sample rate.
**
** There is another (probably better) version in the source code to the SoX but it
** has a copyright which probably prevents it from being allowable as GPL/LGPL.
**
** Another, very complete, implementation of the same conversions, written by
** Ken Turkowski <turk@computer.org> is available at
** http://www.worldserver.com/turk/opensource/ToFromIEEE.c.txt
**
** =========================================================================================
**
** Copyright (C) 1999-2002 Erik de Castro Lopo <erikd@zip.com.au>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation; either version 2.1 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <stdlib.h>
#include <memory.h>

int
tenbytefloat2int (unsigned char *bytes)
{       int val = 3 ;

	if (bytes [0] & 0x80)   /* Negative number. */
		return 0 ;

	if (bytes [0] <= 0x3F)  /* Less than 1. */
		return 1 ;

	if (bytes [0] > 0x40)   /* Way too big. */
		return  0x4000000 ;

	if (bytes [0] == 0x40 && bytes [1] > 0x1C) /* Too big. */
		return 800000000 ;

	/* Ok, can handle it. */

	val = (bytes [2] << 23) |
	 (bytes [3] << 15) | (bytes [4] << 7)  | (bytes [5] >> 1) ;

	val >>= (29 - bytes [1]) ;

	return val ;
} /* tenbytefloat2int */


void
uint2tenbytefloat (unsigned int num, unsigned char *bytes)
{	unsigned int mask = 0x40000000 ;
	int		count ;


	memset (bytes, 0, 10) ;

	if (num <= 1)
	{	bytes [0] = 0x3F ;
		bytes [1] = 0xFF ;
		bytes [2] = 0x80 ;
		return ;
		} ;

	bytes [0] = 0x40 ;

	if (num >= mask)
	{	bytes [1] = 0x1D ;
		return ;
		} ;

	for (count = 0 ; count <= 32 ; count ++)
	{	if (num & mask)
			break ;
		mask >>= 1 ;
		} ;

	num <<= count + 1 ;
	bytes [1] = 29 - count ;
	bytes [2] = (num >> 24) & 0xFF ;
	bytes [3] = (num >> 16) & 0xFF ;
	bytes [4] = (num >>  8) & 0xFF ;
	bytes [5] = num & 0xFF ;

} /* uint2tenbytefloat */



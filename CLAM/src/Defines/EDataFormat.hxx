/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _EDataFormat_
#define _EDataFormat_

#include "ByteOrder.hxx"

enum EDataFormat
{
	eFmtDefault = 0,
	eFmtQuery = 0,
	eFmt8Bit = 0x0008,
	eFmt16Bit = 0x0010,
	eFmt32Bit = 0x0020,
	eFmt64Bit = 0x0040,
	eFmtInteger = 0x0100,
	eFmtFloat = 0x0200,
	eFmtUnsigned = 0x0000,
	eFmtSigned = 0x8000,
	eFmtLittleEndian = 0x1000,
	eFmtBigEndian = 0x2000,
#ifdef CLAM_LITTLE_ENDIAN
	eFmtNative = eFmtLittleEndian,
#else
	eFmtNative = eFmtBigEndian,
#endif
	eFmtF32N = eFmtFloat|eFmt32Bit|eFmtNative,
	eFmtF32L = eFmtFloat|eFmt32Bit|eFmtLittleEndian,
	eFmtF32B = eFmtFloat|eFmt32Bit|eFmtBigEndian,

	eFmtF64N = eFmtFloat|eFmt64Bit|eFmtNative,
	eFmtF64L = eFmtFloat|eFmt64Bit|eFmtLittleEndian,
	eFmtF64B = eFmtFloat|eFmt64Bit|eFmtBigEndian,

	eFmtS16N = eFmtInteger|eFmt16Bit|eFmtSigned|eFmtNative,
	eFmtS16L = eFmtInteger|eFmt16Bit|eFmtSigned|eFmtLittleEndian,
	eFmtS16B = eFmtInteger|eFmt16Bit|eFmtSigned|eFmtBigEndian,

	eFmtS32N = eFmtInteger|eFmt32Bit|eFmtSigned|eFmtNative,
	eFmtS32L = eFmtInteger|eFmt32Bit|eFmtSigned|eFmtLittleEndian,
	eFmtS32B = eFmtInteger|eFmt32Bit|eFmtSigned|eFmtBigEndian
};

inline EDataFormat DataFormat(const int&) { return eFmtS32N; }
inline EDataFormat DataFormat(const short&) { return eFmtS16N; }
inline EDataFormat DataFormat(const float&) { return eFmtF32N; }
inline EDataFormat DataFormat(const double&) { return eFmtF64N; }

#define GetDataFormat(ret,T) { T foo; ret = DataFormat(foo); }

#endif

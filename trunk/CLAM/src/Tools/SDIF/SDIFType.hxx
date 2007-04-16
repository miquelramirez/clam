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

#ifndef __SDIFType__
#define __SDIFType__


#include <string.h>
#include "DataTypes.hxx"

namespace SDIF
{

	/**
	* Used to store the Frame and Matrix types, a 4-byte
	* identifier
	*/
	class TypeId
	{
	friend class File;
	protected:
		/** The 4-byte identifier */
		CLAM::TByte mData[4];
	public:
		/** Create a new Type
		* @param str String containing the 4-byte identifier
		*/
		TypeId(const char* str = "\0\0\0\0")
		{
			memcpy(mData,str,4);	
		}
		/** Compare this Type
		* @param cmp The Type to compare with
		* @return true when equal
		*/
		bool operator == (const TypeId& cmp) { return !memcmp(mData,cmp.mData,4); }

		/** Convert the Type to a character string
		* @return The 4-byte identifier in a null-terminator const char string>         */
		const char* String(void) const
		{
			static char str[5];
			memcpy(str,mData,4);
			str[4]='\0';
			return str;
		}

		static TypeId sDefault;
	};

	enum DataType
	{
		eUnknown  = 0,
		eFloat32  =   0x4, // 32-bit big-endian IEEE 754 float, 4 bytes
		eFloat64  =   0x8, // 64-bit big-endian IEEE 754 float, 8 bytes
		eInt32    = 0x104, // 32-bit big-endian two's complement integer, 4 bytes
		eInt64    = 0x108, // 64-bit big-endian two's complement integer, 8 bytes
		eUint42   = 0x204, // 32-bit big-endian unsigned integer, 4 bytes
		eUTF8byte = 0x301, // Byte of UTF8-encoded text, 1 bytes
		eByte     = 0x401  // Arbitrary byte, 1 bytes
	};

	template <class T> class GetType
	{
	public:
		static DataType Get(void) { return eUnknown; }
	};
	
	class TUTF8byte
	{
	public:
		char c;
	};
	
	template <> class GetType<CLAM::TFloat32>{public:static DataType Get(void){return eFloat32;}};
	template <> class GetType<CLAM::TFloat64>{public:static DataType Get(void){return eFloat64;}};
	template <> class GetType<CLAM::TInt32>{public:static DataType Get(void){return eInt32;}};
	template <> class GetType<CLAM::TInt64>{public:static DataType Get(void){return eInt64;}};
	template <> class GetType<TUTF8byte>{public:static DataType Get(void){return eUTF8byte;}};
	template <> class GetType<CLAM::TByte>{public:static DataType Get(void){return eByte;}};

}
#endif


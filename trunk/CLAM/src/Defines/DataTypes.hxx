/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _CLAMDataTypes_
#define _CLAMDataTypes_

namespace CLAM {

typedef int TIndex;
typedef int TSize;
typedef short TInt16;
typedef unsigned short TUInt16;
typedef int TInt32;
typedef unsigned int TUInt32;
#ifdef _MSC_VER
typedef __int64 TInt64;
typedef unsigned __int64 TUInt64;
#else
typedef long long TInt64;
typedef unsigned long long TUInt64;
#endif
typedef float TFloat32;
typedef double TFloat64;
typedef unsigned char TByte;
typedef TByte TMIDIByte;
typedef TInt32 TMIDITime;
typedef double TTime;

#ifdef CLAM_DOUBLE
	typedef double TData;
#else
	typedef float TData;
#endif

/* the different FFT_Types */
enum EFFTType
{
	eNone,
	eRFFTW,
	eFFT_NREC,
	eFFT_IUA
};

}

#endif


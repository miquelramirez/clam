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

#ifndef __ByteOrder__
#define __ByteOrder__

// --------------------------------------
// Mac OS X

#ifdef __BIG_ENDIAN__
# define CLAM_BIG_ENDIAN
#endif

#ifdef __LITTLE_ENDIAN__
# define CLAM_LITTLE_ENDIAN
#endif

// --------------------------------------
// Linux

#ifdef linux
# include <sys/param.h>
# if __BYTE_ORDER == __LITTLE_ENDIAN
#  define CLAM_LITTLE_ENDIAN
# endif
# if __BYTE_ORDER == __BIG_ENDIAN
#  define CLAM_BIG_ENDIAN
# endif
#endif

// --------------------------------------
// Win32

#ifdef WIN32
# define CLAM_LITTLE_ENDIAN
#endif

// --------------------------------------
// Error Check

#ifdef CLAM_LITTLE_ENDIAN
# ifdef CLAM_BIG_ENDIAN
#  error confused about endianity
# endif
#endif

#ifndef CLAM_LITTLE_ENDIAN
# ifndef CLAM_BIG_ENDIAN
#  error could not determine endianity
# endif
#endif

// --------------------------------------
#endif


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

#ifndef __ByteOrder__
#define __ByteOrder__

#ifdef macintosh
# define CLAM_BIG_ENDIAN
#endif

#ifdef __powerpc__
# define CLAM_BIG_ENDIAN
#endif

#ifdef __POWERPC__
# define CLAM_BIG_ENDIAN
#endif

#ifdef linux
# include <sys/param.h>
# if __BYTE_ORDER == __LITTLE_ENDIAN
#  define CLAM_LITTLE_ENDIAN
# endif
# if __BYTE_ORDER == __BIG_ENDIAN
#  define CLAM_BIG_ENDIAN
# endif
#endif

#ifdef WIN32
# define CLAM_LITTLE_ENDIAN
#endif


#ifdef CLAM_LITTLE_ENDIAN
# ifdef CLAM_BIG_ENDIAN
Error: confused about endianity
# endif
#endif

#ifndef CLAM_LITTLE_ENDIAN
# ifndef CLAM_BIG_ENDIAN
Error: could not determine endianity
# endif
#endif

#endif

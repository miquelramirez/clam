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
#else
# ifdef linux
#  include <sys/param.h>
#  ifdef __BYTE_ORDER
#   if __BYTE_ORDER == __LITTLE_ENDIAN
#    define CLAM_LITTLE_ENDIAN
#   else
#    if __BYTE_ORDER == __BIG_ENDIAN
#     define CLAM_BIG_ENDIAN
#    else
Error: unknown byte order!
#    endif
#   endif
#  else
Error: unknown byte order!
#  endif /* __BYTE_ORDER */
# else
#  define CLAM_LITTLE_ENDIAN
# endif
#endif


#endif


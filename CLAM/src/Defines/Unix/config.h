/* ../src/Defines/Unix/config.h.  Generated automatically by configure.  */
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

#ifndef CLAM_CONFIG_H
#define CLAM_CONFIG_H



/* Define if your stdc++ library provides the sstream include file
 * Otherwhise, you will have to define the OBSOLETE_SSTREAM macro
 * with the name of an alternative file
 */
#define HAVE_STANDARD_SSTREAM 1

/* Define if your stdc++ library uses the name "strstream" for 
 * both the name of the include file and the name of the string stream
 * class.
 */
/* #undef HAVE_STRSTREAM_SSTREAM */

/* Define this macro if the str() method in the stringstream class
 * returns a string, as the standard requires.
 * Otherwhise it will be expected to return a char, as in old
 * implementations of the GNU libstdc++
 */
#define HAVE_STANDARD_SSTREAM_STR 1


/* Thefine this macro if the std::vector class provides an at()
 * method, as required by the standard.
 */
/* #undef HAVE_STANDARD_VECTOR_AT */

/* FFTW library header file name */
#define FFTW_HEADER <rfftw.h>

/* Defined when using float as data type */
/* #undef CLAM_FLOAT */

/* Defined when using double as data type */
#define CLAM_DOUBLE 1

/* Debuging flag */
#define _DEBUG 1

/* Defined when XML support is enabled */
#define CLAM_USE_XML 1

/* Obsolete debugging flag. Do not use it in your code. */
#define PARANOID 1

#define CLAM_HAVE_PTHREADS

#endif

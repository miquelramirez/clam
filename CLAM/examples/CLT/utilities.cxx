#ifndef CLT_UTILITIES
#define CLT_UTILITIES

/* utilities.cxx

   Computer Music Toolkit - a library of LADSPA plugins. Copyright (C)
   2000 Richard W.E. Furse. The author may be contacted at
   richard@muse.demon.co.uk.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public Licence as
   published by the Free Software Foundation; either version 2 of the
   Licence, or (at your option) any later version.

   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA. */

/*****************************************************************************/

/* This module contains code providing and supporting the
   CMT_Descriptor() function that provides hosts with initial access
   to LADSPA plugins. ALL PLUGINS MUST BE REGISTERED IN THIS FILE (see
   below). */

/*****************************************************************************/

/* Module Initialisation:
   ---------------------- */
void initialise_CLT_amplifier();
//void initialise_CLT_LP_filter();
//void initialise_CLT_HP_filter();
//void initialise_CLT_FD_filter();
/** This function should initialise all modules in the library. This
    will lead to all plugin descriptors being registered. If you write
    a new plugin you should initialise it here. If the module has
    structures it wishes to remove also then these should be included
    in finalise_modules(). */
void initialise_modules() 
{
		initialise_CLT_amplifier();
//		initialise_CLT_LP_filter();
//		initialise_CLT_HP_filter();
//		initialise_CLT_FD_filter();
}

/*****************************************************************************/

/* Module Finalisation:
   -------------------- */

//void finalise_CLT_LP_filter();
//void finalise_CLT_HP_filter();
//void finalise_CLT_FD_filter();

/** Finalise any structures allocated by the modules. This does not
    include descriptors passed to registerNewPluginDescriptor(). */
void finalise_modules() 
{
//		finalise_CLT_LP_filter();
//		finalise_CLT_HP_filter();
//		finalise_CLT_FD_filter();
}

/*****************************************************************************/
#endif
/* EOF */

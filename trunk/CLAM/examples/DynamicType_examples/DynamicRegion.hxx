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

// DynamicRegion.hxx. 
// Just another test class for DynamicTypes.
//
// written by Pau Arumí - May 2001
//////////////////////////////////////////////////////////////////////

#ifndef _DynamicRegion_
#define _DynamicRegion_

#include "DynamicType.hxx" 

class DynamicRegion : public CLAM::DynamicType
{
public:
	REG_NUM_ATTR (DynamicRegion, 5);

	
	REGISTER (0,	BeginTime, float);
	REGISTER (1,	EndTime,	float);
	REGISTER (2,	BeginAmplitude,		float);
	REGISTER (3,	EndAmplitude,	float);
	REGISTER (4,	Vibrato,	int);
	
};


#endif


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

#include "PitchNote.hxx"

namespace CLAM
{

/* Enum */
Enum::tEnumValue EPitch::sEnumValues[] = {	
	{EPitch::eC, "C"},
	{EPitch::eDb,"Db"},
	{EPitch::eD, "D"},
	{EPitch::eEb,"Eb"},
	{EPitch::eE, "E"},
	{EPitch::eF, "F"},
	{EPitch::eGb,"Gb"},
	{EPitch::eG, "G"},
	{EPitch::eAb,"Ab"},
	{EPitch::eA, "A"},
	{EPitch::eBb,"Bb"},
	{EPitch::eB, "B"},
	{EPitch::eNone,"NotSet"},
	{0,NULL}
};
Enum::tValue EPitch::sDefault = EPitch::eNone;

void PitchNote::DefaultInit() 
{  
	AddPitch();
	AddOctave();
	UpdateData();
	
	/* Default values */
	SetPitch(EPitch::eA);
	SetOctave(0);
}

} // namespace CLAM


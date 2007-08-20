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

#ifndef _ELoopPointUnits_
#define _ELoopPointUnits_

#include "Enum.hxx"
#include "DataTypes.hxx"

namespace CLAM{

/** PhaseGeneration
*   Enumeration to use for specifying the type of phase generation used
*/
class ELoopPointUnits : public Enum
{
public:
	ELoopPointUnits() : Enum(ValueTable(), eMilliseconds) {}
	ELoopPointUnits(tValue v) : Enum(ValueTable(), v) {};
	ELoopPointUnits(std::string s) : Enum(ValueTable(), s) {};

	typedef enum {
		eMilliseconds,
		eFrames,
	} tEnum;

	virtual Component* Species() const
	{
		return (Component*) new ELoopPointUnits;
	};
	static tEnumValue * ValueTable()
	{
		static tEnumValue sEnumValues[] = {
			{eMilliseconds,"Milliseconds"},
			{eFrames,"Frames"},
			{0,NULL}
		};
		return sEnumValues;
	}
};

}

#endif


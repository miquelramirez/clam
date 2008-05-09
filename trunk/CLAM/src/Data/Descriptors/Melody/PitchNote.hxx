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

#ifndef _PitchNote_
#define _PitchNote_
#include <typeinfo>

#include "DynamicType.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"
#include "Enum.hxx"

namespace CLAM
{

	// Class EPitch
	class EPitch : public Enum
	{
	public:
		EPitch() : Enum(ValueTable(), eNone) {}
		EPitch(tValue v) : Enum(ValueTable(), v) {};
		EPitch(std::string s) : Enum(ValueTable(), s) {};
		
		typedef enum {
			eC,eDb,eD,eEb,eE,eF,eGb,eG,eAb,eA,eBb,eB,eNone
		};	

		virtual Component* Species() const
		{
			return (Component*) new EPitch();
		};
		static tEnumValue * ValueTable()
		{
			static tEnumValue sEnumValues[] = {	
				{eC, "C"},
				{eDb,"Db"},
				{eD, "D"},
				{eEb,"Eb"},
				{eE, "E"},
				{eF, "F"},
				{eGb,"Gb"},
				{eG, "G"},
				{eAb,"Ab"},
				{eA, "A"},
				{eBb,"Bb"},
				{eB, "B"},
				{eNone,"NotSet"},
				{0,NULL}
			};
			return sEnumValues;
		}
	};
	

	///////////////////////////////////////////////////////
	// Class PitchNote:
	//

	/** Pitch note type definition = pitch + octave 
	 **/
	class PitchNote: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (PitchNote, 2, ProcessingData);
		DYN_ATTRIBUTE (0, public, EPitch, Pitch);   
		DYN_ATTRIBUTE (1, public, int,    Octave);
		
		void SetMIDIPitch(int);
	protected:
		void DefaultInit();
	};
	
} // namespace CLAM

#endif // _PitchNote_



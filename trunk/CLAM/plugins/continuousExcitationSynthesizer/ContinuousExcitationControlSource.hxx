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

#ifndef _ContinuousExcitationControlSource_HXX
#define _ContinuousExcitationControlSource_HXX

#include <string>
#include <vector>

namespace CLAM {

/**
* This class is an abstract overclass representing different data sources
* for continuous excitation information. Example subclasses might include
* a score file reader, a MIDI port reader, or an OSC port reader.
*
* \author greg kellum [gkellum@iua.upf.edu] 7/15/07
* \since  CLAM v1.1.
*/
class ContinuousExcitationControlSource
{
public:
	/**
	* This method can be used to ask for the next line of control information.
	*/
	virtual bool GetControlValues(float& pitch, float& amplitude) =0;
	
	void SetFeedForever(bool argFeedForever) { this->isFeedForever = argFeedForever; }
	
	bool IsFeedForever() { return this->isFeedForever; }

private:
	bool isFeedForever;
};

} // end namespace CLAM

#endif

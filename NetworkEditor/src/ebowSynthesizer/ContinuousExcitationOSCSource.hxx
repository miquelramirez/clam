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

#ifndef _ContinuousExcitationOSCSource_HXX
#define _ContinuousExcitationOSCSource_HXX

#include "OSCSource.hxx"
#include "ContinuousExcitationControlSource.hxx"
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
class ContinuousExcitationOSCSource : public ContinuousExcitationControlSource
{
public:
	ContinuousExcitationOSCSource(std::string targetName);
	~ContinuousExcitationOSCSource();

	/**
	* This method can be used to ask for the next line of control information.
	*/
	virtual bool GetControlValues(float& pitch, float& amplitude);

private:
	std::string mTargetName;
	OSCSource mOSCSource;
};

} // end namespace CLAM

#endif

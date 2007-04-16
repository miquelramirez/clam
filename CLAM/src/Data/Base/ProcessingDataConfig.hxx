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

//////////////////////////////////////////////////////////////////////////////////////
//  ProcessingDataConfig
//

#ifndef _ProcessingDataConfig_
#define _ProcessingDataConfig_

#include "DynamicType.hxx"

namespace CLAM {

	//* By now, just a base class of processingData settings: No common interfice needed

	/** Base class of ProcessingData settings, all the configurations of ProcessingData objects will be derived of this.
	 *  @see ProcessingData, DynamicType
	 */
class ProcessingDataConfig : public DynamicType
{
public:
	/** Constructor of an object that will contain the number of attributes passed by parameter
	 *  @param n Number of dynamic attributes that will be defined
	 */
	ProcessingDataConfig(const int n) : DynamicType(n) {}

	/** Copy constructor of a ProcessingData configuration object.
	 *  @param prototype Another ProcessingDataConfig type from which the structure is taken
	 *  @param shareData Tells whether the object will share the same data of the object passed by parameter or not, by default set to false
	 *  @param deep If this parameter is set to true, all the data pointed by the prototype will be copied to the new object; true by default
	 */
	ProcessingDataConfig(const ProcessingDataConfig& prototype, bool shareData=false, bool deep=true)
		: DynamicType(prototype, shareData, deep)
	{};
	virtual ~ProcessingDataConfig(){};
};

}; //namespace CLAM
#endif // _ProcessingDataConfig_


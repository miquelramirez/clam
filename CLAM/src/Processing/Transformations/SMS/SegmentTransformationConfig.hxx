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
#ifndef _SegmentTransformetionConfig_
#define _SegmentTransformetionConfig_

#include "BPF.hxx"
#include "Processing.hxx"
#include "DataTypes.hxx"

namespace CLAM
{
	
	/** Configuration class for all SegmentTransformations. It includes a float-like single value 
	 *	parameter and a BPF envelope-like parameter. Either one of these may be used to initialize 
	 *	and update the value control in an SegmentTransformation.
	 */
	class SegmentTransformationConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SegmentTransformationConfig, 3,ProcessingConfig);
		/** Type of transformation, for the time being just a string, should become
		* an enumeration of known transformation types??*/
		DYN_ATTRIBUTE (0, public, std::string, Type);
		/** Single Value Parameter */
		DYN_ATTRIBUTE (1, public, TData, Amount);
		/** BPF (envelope-like) Parameter */
		DYN_ATTRIBUTE (2, public, BPF, BPFAmount);


	protected:
		/** The single-value Amount and the On configuration are added by default. 
		 *	Should you need the BPF, it must be added explicitly
		 */
		void DefaultInit();

		/** By default, the amount is set to 0 and the On parameter to true.*/
		void DefaultValues();
	};


}


#endif // _SegmentTransformetionConfig_


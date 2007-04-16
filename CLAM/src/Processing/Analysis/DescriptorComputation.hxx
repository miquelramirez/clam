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

#ifndef _DESCRIPTOR_COMPUTATION_H_
#define _DESCRIPTOR_COMPUTATION_H_

#include "Processing.hxx"
#include "DescriptorComputationConfig.hxx"
#include "Descriptor.hxx"

namespace CLAM {

	
	/** This Processing class only calls the Compute operation of Descriptor passed to the Do 
	 */
	class DescriptorComputation
		: public Processing {
	
	protected:
		const char *GetClassName() const;

		/** Config change method
		 */
		bool ConcreteConfigure(const ProcessingConfig& c);

	public:
		DescriptorComputation();

		DescriptorComputation(const DescriptorComputationConfig &c);

		~DescriptorComputation();

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(Descriptor& input);
		bool Do(DescriptorAbs& input);
	protected:
		DescriptorComputationConfig mConfig;
	
	};

}

#endif 


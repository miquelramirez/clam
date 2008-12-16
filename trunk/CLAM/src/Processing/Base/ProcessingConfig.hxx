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

#ifndef _ProcessingConfig_
#define _ProcessingConfig_

#include "DynamicType.hxx"

namespace CLAM {

	/**
	 * This is an abstract class which serves as interface for
	 * configuration classes used as processing object construction
	 * arguments, which must derive from it.
	 * <p>
	 * Note that the required virtual methods may be provided including
	 * a "Name" dynamic attribute in the derived dynamic classes.
	 * <p>
	 * Note that processing objects constructors will take configuration
	 * objects by value; further changes in a configuration object
	 * will have no effect on the processing object constructed with it.
	 */
	class ProcessingConfig : public DynamicType {
	public:
		/**
		 * Constructor. Must be called from the initialization list
		 * of the derived classes.
		 * @param n Number of dynamic attributes
		 */
		ProcessingConfig(const int n) : DynamicType(n) {};

		/** Copy constructor.
		 * @param prototype copy source.
		 * @param shareData true if the new object is to share its data with the source */
		ProcessingConfig(const ProcessingConfig& prototype, bool shareData=false, bool deep=true)
			: DynamicType(prototype, shareData, deep)
			{};


		virtual ~ProcessingConfig();

		virtual const char * GetClassName() const {return "Processing";}
	};

}

#endif


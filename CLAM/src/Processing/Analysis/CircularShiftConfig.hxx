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

#ifndef _CircularShiftConfig_
#define _CircularShiftConfig_

#include "DataTypes.hxx"
#include "ProcessingConfig.hxx"

namespace CLAM {

	class CircularShiftConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (CircularShiftConfig, 2,ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/* Right now you can change the amount any time but it will only be taken into
		account if Type is set to eCustom */
		DYN_ATTRIBUTE (1, public, TData, Amount);

		~CircularShiftConfig(){};
	protected:
		void DefaultInit();
	};

}

#endif

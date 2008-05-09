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

#ifndef _PhaseManagementConfig_
#define _PhaseManagementConfig_
#include <typeinfo>

#include "EPhaseGeneration.hxx"
#include "ProcessingConfig.hxx"

namespace CLAM {

	/**
	 * Config class for the PhaseManagement Processing
	 */
class PhaseManagementConfig:public ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE (PhaseManagementConfig,3,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,TSize,MaxSines);
	DYN_ATTRIBUTE(1,public,TData,SamplingRate);
	DYN_ATTRIBUTE(2,public,EPhaseGeneration,Type);
public:
	void DefaultInit();
	void DefaultValues();
};

} // namespace CLAM

#endif


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

#ifndef _TOPLEVELPROCESSINGOBJECT
#define _TOPLEVELPROCESSINGOBJECT


#include "ProcessingComposite.hxx"
#include "Processing.hxx"

namespace CLAM {

	class TopLevelProcessingConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (TopLevelProcessingConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
	protected:
		void DefaultInit(void);
	};


	/**
	 * Top level processing object composite.
	 * <p>
	 * This is a singleton class used to agregate all the horphan
	 * processing objects. It will allways be the upper composite
	 * object in the processing object hierarchy.
	 */
	class TopLevelProcessing : public ProcessingComposite
	{
		TopLevelProcessing(const TopLevelProcessingConfig &c);

		TopLevelProcessingConfig mConfig;

		static TopLevelProcessing *mpInstance;

	protected:

		bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

	public:

		/** Singleton accessor */
		static TopLevelProcessing *GetInstance();

		const ProcessingConfig &GetConfig() const {return mConfig;}

		bool Do(void);
		
	};

}

#endif


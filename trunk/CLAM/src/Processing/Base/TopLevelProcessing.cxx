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

#include "TopLevelProcessing.hxx"
#include "Assert.hxx"

namespace CLAM {

	void TopLevelProcessingConfig::DefaultInit(void)
	{ 
		AddName(); 
		UpdateData(); 
	}

	TopLevelProcessing *TopLevelProcessing::mpInstance=0;

	TopLevelProcessing::
	TopLevelProcessing(const TopLevelProcessingConfig &c)
	{
		// Hack: mpParent must be set before calling Configure, because
		// otherwise Configure will try to get the TopLevel instance.
		SetOrphan();

		ConfigureOrphan(c);
	}
	
	bool TopLevelProcessing::
	ConcreteConfigure(const ProcessingConfig&c) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const TopLevelProcessingConfig&>(c);
		return true;
	}

	TopLevelProcessing* TopLevelProcessing::GetInstance()
	{
		if (!mpInstance) {
			TopLevelProcessingConfig cfg;
			cfg.SetName("TopLevel");
			mpInstance = new TopLevelProcessing(cfg);
		}
		return mpInstance;
	}

	bool TopLevelProcessing::Do()
	{
		CLAM_ASSERT(0,"TopLevelProcessing::Do(): Not implemented\n");
		return false;
	}

}

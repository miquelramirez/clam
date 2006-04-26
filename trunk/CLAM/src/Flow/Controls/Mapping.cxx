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

#include <iostream>
#include "Mapping.hxx"

namespace CLAM {
	Mapping::MappingFactories& Mapping::FactoryInstance()
	{
		static MappingFactories sFactories;
		return sFactories;
	}
	Mapping::~Mapping() {}

	Mapping* Mapping::Create(const std::string& name)
	{
		for (unsigned int i=0;i<FactoryInstance().size();i++)
		{
			if (FactoryInstance()[i]->mName == name)
			{
				return FactoryInstance()[i]->Create();
			}
		}
		return 0;
	}

	void MappingFactory::AddMe(void)
	{
		Mapping::FactoryInstance().push_back(this);
	}


	LinearMappingFactory LinearMappingFactory::sSingleton;
	NoteToFreqMappingFactory NoteToFreqMappingFactory::sSingleton;
	ValueToRatioMappingFactory ValueToRatioMappingFactory::sSingleton;
}


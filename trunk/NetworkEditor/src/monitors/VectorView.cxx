/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <CLAM/ProcessingFactory.hxx>
#include "VectorView.hxx"

namespace Hidden
{
	static const char * metadata[] = {
		"key", "VectorView",
		"category", "Monitors",
		"description", "VectorView",
		"port_monitor_type", typeid(std::vector<CLAM::TData>).name(),
		"icon", "lpmodel.svg",
		0
	};
	//static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, VectorViewMonitor> regVectorViewMonitor("VectorView");
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, VectorViewMonitor> reg = metadata;

/*	static class VectorViewMetadata
	{
	public:
		VectorViewMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("VectorView", "port_monitor_type", typeid(std::vector<CLAM::TData>).name());
			factory.AddAttribute("VectorView", "icon", "lpmodel.svg");
		}
	} dummy;*/
}

CLAM::VM::VectorView::~VectorView()
{
}


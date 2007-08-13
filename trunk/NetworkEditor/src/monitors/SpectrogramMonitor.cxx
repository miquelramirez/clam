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
#include "SpectrogramMonitor.hxx"

namespace Hidden
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, SpectrogramMonitor> regSpectrogramMonitor("Spectrogram");
	static class SpectrogramMetadata
	{
	public:
		SpectrogramMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("Spectrogram", "port_monitor_type", typeid(std::vector<CLAM::TData>).name());
			//factory.AddAttribute("Spectrogram", "icon", "keyspace.svg");
		}
	} dummy;
}


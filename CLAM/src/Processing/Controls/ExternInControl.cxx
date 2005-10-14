/*
 * Copyright (c) 2001-2005 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "ExternInControl.hxx"
#include "Factory.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<ExternInControl> regtExternInControl( "ExternInControl" );
	}
	
	bool ExternInControl::Do( const float value )
	{	
		mOutput.SendControl( (TControlData)value);
		return true;
	}

	bool ExternInControl::ConcreteConfigure(const ProcessingConfig &c)
	{
		CopyAsConcreteConfig(mConf,c);
		return true;
	}
}

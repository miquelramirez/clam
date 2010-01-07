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

#include "ControlSource.hxx"
#include "ProcessingFactory.hxx"


namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "ControlSource",
		"category", "Controls",
		"description", "ControlSource",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ControlSource> reg = metadata;
}


bool ControlSource::Do( const float value )
{	
	mOutput.SendControl( (TControlData)value);
	return true;
}

bool ControlSource::ConcreteConfigure(const ProcessingConfig &c)
{
	CopyAsConcreteConfig(mConf,c);
	if (not mConf.HasDefaultValue())
	{
		mConf.AddDefaultValue();
		mConf.UpdateData();
		mConf.SetDefaultValue(mConf.GetMinValue());
	}
	return true;
}

}


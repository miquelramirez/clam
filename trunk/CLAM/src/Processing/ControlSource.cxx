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

#include <sstream>

namespace CLAM
{

namespace 
{
	static const char * metadata[] = {
		"key", "ControlSource",
		"category", "Controls",
		"description", "ControlSource",
		"control_source_type", typeid(TControlData).name(),
		0
	};
	static FactoryRegistrator<ProcessingFactory, ControlSource> reg = metadata;
}

ControlSource::~ControlSource()
{
	for (unsigned i = 0; i < _outControls.size();	++i)
		delete _outControls[i];
}

bool ControlSource::Do( const float value, unsigned index )
{
	CLAM_ASSERT(index < _outControls.size(),
		"Index of requested controlport out-of-range");

	FloatOutControl* output = _outControls.at(index);
	output->SendControl((TControlData)value);
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

	unsigned controls = mConf.GetNrOfControls();
	ResizeControls(controls);

	return true;
}

void ControlSource::ResizeControls(unsigned controls)
{
	if (controls == _outControls.size())
		return;

	for (unsigned control = controls; control < _outControls.size(); ++control)
		delete _outControls[control];

	unsigned oldSize = _outControls.size();
	_outControls.resize(controls);

	for (unsigned control = oldSize; control < controls; ++control)
	{
		std::stringstream ss;
		if (controls == 1)
			ss << "output"; // dont break networks from the pre-multi-control era
		else
			ss << "output_" << control;
		_outControls[control] = new FloatOutControl(ss.str().c_str(), this);
	}
}

}


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

#include "ControlSink.hxx"
#include "ProcessingFactory.hxx"

#include <sstream>

namespace CLAM
{
namespace
{
	static const char * metadata[] = {
		"key", "ControlSink",
		"category", "Controls", 
		"description", "ControlSink",
		"control_sink_type", typeid(TControlData).name(),
		0
	};
	static FactoryRegistrator<ProcessingFactory, ControlSink> reg = metadata;
}
	
ControlSink::~ControlSink()
{
	for (unsigned i = 0; i < _inControls.size();	++i)
		delete _inControls[i];
}

bool ControlSink::Do( )
{
	return true;
}

float ControlSink::GetControlValue(unsigned index)
{
	CLAM_ASSERT(index < _inControls.size(),
		"Index of requested controlport out-of-range");

	FloatInControl* input = _inControls.at(index);
	return (float)input->GetLastValue();
}

bool ControlSink::ConcreteConfigure(const ProcessingConfig &c)
{
	CopyAsConcreteConfig(mConf,c);
	unsigned controls = mConf.GetNrOfControls();
	ResizeControls(controls);
	return true;
}

void ControlSink::ResizeControls(unsigned controls)
{
	if (controls == _inControls.size())
		return;

	for (unsigned control = controls; control < _inControls.size(); ++control)
		delete _inControls[control];

	unsigned oldSize = _inControls.size();
	_inControls.resize(controls);

	for (unsigned control = oldSize; control < controls; ++control)
	{
		std::stringstream ss;
		if (controls == 1)
			ss << "input control"; // dont break existng networks
		else
			ss << "input_control_" << control;
		_inControls[control] = new FloatInControl(ss.str().c_str(), this);
	}
}

}


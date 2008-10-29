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

#ifndef __MyProcessingWithPortsAndControls_hxx__
#define __MyProcessingWithPortsAndControls_hxx__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "AudioOutPort.hxx"
#include "AudioInPort.hxx"
#include "InControl.hxx"
#include "Audio.hxx"
#include <string>

namespace CLAM
{

// we don't need great configurations, just the name.
class MyProcessingWithPortsAndControlsConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (MyProcessingWithPortsAndControlsConfig, 0, ProcessingConfig);
};

class MyProcessingWithPortsAndControls : public Processing
{
private:
	MyProcessingWithPortsAndControlsConfig mConfig;

	// This processing will have an input control to get the value of modulation
	FloatInControl mModControl;
	// Two ports: one for the input data and one for the output after the processing is done. Note that
	// the instances are templatized by the type of data they receive.
	AudioInPort mInput;
	AudioOutPort mOutput;

public:
	MyProcessingWithPortsAndControls();
	MyProcessingWithPortsAndControls( const MyProcessingWithPortsAndControlsConfig & cfg );		
	virtual ~MyProcessingWithPortsAndControls();
	const char * GetClassName() const {return "MyProcessingWithPortsAndControls";}
	inline const ProcessingConfig &GetConfig() const { return mConfig;}	
	
	bool ConcreteConfigure( const ProcessingConfig & cfg );
	

	bool Do(const Audio & in, Audio & out);
	bool Do();

};

} // namespace CLAM

#endif // __MyProcessingWithPorts_hxx__


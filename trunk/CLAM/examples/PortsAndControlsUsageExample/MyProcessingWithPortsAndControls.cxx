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

#include "MyProcessingWithPortsAndControls.hxx"

namespace CLAM
{

MyProcessingWithPortsAndControls::MyProcessingWithPortsAndControls()
	// we specify for each port its name, and the processing parent
	: mModControl( "Modulation", this ),
	  mInput( "Audio Input", this ), 
	  mOutput( "Audio Output", this )
	  // for the control, just the name and the processing
{
	MyProcessingWithPortsAndControlsConfig cfg;
	Configure( cfg );
}
	
MyProcessingWithPortsAndControls::MyProcessingWithPortsAndControls( const MyProcessingWithPortsAndControlsConfig & cfg )
	: mModControl( "Modulation", this ),
	  mInput( "Audio Input", this ),
	  mOutput( "Audio Output", this )
{
	Configure( cfg );
}

MyProcessingWithPortsAndControls::~MyProcessingWithPortsAndControls()
{
}
	
bool MyProcessingWithPortsAndControls::ConcreteConfigure( const ProcessingConfig & cfg )
{	
	CopyAsConcreteConfig(mConfig, cfg);

	return true;
}
	

// this is the do with parameter: just receive a value from control and multiply the input signal with it.
bool MyProcessingWithPortsAndControls::Do(const Audio & in, Audio & out)
{
	if( !AbleToExecute() ) return true;
	
	CLAM::TData value = mModControl.GetLastValue();
	
	for(int i=0; i<in.GetSize(); i++)
	{
		out.GetBuffer()[i] = value * in.GetBuffer()[i];
	}
	return true;
}

// the automatic Do gets the data from the ports and process it. After this, notify the ports that it is finished (with Consume and Produce methods)
bool MyProcessingWithPortsAndControls::Do()
{
	bool res = Do( mInput.GetAudio(), mOutput.GetAudio() );
	mInput.Consume();
	mOutput.Produce();
	return res;
}

} // namespace CLAM


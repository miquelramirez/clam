/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _OSCSourceProcessing_
#define _OSCSourceProcessing_

//CLAM dependencies
#include "OscSource.hxx"
#include "OutControl.hxx"
#include "Processing.hxx"

namespace CLAM {

/**
* This is the configuration class for the OSCSourceProcessing.
*
* \author greg kellum [gkellum@iua.upf.edu] 08/20/2007
* \since CLAM v1.1
*/
class OSCSourceConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE(OSCSourceConfig, 2, ProcessingConfig);

	/// Every OSC recipient has an address which is shaped like a file path, 
	/// e.g./ebowSynthesizer. Use the TargetName to set the address of the events
	/// you want to receive
	DYN_ATTRIBUTE (0, public, std::string, TargetName);

	/// This configures the OSCSourceProcessing with the right number of outputs
	/// for the type of message you are receiving, i.e. one output for every
	/// message element. Although OSC supports many different message types,
	/// the outputs created here will always output floats.
	DYN_ATTRIBUTE (1, public, int, NumberOfOutputs);

protected:
	/** Dynamic type initialization: All attributes are instantiated. */
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetTargetName( "" );
	}
};

/** 
 * This is a Processing wrapper class of an OSCSource. Given the name of an OSC
 * message and the number of values in that type of message, this class creates
 * an output for each value and writes the most recent OSC value to the output
 * each time Do() is called by the network.
 * 
 * To use this class, you will need to have the oscpack library installed.
 * It can be downloaded at: http://www.audiomulch.com/~rossb/code/oscpack/
 * And you will need to compile CLAM with the flag with_osc=true 
 *
 * This class will listen for OSC messages on port 7000. Currently, it's not
 * possible to change the port.
 *
 * For more information on OSC, see http://www.cnmat.berkeley.edu/OpenSoundControl/
 * 
 * \author greg kellum [gkellum@iua.upf.edu] 08/20/2007
 * \since CLAM v1.1
 *
 */
class OSCSourceProcessing : public Processing
{
public:
	OSCSourceProcessing();
	OSCSourceProcessing(OSCSourceConfig& config);
	~OSCSourceProcessing();

	const char* GetClassName() const { return "OSCSourceProcessing"; }
	
	bool ConcreteConfigure(const ProcessingConfig&);

	const ProcessingConfig &GetConfig() const { return mConfig;}

	bool Do(void);

protected:
	bool ConcreteStart();
	bool ConcreteStop();

private:
	OSCSourceConfig mConfig;

	OSCSource mOSCSource;

	std::vector<OutControl*> outControls;
};

} // end namespace CLAM

#endif

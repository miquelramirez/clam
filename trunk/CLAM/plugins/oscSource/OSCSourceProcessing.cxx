/*
 * Copyright (c) 2007 UNIVERSITAT POMPEU FABRA
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
#include "OSCSourceProcessing.hxx"
#include "ProcessingFactory.hxx"
#include <iostream>
#include <vector>


namespace CLAM {

namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, OSCSourceProcessing> regOSCSourceProcessing("OSCSourceProcessing");
}

OSCSourceProcessing::OSCSourceProcessing()
 : 	mOSCSource() //, mFloatControl1("Pitch", this), mFloatControl2("Amplitude", this)

{
	Configure(OSCSourceConfig());
	
	mOSCSource.Start();
}

OSCSourceProcessing::OSCSourceProcessing(OSCSourceConfig& config)
 :  mOSCSource() //, mFloatControl1("Pitch", this), mFloatControl2("Amplitude", this)
{
	Configure(config);

	mOSCSource.Start();
}

OSCSourceProcessing::~OSCSourceProcessing()
{
	mOSCSource.Stop();
	
	for (int counter = 0; counter < outControls.size(); counter++)
	{
		delete outControls.at( counter );
	}

}

bool OSCSourceProcessing::ConcreteConfigure(const ProcessingConfig& cfgObject)
{
	CopyAsConcreteConfig( mConfig, cfgObject );

	if ( !mConfig.HasTargetName() || mConfig.GetTargetName() == "" )
	{
		AddConfigErrorMessage("The provided config object lacked the field 'TargetName'");
		return false;
	}

	if ( !mConfig.HasNumberOfOutputs() || mConfig.GetNumberOfOutputs() == 0 )
	{
		AddConfigErrorMessage("The provided config object lacked the field 'NumberOfOutputs'");
		return false;
	}
	// if we are reconfiguring this object, we might already have some controls
	// created. so let's see what the difference is between what we have and what
	// we need
	int numberOfControlsToCreate = mConfig.GetNumberOfOutputs() - outControls.size();
	if (numberOfControlsToCreate==0)
		return true;
	// we don't have enough controls. create more.
	if ( numberOfControlsToCreate > 0 )
	{
		for (int counter = 0; counter < numberOfControlsToCreate; counter++)
		{
			std::ostringstream os;
			os << "Float " << counter << std::flush;
			outControls.push_back( new OutControl(os.str().c_str(), this) );
		}
	}
	// (numberOfControlsToCreate < 0)
	// we have too many controls. delete some.

	// deleting controls causes the NetworkEditor to crash. so, let's
	// not do this...  
	/*
	for (int counter = outControls.size(); 
			counter > mConfig.GetNumberOfOutputs(); 
			counter--)
	{
		int indexOfOutputToDelete = counter-1;
		OutControl* pOutControl = outControls.at( indexOfOutputToDelete );
		outControls.erase( outControls.end()-1 );
		delete pOutControl;
	}
	*/

	return true;
}

bool OSCSourceProcessing::Do(void)
{
	std::vector<float>* valuesPtr = mOSCSource.GetLastValuesForTarget(mConfig.GetTargetName());
	
	if (valuesPtr == NULL) return true;
	if (valuesPtr->size() < 2) return true;

	float pitch = valuesPtr->at(0);
	float amplitude = valuesPtr->at(1);
	for (int counter = 0; counter < valuesPtr->size() && counter < outControls.size(); counter++)
	{
		float value = valuesPtr->at(counter);
		OutControl* pOutControl = outControls.at( counter );
		pOutControl->SendControl(value);
	}
	return true;
}

bool OSCSourceProcessing::ConcreteStart()
{
}

bool OSCSourceProcessing::ConcreteStop()
{
	mOSCSource.Stop();
}

} // end namespace CLAM

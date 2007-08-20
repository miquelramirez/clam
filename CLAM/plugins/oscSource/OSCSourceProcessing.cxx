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
	else
	{
		// if we are reconfiguring this object, we might already have some controls
		// created. so let's see what the difference is between what we have and what
		// we need
		int numberOfControlsToCreate = mConfig.GetNumberOfOutputs() - outControls.size();
		
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
		// we have too many controls. delete some.
		else if (numberOfControlsToCreate < 0)
		{
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
		}
	}
	
	return true;
}

bool OSCSourceProcessing::Do(void)
{
	std::vector<float>* valuesPtr = mOSCSource.GetLastValuesForTarget(mConfig.GetTargetName());
	float pitch;
	float amplitude;
	if (valuesPtr == NULL || valuesPtr->size() < 2)
	{
		pitch = 0.0;
		amplitude = 0.0;
	}
	else
	{
		pitch = valuesPtr->at(0);
		amplitude = valuesPtr->at(1);
		for (int counter = 0; counter < valuesPtr->size() && counter < outControls.size(); counter++)
		{
			float value = valuesPtr->at(counter);
			OutControl* pOutControl = outControls.at( counter );
			pOutControl->SendControl(value);
		}

	}
}

bool OSCSourceProcessing::ConcreteStart()
{
}

bool OSCSourceProcessing::ConcreteStop()
{
	mOSCSource.Stop();
}

} // end namespace CLAM

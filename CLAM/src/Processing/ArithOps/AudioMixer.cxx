

#include "AudioMixer.hxx"

namespace CLAM
{

AudioMixer::AudioMixer() : mOutputPort("Output Audio",this)
{
	AudioMixerConfig cfg;
	Configure( cfg );
}


void AudioMixer::CreatePortsAndControls()
{
	for( int i=0; i<mConfig.GetNumberOfInPorts(); i++ )
	{
		std::stringstream number("");
		number << i;
		AudioInPort * inPort = new AudioInPort( "Input " + number.str(), this );
		inPort->SetSize( mConfig.GetFrameSize() );
		inPort->SetHop( mConfig.GetFrameSize() );
		mInputPorts.push_back( inPort );
		
		mInputControls.push_back( new InControl("Gain " + number.str(), this) );
	}
	for( int i=0; i<mConfig.GetNumberOfInPorts(); i++ )
	{
		/* Set gain = 1 by default */
		mInputControls[i]->DoControl(1.);
	}
	
	mOutputPort.SetSize( mConfig.GetFrameSize());
	mOutputPort.SetHop( mConfig.GetFrameSize());
}

void AudioMixer::RemovePortsAndControls()
{
	std::vector< AudioInPort* >::iterator itInPort;
	for(itInPort=mInputPorts.begin(); itInPort!=mInputPorts.end(); itInPort++)
		delete *itInPort;
	mInputPorts.clear();

	std::vector< InControl* >::iterator itInControl;
	for(itInControl=mInputControls.begin(); itInControl!=mInputControls.end(); itInControl++)
		delete *itInControl;
	mInputControls.clear();  
			
	GetInPorts().Clear();
	GetInControls().Clear();
}

bool AudioMixer::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	RemovePortsAndControls();
	CreatePortsAndControls();
	return true;
}

bool AudioMixer::Do()
{
	unsigned int frameSize = mConfig.GetFrameSize();
	unsigned int numInPorts = mConfig.GetNumberOfInPorts();

	TData normConstant = (TData)1.0 /TData(numInPorts);
	
	for (unsigned int sample=0; sample < frameSize; sample++) 
	{
		TData sum=0.0;
		
		for (unsigned int inPort=0; inPort< numInPorts; inPort++)
		{
			TData valueModified = mInputPorts[inPort]->GetData(sample) * mInputControls[inPort]->GetLastValue();
			sum += valueModified;
		}
		mOutputPort.GetData(sample) = sum * normConstant;
	}

	// execute consume/produce methods	
	for (unsigned int inPort=0; inPort<numInPorts; inPort++)
		mInputPorts[inPort]->Consume();
	mOutputPort.Produce();
	
	return true;
}

} // namespace CLAM


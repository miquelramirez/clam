
#include "OutControlSender.hxx"
#include <sstream>

#include <iostream> // TODO: remove


namespace CLAM
{

void OutControlSenderConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
}

OutControlSender::OutControlSender()
{
	OutControlSenderConfig cfg;
	Configure(cfg);
}

OutControlSender::OutControlSender( const OutControlSenderConfig & cfg)
{
	Configure(cfg);
}


bool OutControlSender::Do()
{
	if( !AbleToExecute() ) return true;
	std::list<OutControl*>::iterator it;
	for( it=mOutputs.begin(); it!=mOutputs.end(); it++ )
		(*it)->SendControl(1.0);


	return true;
}

bool OutControlSender::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	int difference = mConfig.GetNumControls() - mOutputs.size();
	
	if( difference > 0 ) // we need to create more controls
	{
		for(int i=0;i<difference;i++)
		{
			std::stringstream name("");
			name << "out_" << mOutputs.size();
			mOutputs.push_back( new OutControl( name.str(), this ));			
		}
		return true;
	}	

	if( difference < 0 ) // we need to delete some controls
	{
		std::list<OutControl * >::iterator it;
		for(it=mOutputs.begin(); it!=mOutputs.end(); it++ )
			delete *it;
		
		mOutputs.clear();
		GetOutControls().Clear();
		for(int i=0;i<mConfig.GetNumControls();i++)
		{	
			std::stringstream name("");
			name << "out_" << mOutputs.size();
			mOutputs.push_back( new OutControl( name.str(), this ));			
		}
		return true;
	}
	return true;
}

} // namespace CLAM




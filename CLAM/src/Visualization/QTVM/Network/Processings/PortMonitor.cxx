#include "PortMonitor.hxx"
#include "Factory.hxx"

namespace CLAM
{

	void PortMonitorConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}

	template <>
	bool PortMonitor<Audio,AudioInPort>::Do()
	{
		if(!AbleToExecute()) return true;
		unsigned whichDataToWrite = mWhichDataToRead?0:1;
		mData[whichDataToWrite] = mInput.GetAudio();
		{
			TryMutex::ScopedTryLock lock(mSwitchMutex,true);
			if (lock.Locked())
				mWhichDataToRead = whichDataToWrite;
		}
		mInput.Consume();
		return true;
	}

	template <>
	PortMonitor<Audio,AudioInPort>::PortMonitor()
		: mInput("Input", this)
	{
		PortMonitorConfig cfg;
		Configure(cfg);
		mData[0].SetSize(256);
		mData[1].SetSize(256);
	}

	template <>
	PortMonitor<Audio,AudioInPort>::PortMonitor(const PortMonitorConfig& cfg)
		: mInput("Input", this)
	{
		Configure(cfg);
		mData[0].SetSize(256);
		mData[1].SetSize(256);
	}


	static CLAM::Factory<CLAM::Processing>::Registrator<CLAM::PeaksPortMonitor> regtPeaksPortMonitor("PeaksPortMonitor");
	static CLAM::Factory<CLAM::Processing>::Registrator<CLAM::SpectrumPortMonitor> regtSpectrumPortMonitor("SpectrumPortMonitor");;
	static CLAM::Factory<CLAM::Processing>::Registrator<CLAM::AudioPortMonitor> regtAudioPortMonitor("AudioPortMonitor");
	static CLAM::Factory<CLAM::Processing>::Registrator<CLAM::FundamentalPortMonitor> regtFundamentalPortMonitor("FundamentalPortMonitor");
	
}

// END


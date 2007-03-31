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
		mSigNewData.Emit();
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
	    : mInput("Input", this),mWhichDataToRead(0)
	{
		PortMonitorConfig cfg;
		Configure(cfg);
	}

	template <>
	PortMonitor<Audio,AudioInPort>::PortMonitor(const PortMonitorConfig& cfg)
	    : mInput("Input", this),mWhichDataToRead(0)
	{
		Configure(cfg);
	}
#ifndef QT_PLUGIN
	static CLAM::Factory<CLAM::Processing>::Registrator<CLAM::PeaksPortMonitor> regtPeaksPortMonitor("PeaksPortMonitor");
	static CLAM::Factory<CLAM::Processing>::Registrator<CLAM::SpectrumPortMonitor> regtSpectrumPortMonitor("SpectrumPortMonitor");
	static CLAM::Factory<CLAM::Processing>::Registrator<CLAM::AudioPortMonitor> regtAudioPortMonitor("AudioPortMonitor");
	static CLAM::Factory<CLAM::Processing>::Registrator<CLAM::FundamentalPortMonitor> regtFundamentalPortMonitor("FundamentalPortMonitor");
	static CLAM::Factory<CLAM::Processing>::Registrator<CLAM::AudioBuffPortMonitor> regtAudioBuffPortMonitor("AudioBuffPortMonitor");
	static CLAM::Factory<CLAM::Processing>::Registrator<CLAM::SpecgramPortMonitor> regtSpecgramPortMonitor("SpecgramPortMonitor");
	static CLAM::Factory<CLAM::Processing>::Registrator<CLAM::FundTrackPortMonitor> regtFundTrackPortMonitor("FundTrackPortMonitor");
	static CLAM::Factory<CLAM::Processing>::Registrator<CLAM::SinTracksPortMonitor> regtSinTracksPortMonitor("SinTracksPortMonitor");
#endif
	
}

// END


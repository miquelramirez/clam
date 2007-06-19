#include "PortMonitor.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

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
	PortMonitor<Audio,AudioInPort>::PortMonitor(const Config& cfg)
	    : mInput("Input", this)
		, mWhichDataToRead(0)
	{
		Configure(cfg);
	}
#ifndef QT_PLUGIN
	static FactoryRegistrator<ProcessingFactory, PeaksPortMonitor> regPeaksPortMonitor("PeaksPortMonitor");
	static FactoryRegistrator<ProcessingFactory, SpectrumPortMonitor> regSpectrumPortMonitor("SpectrumPortMonitor");
	static FactoryRegistrator<ProcessingFactory, AudioPortMonitor> regAudioPortMonitor("AudioPortMonitor");
	static FactoryRegistrator<ProcessingFactory, FundamentalPortMonitor> regFundamentalPortMonitor("FundamentalPortMonitor");
	static FactoryRegistrator<ProcessingFactory, AudioBuffPortMonitor> regAudioBuffPortMonitor("AudioBuffPortMonitor");
	static FactoryRegistrator<ProcessingFactory, SpecgramPortMonitor> regSpecgramPortMonitor("SpecgramPortMonitor");
	static FactoryRegistrator<ProcessingFactory, FundTrackPortMonitor> regFundTrackPortMonitor("FundTrackPortMonitor");
	static FactoryRegistrator<ProcessingFactory, SinTracksPortMonitor> regSinTracksPortMonitor("SinTracksPortMonitor");
#endif
	
} // namespace CLAM


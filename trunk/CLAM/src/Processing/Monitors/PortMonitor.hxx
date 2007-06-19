#ifndef __PortMonitor_hxx__
#define __PortMonitor_hxx__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "Mutex.hxx"

// Temporary until concrete classes will be separated
#include "SpectralPeakArray.hxx"
#include "Spectrum.hxx"
#include "Fundamental.hxx"
#include "InPort.hxx"
#include "Audio.hxx"
#include "AudioInPort.hxx"

// sigslot
#include "Signalv0.hxx"
#include "Slotv0.hxx"

namespace CLAM
{
	/**
	 * A processing that allows other (GUI) thread to monitor a port in a thread safe way.
	 * Subclass specializations of this template in order to use it.
	 * @code
	 * class SpectrumPortMonitor : public CLAM::PortMonitor<CLAM::Spectrum>
	 * {
	 * 	public:
	 *	const char * GetClassName() const {return "SpectrumPortMonitor";}
	 * };
	 * @endcode
	 *
	 * In order to get data from a GUI thread you should use a PortMonitor::DataLocker
	 * @code
	 *
	 * // Supose 'monitor' being a SpectrumPortMonitor reference.
	 * ...
	 * {
	 * 	const Spectrum & spectrum = monitor.FreezeAndGetData();
	 * 	// The monitor won't overwrite the current copy until Release.
	 * 	
	 * 	// Do what ever with the object
	 *
	 * 	visualize(spectrum);
	 * 	
	 * 	monitor.UnfreezeData();
	 * 	// After unfreezing, 'spectrum' is not safe to use.
	 * 	// and the monitor will be allowed to swap copies
	 * }
	 * @endcode
	 */
	template <typename TheDataType, typename ThePortType=InPort<TheDataType> >
	class PortMonitor : public Processing
	{
	public:
		typedef TheDataType DataType;
		typedef ThePortType PortType;

		inline PortMonitor(const Config & cfg= Config() );
		inline virtual ~PortMonitor();

		inline bool Do();

		const char * GetClassName() const {return "PortMonitor";}

		inline const DataType & FreezeAndGetData();
		inline void UnfreezeData();

		void AttachStartSlot(SigSlot::Slotv0& slot) {mSigStart.Connect(slot);}
		void AttachStopSlot(SigSlot::Slotv0& slot) { mSigStop.Connect(slot);}
		void AttachSlotNewData(SigSlot::Slotv0& slot) { mSigNewData.Connect(slot);}

	protected:
		bool ConcreteStart() { mSigStart.Emit(); return true;}
		bool ConcreteStop() { mSigStop.Emit(); return true;}


	private:
		PortType          mInput;
		DataType          mData[2];
		TryMutex          mSwitchMutex;
		unsigned          mWhichDataToRead;
		SigSlot::Signalv0 mSigStart;
		SigSlot::Signalv0 mSigStop;
		SigSlot::Signalv0 mSigNewData;
	};

	namespace Hidden
	{
		template <typename T>
		static void initData(DynamicType * selector, T & data)
		{
			data.AddAll();
			data.UpdateData();
		}
		template <typename T>
		static void initData(void * selector, T & data)
		{
		}
	}

	template <typename PortDataType, typename PortType>
	PortMonitor<PortDataType,PortType>::PortMonitor(const Config& cfg)
		: mInput("Input", this)
		, mWhichDataToRead(0)
	{
		Configure(cfg);
		Hidden::initData(&mData[0],mData[0]);
		Hidden::initData(&mData[1],mData[1]);
	}

	template <typename PortDataType, typename PortType>
	PortMonitor<PortDataType,PortType>::~PortMonitor()
	{
	}

	template <typename PortDataType, typename PortType>
	const typename PortMonitor<PortDataType,PortType>::DataType & PortMonitor<PortDataType,PortType>::FreezeAndGetData()
	{
		Hidden::LockOps<TryMutex>::Lock(mSwitchMutex);
		return mData[mWhichDataToRead];
	}

	template <typename PortDataType, typename PortType>
	void PortMonitor<PortDataType,PortType>::UnfreezeData()
	{
		Hidden::LockOps<TryMutex>::Unlock(mSwitchMutex);
	}

	template <typename PortDataType, typename PortType>
	bool PortMonitor<PortDataType,PortType>::Do()
	{
		if(!AbleToExecute()) return true;
		unsigned whichDataToWrite = mWhichDataToRead?0:1;
		mData[whichDataToWrite] = mInput.GetData();
		mSigNewData.Emit();
		{
			TryMutex::ScopedTryLock lock(mSwitchMutex,true);
			if (lock.Locked())
				mWhichDataToRead = whichDataToWrite;
		}
		mInput.Consume();
		return true;
	}



	class PeaksPortMonitor : public PortMonitor <SpectralPeakArray>
	{
	public:
		const char * GetClassName() const {return "PeaksPortMonitor";}
	};
	class SinTracksPortMonitor : public PortMonitor<SpectralPeakArray>
	{
	public:
		const char * GetClassName() const {return "SinTracksPortMonitor";}
	};
	class SpectrumPortMonitor : public PortMonitor <Spectrum>
	{
	public:
		const char * GetClassName() const {return "SpectrumPortMonitor";}
	};
	class SpecgramPortMonitor : public PortMonitor<Spectrum>
	{
	public:
		const char * GetClassName() const {return "SpecgramPortMonitor";}
	};
	class FundamentalPortMonitor : public PortMonitor <Fundamental>
	{
	public:
		const char * GetClassName() const {return "FundamentalPortMonitor";}
	};
	class FundTrackPortMonitor : public PortMonitor<Fundamental>
	{
	public:
		const char * GetClassName() const {return "FundTrackPortMonitor";}
	};

	template <>
	bool PortMonitor<Audio,AudioInPort>::Do();

	template <>
	PortMonitor<Audio,AudioInPort>::PortMonitor(const Config& cfg);

	class AudioPortMonitor : public PortMonitor <Audio,AudioInPort>
	{
	public:
		const char * GetClassName() const {return "AudioPortMonitor";}
	};
	class AudioBuffPortMonitor : public PortMonitor<Audio,AudioInPort>
	{
	public:
		const char * GetClassName() const {return  "AudioBuffPortMonitor";}
	};
}

#endif


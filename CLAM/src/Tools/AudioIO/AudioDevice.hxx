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
#ifndef __AudioDevice__
#define __AudioDevice__

#include "AudioManager.hxx"
#include "Audio.hxx"

namespace CLAM{



/** This is the abstract base class for an audio device. With an audio device
 * we refer to any kind of (multichannel) audio input/output/fullduplex.
 * AudioDevices are usually created by the AudioManager. The interface 
 * for the user however, are the AudioIn and AudioOut classes.
 * <p>
 * Each AudioIn and AudioOut created is registered with the AudioManager. When the AudioManager is started, it will create the appropiate AudioDevice or AudioDevices, based on the number of inputs, outputs, operating system and AudioIOConfig of the AudioIn and AudioOut objects.
 * @see AudioIn, AudioOut, AudioDeviceList, AudioManager
 */
class AudioDevice
{
	friend class AudioIn;
	friend class AudioOut;
	friend class AudioManager;
public:
	/** This class is used to respond to device information
	 *	request using the GetInfo method.
	 */
	struct TInfo
	{
		std::string mName;
		std::string mArch;
		/** Number of used Read channels*/
		int mNReadChannels;
		/** Number of used Write channels*/
		int mNWriteChannels;
		/** Number of channels configured in the PCM device.
		 *  max( mNReadChannels , mNWriteChannels)
		 */
		int mNChannels;
		int mSampleRate;
		unsigned int mNotifySize;

		/** Base constructor, calls to Reset method to clear all the values*/
		TInfo() { Reset();}

		/** Clear all the values of object, setting strings mName and mArch to "" and integer values mNChannels, mSampleRate and mNotifySize to 0
		 */
		void Reset()
		{
			mName=mArch="";
			mNChannels=mNReadChannels=mNWriteChannels=mSampleRate=mNotifySize=0;
		}
	};
	
	std::vector<AudioIn*> mInputs;
	std::vector<AudioOut*> mOutputs;
	
	std::string mName;
	bool mForceNChannels;
	int mNChannels;
	int mNReadChannels;
	int mNWriteChannels;
	
	/** Constructor of the class that sets the name of object to the string passed by parameter
	 *  @arg name String with the name of object
	 */
	AudioDevice(const std::string& name)
	{
		mName = name;
		mNChannels = mNReadChannels = mNWriteChannels = 0;
		mForceNChannels=false;
		mAudioManager=0;
	}
	
	/** Destructor of class*/
	virtual ~AudioDevice() { };
	
	/** This method must be called to begin the use of this Device. Must be implemented by any Device derived from this class*/
	virtual void Start(void) throw(Err) = 0;
	/** This method must be called to end the use of this Device. Must be implemented by any Device derived from this class*/
	virtual void Stop(void) throw(Err) = 0;
	/** Reads the information given by this Device and passes data to an Audio chunk. Must be implemented by any Device derived from this class.
	 *  @param audio Audio object where data will be stored.
	 *  @param channelID Channel to read.
	 */
	virtual void Read(Audio& audio,const int channelID) = 0;

	/** Writes the information given by an Audio chunk in the Devices. Must be implemented by any Device derived from this class.
	 *  @param audio Audio object with values that must to be passed to Devices
	 *  @param channelID Channel to write
	 */
	virtual void Write(const Audio& audio,const int channelID) = 0;
	
	/** Getter for the Info of Device Object attached to this AudioDevices instantiation
	 *  @param info TInfo object that method will modify with the values of Tinfo internal object
	 */
	virtual void GetInfo(TInfo&);

	/** Setter for the number of channels to allocate in the hardware.
	 *  This is necessary for certain hardware which needs a fixed number
	 *  of channels (ADAT or SPDIF).
	 *  @param Number of channels
	 */
	virtual void SetNChannels(int channels);
	
protected:
	
	bool Register(AudioManager* am,AudioIn& in);
	bool Register(AudioManager* am,AudioOut& out);
	void Unregister(AudioIn& in);
	void Unregister(AudioOut& out);
	
	int SampleRate(void);
	int Latency(void);
	void SetLatency(int latency);

private:
	AudioManager* mAudioManager;
	AudioManager& _AudioManager(void);
	void _SetAudioManager(AudioManager* am);
};

};//CLAM

#endif


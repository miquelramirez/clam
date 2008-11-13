/* Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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



#ifndef __AudioOut__
#define __AudioOut__

#include "AudioIO.hxx"
#include "Processing.hxx"
#include "Audio.hxx"
#include "AudioDevice.hxx"
#include "AudioInPort.hxx"

namespace CLAM{

/** This class is the interface to an output of an AudioDevice.
 *  @see Processing, AudioIOConfig, AudioOut, AudioManager, AudioDevice
 */
class AudioOut: public Processing
{
friend class AudioManager;
friend class AudioDevice;
private:
	AudioIOConfig mConfig;
	AudioDevice* mpDevice;

	AudioInPort mInput;
public:

	/** Configuration method interface. The Processing base class forces all the concrete classes derived from it to implement this method, which must actually perform the specific configuration tasks.  
	 *  Note that the user can not call this method directly. He will use Configure instead. The argument is expected to be an object of the necesary concrete configuration class.
	 *  @return true if the object has been configured correctly; true otherwise
	 *  @param The related ProcessingConfig object
	 *  @throw A bad_cast exception if the arguments is not the expected configuration class
	 */
	bool ConcreteConfigure(const ProcessingConfig& c)
	 throw(ErrProcessingObj);

	/** Getter for the configuration of the class
	 *  @return The ProcessingConfig object attached to this Processing object
	 */		
	const ProcessingConfig &GetConfig() const { return mConfig;}


	/** Default constructor for the class. Sets the configuration to the default values, calling the base constructor of the AudioIOConfig object
	*/
	AudioOut();

	/** Constructor of the class with an AudioIOConfig object constructed by the user as parameter.
	 *  @param c The concrete AudioIOConfig that will be used for this construction
	 */
	AudioOut( const AudioIOConfig& cfg );

	/** Destructor method of the class*/
	~AudioOut();
	

	const char * GetClassName() const {return "AudioOut";}
	
	/** Supervised mode of Do method. Calls the non-supervised method with the Audio data chunk attached before as the parameter where data will be written
	 */
	bool Do(void);

	/**
	 * Non supervised mode of Do function. The object writes in the device attached for the appropiate channel. 
	 * Values selected to write are provided for the data chunk passed by parameter. On current implementations 
	 * Do() will return immediately when not all output channels of the device has been 'filled' yet. If this is 
	 * last channel to be 'filled', Do() will block, until the device is ready to receive more data. The size of 
	 * the data chunk passed has restrictions which are dependent on the implementation. Most will require the 
	 * size to be a power-of-two somewhere in the region of 32 samples to 8192 samples.
	 * @param data The Audio chunk that we want to pass to the selected Device
	 * @return true if the method has been executed correctly
	 */
	bool Do(const Audio& data)
	{
		if (!AbleToExecute()) return true; // object was disabled
		
		mpDevice->Write(data,mConfig.GetChannelID());
		return true;
	}

	/** Getter for the Info of Device Object attached to this AudioIn instantiation
	 *  @param info TInfo object that method will modify with the values of Tinfo internal object
	 */
	void GetDeviceInfo(AudioDevice::TInfo &) const;

	int GetChannelID() const {return mConfig.GetChannelID();}
	
	/** Wether the processing is a sync source such as audio i/o device,
	 * or an audio callback hook (i.e. Externalizer) */
	bool IsSyncSource() const { return true; }

protected:
	bool ConcreteStart();
	bool ConcreteStop();
};

};//CLAM

#endif


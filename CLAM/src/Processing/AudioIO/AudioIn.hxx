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



#ifndef __AudioIn__
#define __AudioIn__

#include "AudioIO.hxx"
#include "Processing.hxx"
#include "Audio.hxx"
#include "AudioOutPort.hxx"
#include "AudioDevice.hxx"


namespace CLAM{

/** This class is the interface to an input of an AudioDevice
 *  @see Processing, AudioIOConfig, AudioOut, AudioManager, AudioDevice
 */
class AudioIn: public Processing
{
friend class AudioManager;
friend class AudioDevice;
private:
	AudioIOConfig mConfig;
	AudioDevice* mpDevice;
public:
	AudioOutPort mOutput;

	/** Configuration method interface. The Processing base class forces all the concrete classes derived from it to implement this method, which must actually perform the specific configuration tasks.  
	 *  Note that the user can not call this method directly. He will use Configure instead. The argument is expected to be an object of the necesary concrete configuration class.
	 *  @return true if the object has been configured correctly; true otherwise
	 *  @param The related ProcessingConfig object
	 */
	bool ConcreteConfigure(const ProcessingConfig& c);

	/** Getter for the configuration of the class
	 *  @return The ProcessingConfig object attached to this Processing object
	 */		
	const ProcessingConfig &GetConfig() const { return mConfig;}

	/** Default constructor for the class. Sets the configuration to the default values, calling the base constructor of the AudioIOConfig object
	*/
	AudioIn() :mOutput( "Audio Output",this ) { mpDevice = 0; Configure(AudioIOConfig()); }

	/** Constructor of the class with an AudioIOConfig object constructed by the user as parameter.
	 *  @param c The concrete AudioIOConfig that will be used for this construction
	 */
	AudioIn(const AudioIOConfig &c) : mOutput( "Audio Output",this )  { mpDevice = 0; Configure(c); }
	/** Destructor method of the class*/
	~AudioIn() { if (mpDevice) mpDevice->Unregister(*this); }

	const char * GetClassName() const {return "AudioIn";}

	/** Supervised mode of Do method. Calls the non-supervised method with the Audio data chunk attached before as the parameter where the data will be passed 
	 */
	bool Do(void);

	/** Non supervised mode of Do function. The object reads the device attached for the appropiate channel and puts the output in the Audio data chunk passed by parameter
	 * @param data The Audio chunk where we want to put the data that object receives of Device
	 * @return true if the method has been executed correctly
	 */
	bool Do(Audio& data)
	{
		if (!AbleToExecute()) return true; // object was disabled

		mpDevice->Read(data,mConfig.GetChannelID());
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
	bool ConcreteStart(void);
};


};//CLAM
#endif // __AudioIn__


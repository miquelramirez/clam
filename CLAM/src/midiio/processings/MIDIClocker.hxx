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

#ifndef CLAM_MIDIClocker_hxx
#define CLAM_MIDIClocker_hxx

#include "Processing.hxx"
#include "MIDIDevice.hxx"
#include "MIDIEnums.hxx"

namespace CLAM { class MIDIClocker; }


/** This class is the interface to an input of an MIDIDevice.
 *  @see Processing, MIDIClocker::Config, MIDIOut, MIDIManager, MIDIDevice
 */
class CLAM::MIDIClocker : public Processing
{
	friend class MIDIManager;
	friend class MIDIDevice;

public:
	/** Configuration of the MIDIClocker class. You can specify
	 * a device (string), which will be used by the MIDIManager, when creating
	 * the MIDIDevice objects. When you don't specify a concrete device, or
	 * specify "default", the MIDIManager will choose the default device for your
	 * setup. For multichannel MIDI, you need to configure each in/out with it's
	 * own channel id, typically 0 for left, and 1 for right.
	 * @see MIDIClocker, MIDIDevice, MIDIManager, ProcessingConfig
	 */
	class Config: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Config, 1, ProcessingConfig);

		/** The Device that will be used by this MIDIClocker or MIDIOut instantiation, if it is not indicated, or its value is "default:default", system will choose the best device for this object*/
		DYN_ATTRIBUTE (0, public, std::string, Device);

	protected:
		void DefaultInit(void)
		{
			AddDevice();
			UpdateData();

			SetDevice("default:default");
		}
	};

protected:
	Config mConfig;
	MIDIDevice* mpDevice;
	FloatInControl mInput;
public:

	/** Configuration method interface. The Processing base class forces all the concrete classes derived from it to implement this method, which must actually perform the specific configuration tasks.  
	 *  Note that the user can not call this method directly. He will use Configure instead. The argument is expected to be an object of the necesary concrete configuration class.
	 *  @return true if the object has been configured correctly; true otherwise
	 *  @param The related ProcessingConfig object
	 *  @throw ErrProcessingObj when there is an error on device adquisition
	 */
	virtual bool ConcreteConfigure(const ProcessingConfig& c)
		throw(ErrProcessingObj);
	
	/** Getter for the configuration of the class
	 *  @return The ProcessingConfig object attached to this Processing object
	 */		
	const ProcessingConfig &GetConfig() const { return mConfig;}

	/** 
	 *  @param c Initialization config
	 */		
	MIDIClocker(const Config &c = Config())
		: mInput("input",this,&MIDIClocker::DoClock)
	{
		mpDevice = 0;
		Configure(c);
	}

	/** Destructor of the class*/
	~MIDIClocker() { if (mpDevice) mpDevice->Unregister(*this); }

	const char * GetClassName() const {return "MIDIClocker";}

	/** Non supervised mode of Do function. Non implemented yet
	 */
	bool Do(void)
	{ 
		return true;
	}

	/** Getter for the Info of Device Object attached to this MIDIClocker instantiation
	 *  @param info TInfo object that method will modify with the values of Tinfo internal object
	 */
	void GetDeviceInfo(MIDIDevice::TInfo &) const;

	/*
	 *  @param val The value received with this message
	 *  @return 1 if method has been executed correctly
	 */
	void DoClock(TControlData val)
	{
		if (mpDevice) mpDevice->SetClock(val);
	}

protected:
	bool ConcreteStart();

};



#endif


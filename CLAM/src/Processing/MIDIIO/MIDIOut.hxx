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

#ifndef __MIDIOut__
#define __MIDIOut__

#include "Processing.hxx"
#include "MIDIDevice.hxx"
#include "MIDIEnums.hxx"
#include "MIDIIOConfig.hxx"

namespace CLAM {

/** This class is the interface to an input of an MIDIDevice.
 *  @see Processing, MIDIInConfig, MIDIIn, MIDIManager, MIDIDevice
 */
class MIDIOut: public Processing
{
friend class MIDIManager;
friend class MIDIDevice;
protected:
	MIDIIOConfig mConfig;
	MIDIDevice* mpDevice;
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
protected:
	virtual void Handle(unsigned char* msg,int size) = 0;
public:
	/** Constructor of the class with a boolean as parameter to choose whether
	 *  to configure the object. It has been implemented by this way to solve
	 *  a problem between constructor of this class and constructors of classes
	 *  derived of MIDIOut. The derived class constructor will call the MIDIOut
	 *  constructor which will call Configure. Obviously, this will cause
	 *  problems, when the ConcreteConfigure is reimplemented in the derived
	 *  class, but the MIDIOut constructor calls MIDIOut::ConcreteConfigure
	 *  instead. The solution is a default constructor of MIDIOut with the
	 *  signature MIDIOut::MIDIOut(bool configure = true). The configure flag
	 *  indicated that the constructor should call Configure (by default),
	 *  but not in the case of derived classes.
	 *  @param configure Boolean parameter to decide if configure object, like
	 *  explained before. True by default.
	 */
	MIDIOut(bool configure = true)
	{ 
		mpDevice = 0;
		if (configure)
		{
			Configure(MIDIIOConfig());
		}
	}

	/** Constructor of the class with an MIDIIOConfig object constructed by
	 *  the user as parameter.
	 *  @param c The concrete MIDIIOConfig that will be used for this
	 *  construction
	 */		
	MIDIOut(const MIDIIOConfig &c)
	{
		mpDevice = 0;
		Configure(c);
	}

	/** Destructor unregisters this MIDIOut instance from the associated 
	 *  device */
	~MIDIOut() { if (mpDevice) mpDevice->Unregister(*this); }

	const char * GetClassName() const {return "MIDIOut";}

	/** Non supervised mode of Do function. Non implemented yet
	 */
	bool Do(void)
	{ 
		return true;
	}

	/** Getter for the Info of Device Object attached to this MIDIOut
	 *  instantiation
	 *  @param info TInfo object that method will modify with the values of
	 *  Tinfo internal object
	 */
	void GetDeviceInfo(MIDIDevice::TInfo &) const;

protected:
	bool ConcreteStart(void);
};

} // namespace CLAM
#endif

